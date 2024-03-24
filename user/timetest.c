#include "kernel/types.h"
#include "user/user.h"

void uint64_to_string(uint64 n, char *str) {
    int i = 0;
    if (n == 0) {
        str[i++] = '0'; // Handle zero explicitly, otherwise empty string
    }

    // Extract digits from the least significant digit
    while (n > 0) {
        str[i++] = (n % 10) + '0'; // Convert digit to ASCII and store
        n /= 10;
    }
    str[i] = '\0'; // Null-terminate the string

    // Reverse the string since we built it backwards
    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: timetest command [args]\n");
        exit(1);
    }

    int start_ticks = uptime();
    uint64 start_cycles = systime();
    int pid = fork();

    if (pid < 0) {
        fprintf(2, "fork failed\n"); // print to stderr
        exit(1);
    } else if (pid == 0) {
        exec(argv[1], argv + 1);
        fprintf(2, "exec %s failed\n", argv[1]); // print to stderr
        exit(1);
    } else {
        wait(0);

        int end_ticks = uptime();
        uint64 end_cycles = systime();

        int ticks_taken = end_ticks - start_ticks;
        uint64 cycles_taken = end_cycles - start_cycles;
        char numStr[21]; // uint64 max value has 20 digits + null terminator
        uint64_to_string(cycles_taken, numStr);

        // print to stdout
        fprintf(1, "%s ran in %d ticks and %s cycles\n", argv[1], ticks_taken, numStr); 
    }
    
    exit(0);
}
