#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/test.h"

#define MAX_BUF 4096

void print_string_debug(const char *str) 
{
    for (size_t i = 0; str[i] != '\0'; i++) 
        printf("%c", (unsigned char)str[i]);
    printf("\n");
}

/*
 * Run command in Bash and return output as a string.
 */
char *run_bash(const char *cmd) 
{
    char buffer[1024];
    char *result = malloc(MAX_BUF);
    if (!result) return NULL;
    result[0] = '\0';
    int outpipefds[2];
    pid_t pid;

    if (pipe(outpipefds) == -1) 
    {
        perror("pipe");
        free(result);
        return NULL;
    }
    pid = fork();
    if (pid == -1) 
    {
        perror("fork");
        free(result);
        return NULL;
    }
    if (pid == 0) 
    {
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull != -1) 
        {
            dup2(devnull, STDERR_FILENO);  // Redirect stderr to /dev/null
            close(devnull);                // Close the original fd
        }
        close(outpipefds[0]);  // Close read end
        dup2(outpipefds[1], STDOUT_FILENO);
        close(outpipefds[1]);
        execlp("bash", "bash", "-c", cmd, (char *)NULL);
        perror("execlp");
        exit(1);
    }
    close(outpipefds[1]);  // Close write end
    int bytesRead;
    while ((bytesRead = (int)read(outpipefds[0], buffer, sizeof(buffer) - 1)) > 0) 
    {
        buffer[bytesRead] = '\0';
        strncat(result, buffer, MAX_BUF - strlen(result) - 1);
    }
    close(outpipefds[0]);
    wait(NULL);
    fprintf(stderr, "\tGot from bash:\t\t%s", result);
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

    if (pipe(outpipefds) == -1) 
    {
        perror(EMSG_PIPE);
        return NULL;
    }
    if (pipe(inpipefds) == -1) 
    {
        perror(EMSG_PIPE);
        return NULL;
    }
    pid = fork();
    if (pid == -1) 
    {
        perror("fork");
        return NULL;
    }
    if (pid == 0) 
    {
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull != -1) 
        {
            dup2(devnull, STDERR_FILENO);  // Redirect stderr to /dev/null
            close(devnull);                // Close the original fd
        }
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
    } 
    else 
    {
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
        while ((bytesRead = (int)read(outpipefds[0], buffer, sizeof(buffer) - 1)) > 0) 
        {
            strncat(result, buffer, MAX_BUF - strlen(result) - 1);  // Append the output
			memset(buffer, 0, sizeof(buffer));
        }
        close(outpipefds[0]);
    }
    fprintf(stderr, "%s", BLU);
    fprintf(stderr, "\tGot from yoursh:\t%s", result);
    fprintf(stderr, "%s", CRESET);
    return result;
}

/* -------------------------------------------------------------------------
 * Function Group: Globbing test utils
 * Description   : 
 * ------------------------------------------------------------------------- */

void stringset_init(StringSet *set) 
{
    set->items = NULL;
    set->size = 0;
    set->capacity = 0;
}
/*
void stringset_free(StringSet *set) 
{
    for (size_t i = 0; i < set->size; i++) free(set->items[i]);
    free(set->items);
}*/

// Add item if not present (keeps array sorted)
void stringset_add(StringSet *set, const char *item)
{
    // Binary search for item
    size_t low = 0, high = set->size;
    while (low < high) {
        size_t mid = (low + high) / 2;
        int cmp = strcmp(set->items[mid], item);
        if (cmp == 0) return; // Already in set
        else if (cmp < 0) low = mid + 1;
        else high = mid;
    }
    // Insert at position low
    if (set->size == set->capacity) {
        set->capacity = set->capacity ? set->capacity * 2 : 4;
        set->items = realloc(set->items, set->capacity * sizeof(char *));
    }
    // Move items up
    memmove(&set->items[low+1], &set->items[low], (set->size - low) * sizeof(char *));
    set->items[low] = strdup(item);
    set->size++;
}

bool stringset_equal(const StringSet *a, const StringSet *b)
 {
    if (a->size != b->size) return false;
    for (size_t i = 0; i < a->size; i++) {
        if (strcmp(a->items[i], b->items[i]) != 0) return false;
    }
    return true;
}

// Tokenize string by whitespace, add tokens to set
void stringset_from_string(StringSet *set, const char *str)
{
    char *copy = strdup(str);
    char *token = strtok(copy, " \t\r\n");
    while (token) {
        stringset_add(set, token);
        token = strtok(NULL, " \t\r\n");
    }
}
