#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

void GetFlyer(char *firm)
{
    char filename[50];
    snprintf(filename, sizeof(filename), "Black_Friday/%s.txt", firm);
    
    int fd = open(filename, O_RDONLY, 0644);
    if (fd == -1)
    {
        printf("Company Not Found!\n");
        return;
    }
    
    char buf[512];
    ssize_t bytesRead = read(fd, buf, 512);
    if (bytesRead > 0) {
        buf[bytesRead] = '\0';
        printf("%s\n", buf);
    }
    close(fd);
}