#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int is_line_empty(const char *line) {
    while (*line) {
        if (!isspace(*line)) return 0;
        line++;
    }
    return 1;
}

void GetNumComp()
{
    int fd = open("Black_Friday/camp_partic.txt", O_RDONLY);
    if (fd == -1) {
        perror("open failed");
        return;
    }

    char buf[1024];
    ssize_t bytes_read;
    int count = 0;
    char line[1024];
    int pos = 0;

    while ((bytes_read = read(fd, buf, sizeof(buf))) > 0) {
        for (ssize_t i = 0; i < bytes_read; ++i) {
            if (buf[i] == '\n') {
                line[pos] = '\0';
                if (!is_line_empty(line)) {
                    count++;
                }
                pos = 0;
            } else {
                if (pos < sizeof(line) - 1) {
                    line[pos++] = buf[i];
                }
            }
        }
    }

    if (pos > 0) {
        line[pos] = '\0';
        if (!is_line_empty(line)) {
            count++;
        }
    }

    close(fd);
    if(count == 1) printf("%d company take part in the Campaign\n", count);
    else printf("%d companies take part in the Campaign\n", count);
}
   