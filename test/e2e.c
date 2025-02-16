#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/test.h"

/**
 * Run command in Bash and return output as a string.
 */
char *run_bash(const char *cmd) {
    char buffer[1024];
    char *result = malloc(4096);
    result[0] = '\0';

    FILE *pipe = popen(cmd, "r"); // Run command in bash
    if (!pipe) return NULL;

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        strcat(result, buffer);
    }
    pclose(pipe);
    fprintf(stderr, "%s", CYN);
    fprintf(stderr, "Got from bash:%s", buffer);
    fprintf(stderr, "%s", CRESET);
    return result;
}

/**
 * Run command in our shell and return output as a string.
 */
#define MAX_BUF 4096

char *run_my_shell(const char *cmd) 
{
    int outpipefds[2];
    int inpipefds[2];
    pid_t pid;
    char *result = malloc(MAX_BUF);
    char buffer[1024];
    result[0] = '\0';

    // Create a pipe
    if (pipe(outpipefds) == -1) {
        perror(ERRMSG_PIPE);
        return NULL;
    }
    if (pipe(inpipefds) == -1) {
        perror(ERRMSG_PIPE);
        return NULL;
    }
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return NULL;
    }
    if (pid == 0) {
        // In the child process

        // Redirect stdout to the pipe
        close(outpipefds[0]);  // Close unused read end of the pipe
        dup2(outpipefds[1], STDOUT_FILENO);  // Redirect stdout to the pipe
        close(outpipefds[1]);

        close(inpipefds[1]);
        dup2(inpipefds[0], STDIN_FILENO);
        close(inpipefds[0]);

        // Execute the shell command
        execlp(SHELL_EXE, SHELL_EXE, cmd, (char *)NULL);
        // If execlp fails
        perror("execlp");
        exit(1);
    } else {
        // In the parent process

        // Close the write end of the pipe
        close(outpipefds[1]);
        close(inpipefds[0]);

        write(inpipefds[1], cmd, strlen(cmd));
        //write(inpipefds[1], "\n", 1);  // Ensure newline is included
        close(inpipefds[1]);

        // Read the output from the pipe
        int bytesRead;
        while ((bytesRead = (int)read(outpipefds[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            strcat(result, buffer);  // Append the output
        }

        // Wait for the child process to finish
        wait(NULL);

        close(outpipefds[0]);

    }
    fprintf(stderr, "%s", BLU);
    printf("Got from minish:%s", result);
    fprintf(stderr, "%s", CRESET);
    return result;
}

