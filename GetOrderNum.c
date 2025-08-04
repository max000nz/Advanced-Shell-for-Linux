#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void GetOrderNum(char *firm){
    
    char path[50];
    snprintf(path, sizeof(path), "Black_Friday/%s_Order", firm);
    
    DIR *dir = opendir(path);
    if (dir == NULL) {
        printf("%s ---- 0 orders, folder not exist\n", firm);
        return;
    }
    
    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        if (strstr(entry->d_name, ".txt")) {
            count++;
        }
    }

    closedir(dir);

    if (count == 1)
        printf("%s ---- 1 order\n", firm);
    else
        printf("%s ---- %d orders\n", firm, count);
}