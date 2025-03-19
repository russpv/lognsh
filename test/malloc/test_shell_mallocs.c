#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <termios.h>
#include <errno.h>
#include <ctype.h>
#include "../../include/ansi_colors.h"

#define MAX_BUF 1024
#define SHELL_EXE "minish"
#define SHELL_FULLPATH "../../minish"

/*
 * Run command in our shell and return output as a string.
 */
char *run_my_shell(const char *cmd)
{
    int outpipefds[2];
    int inpipefds[2];
    pid_t pid;
    char *result = malloc(MAX_BUF);
    char buffer[1024];
    result[0] = '\0';

    if (pipe(outpipefds) == -1) {
        perror("Pipe");
        return NULL;
    }
    if (pipe(inpipefds) == -1) {
        perror("Pipe");
        return NULL;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return NULL;
    }

    if (pid == 0) {
        // Redirect stdout to the pipe
        close(outpipefds[0]);  // Close unused read end of the pipe
        dup2(outpipefds[1], STDOUT_FILENO);  // Redirect stdout to the pipe
        close(outpipefds[1]);

        close(inpipefds[1]);
        dup2(inpipefds[0], STDIN_FILENO);  // Redirect stdin from the pipe
        close(inpipefds[0]);

        // Prepare arguments for execvp (split the command string into arguments)
        char *args[MAX_BUF / 2];  // To store the arguments (command + arguments)
        int i = 0;
        char *token = strdup(cmd); // Tokenize by space or newline
        args[i++] = token;
        args[i] = NULL;  // Null-terminate the arguments array

    
		    // Set the environment for the child process (including LD_PRELOAD)
		char *envp[] = {
			"LD_PRELOAD=./malloc_override.so",  // Ensure the custom malloc is loaded
			NULL  // Null-terminate the environment array
		};

		// Execute the shell command in the child process
		execve(SHELL_FULLPATH, args, envp);
		perror("execve");  // This will be executed if execve fails
		exit(1);
    } else {
        // Parent process: write to the shell's stdin
        close(outpipefds[1]);
        close(inpipefds[0]);

        write(inpipefds[1], cmd, strlen(cmd));  // Send the command to the child
        close(inpipefds[1]);

        // Read the output from the pipe
        int bytesRead;
        memset(buffer, 0, sizeof(buffer));
        memset(result, 0, MAX_BUF);

        wait(NULL);  // Wait for child process to finish
        while ((bytesRead = (int)read(outpipefds[0], buffer, sizeof(buffer) - 1)) > 0) {
            strncat(result, buffer, MAX_BUF - strlen(result) - 1);  // Append the output
            memset(buffer, 0, sizeof(buffer));  // Clear the buffer
        }
        close(outpipefds[0]);
    }

    // Print the result in color
    fprintf(stderr, "%s", BLU);
    fprintf(stderr, "\tGot from minish: %s", result);
    fprintf(stderr, "%s", CRESET);

    return result;
}

int main(void)
{
    const char *input = "echo haha\n";
    char *my_shell_output = run_my_shell(input);
    
    // Free the allocated memory after use
    free(my_shell_output);

    return 0;
}

