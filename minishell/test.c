#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    // Initialize Readline library
    rl_initialize();
    using_history();
    stifle_history(100);  // Set max history entries to 100

    // Read input from user and add to history
    while (1) {
        char* input = readline("Enter input: ");
        if (!input) {
            break;  // Exit loop on EOF
        }
        add_history(input);
        free(input);
    }

    // Print history list
    HIST_ENTRY** history_list = history_list();
    if (history_list) {
        for (int i = 0; history_list[i] != NULL; i++) {
            printf("%s\n", history_list[i]->line);
        }
    }

    // Save history to file
    write_history(".myhistory");

    return 0;
}