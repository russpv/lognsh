#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/test.h"

#define MAX_BUF 4096

void print_string_debug(const char *str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        printf("%c", (unsigned char)str[i]);
    }
    printf("\n");
}

/*
 * Run command in Bash and return output as a string.
 */
char *run_bash(const char *cmd) {
    char buffer[1024];
    char *result = malloc(MAX_BUF);
    result[0] = '\0';

	memset(result, 0, MAX_BUF);
	memset(buffer, 0, sizeof(buffer));
    FILE *pipe = popen(cmd, "r"); // Run command in bash
    if (!pipe) return NULL;

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
		strncat(result, buffer, MAX_BUF - strlen(result) - 1);  // Append the output
		memset(buffer, 0, sizeof(buffer));
    }
    pclose(pipe);

    fprintf(stderr, "%s", CYN);
    fprintf(stderr, "\tGot from bash:%s", result);
    fprintf(stderr, "%s", CRESET);
    return result;
}

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
        perror(EMSG_PIPE);
        return NULL;
    }
    if (pipe(inpipefds) == -1) {
        perror(EMSG_PIPE);
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
        dup2(inpipefds[0], STDIN_FILENO);
        close(inpipefds[0]);

        execlp(SHELL_EXE, SHELL_EXE, cmd, (char *)NULL);
        perror("execlp");
        exit(1);
    } else {

        // Close the write end of the pipe
        close(outpipefds[1]);
        close(inpipefds[0]);

        write(inpipefds[1], cmd, strlen(cmd));
        close(inpipefds[1]);

        // Read the output from the pipe
        int bytesRead;
		memset(buffer, 0, sizeof(buffer));
		memset(result, 0, MAX_BUF);	
        wait(NULL);
        while ((bytesRead = (int)read(outpipefds[0], buffer, sizeof(buffer) - 1)) > 0) {
            strncat(result, buffer, MAX_BUF - strlen(result) - 1);  // Append the output
			memset(buffer, 0, sizeof(buffer));
        }
        close(outpipefds[0]);
    }
    fprintf(stderr, "%s", BLU);
    fprintf(stderr, "\tGot from yoursh:%s", result);
    fprintf(stderr, "%s", CRESET);
    return result;
}

