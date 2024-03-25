#!/bin/bash

# Program name to test
PROGRAM_NAME="ls"

# Check if expect is installed
if ! command -v expect &> /dev/null; then
    echo "expect could not be found. Please install expect to run this script."
    exit 1
fi

# Expect script to run tests and exit QEMU
run_tests_and_exit_qemu() {
    expect -c "
    set timeout -1
    spawn make qemu
    expect \"init: starting sh\"

    # Initialize variable to hold results
    set results \"\"

    for {set i 1} {\$i <= 10} {incr i 1} {
        send \"timetest $PROGRAM_NAME\r\"
        expect {
            -re {${PROGRAM_NAME} ran in ([0-9]+) ticks and ([0-9]+) cycles} {
                set ticks \$expect_out(1,string)
                set cycles \$expect_out(2,string)
                # Append results to the variable
                append results \"Test \$i: \$ticks ticks, \$cycles cycles\n\"
            }
        }
    }

    # Print all results
    puts \"\nAll Test Results:\n\$results\"

    # Sending Ctrl+A and then 'x' to exit QEMU
    send \"\001\"; # Ctrl+A
    send \"x\r\"
    expect eof
    "
}

# Run the tests and exit QEMU
run_tests_and_exit_qemu
