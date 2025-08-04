#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void GetNumComp();
void GetOrderNum(char *firm);

void GetSummary(){
    GetNumComp();

    int fd = open("Black_Friday/camp_partic.txt", O_RDONLY);
    if (fd == -1) {
        perror("open failed");
        return;
    }

    char buf[1024];
    int bytes = read(fd, buf, sizeof(buf) - 1);
    close(fd);

    if (bytes <= 0) return;
    buf[bytes] = '\0';

    char *line = strtok(buf, "\n");
    while (line) {
        if (strlen(line) > 0) {
            GetOrderNum(line); // твоя функция, которая считает заказы для line
        }
        line = strtok(NULL, "\n");
    }
}