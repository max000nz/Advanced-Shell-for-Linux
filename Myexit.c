#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

void Myexit()
{
    char *args[] = {
        "rm",
        "-rf",
        "Black_Friday",
        NULL
    };

    pid_t pid = fork();
    if (pid == 0)
    {
        execvp(args[0], args);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        printf("Goodbye\n");
    }
    exit(0);
}