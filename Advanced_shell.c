#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

void loadData(char *buffer, char *args[]); // helper
void handle_cd(char **args);//cd cant be used in execvp - needed special function
void SetupFolder(); //mkdir Black_Friday and touch camp_partic.txt

void CreateFlyer(char *firm, char *percent);
void GetFlyer(char *firm);
void MakeOrder(char *firm, char *name);
void GetOrderNum(char *firm);
void GetNumComp();
void GetSummary();
void Myexit(); //different name, because exit() cant be used

int main(void)
{
    SetupFolder();
    char *args[4];
    char buffer[50];
    while (1) //until exit used - advshell is working
    {
        printf("AdvShell>");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) == 0) continue; //if input empty - do nothing
        for (int i = 0; i < 4; i++) args[i] = NULL;
        loadData(buffer, args);
        if (strcmp(args[0], "GetNumComp") == 0){
            GetNumComp();
        }
        else if(strcmp(args[0], "exit") == 0){
            Myexit();
        }
        else if(strcmp(args[0], "GetSummary") == 0){
            GetSummary();
        }
        else if(strcmp(args[0], "cd") == 0){
            handle_cd(args);
        }
        else if(strcmp(args[0], "GetFlyer") == 0){
            GetFlyer(args[1]);
        }
        else if(strcmp(args[0], "GetOrderNum") == 0){
            GetOrderNum(args[1]);
        }
        else if(strcmp(args[0], "MakeOrder") == 0){
            if(args[2] == NULL){
                printf("Name is NULL, try again later\n");
                continue;
            }
            GetFlyer(args[1]);
            MakeOrder(args[1], args[2]);
            pid_t pid = fork();
            if (pid == 0){
                char dirName[50];
                strcpy(dirName, "Black_Friday/");
                strcat(dirName, args[1]);
                strcat(dirName, "_Order/");
                strcat(dirName, args[2]);
                strcat(dirName, ".txt");
                if (access(dirName, F_OK) != -1) {
                    execl("/bin/chmod", "chmod", "444", dirName, NULL);
                    printf("chmod failed\n");
                    exit(EXIT_FAILURE);
                }
                exit(0);
            }
            else{
                wait(NULL);
            }
        }
        else if(strcmp(args[0], "CreateFlyer") == 0){
            if(args[1] == NULL || args[2] == NULL){
                printf("Name or percent is NULL, try again later\n");
                continue;
            }
            pid_t pid = fork();
            if (pid == 0)
            {
                char dirName[50];
                strcpy(dirName, "Black_Friday/");
                strcat(dirName, args[1]);
                strcat(dirName, "_Order");      
                execl("/bin/mkdir", "mkdir", dirName, NULL);
                printf("Not supported\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                wait(NULL);
                CreateFlyer(args[1], args[2]);
            }
        }
        else if (strlen(args[0]) == 0) {
            continue;
        }
        else //here we coming with regular commands of original shell
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                execvp(args[0], args);
                printf("Not supported\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                wait(NULL);
            }
        }
    }
    return 0;
}

void loadData(char *buffer, char *args[])
{
    int i = 0;
    char *token = strtok(buffer, " ");
    while (token != NULL && i < 3)
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

void handle_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "cd: missing argument\n");
    } else if (chdir(args[1]) != 0) {
        perror("cd");
    }
}

void SetupFolder() {
    pid_t pid = fork();
    if (pid == 0) {
        char *mkdir_args[] = { "mkdir", "Black_Friday", NULL };
        execvp("mkdir", mkdir_args);
        perror("mkdir failed");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
        int fd = open("Black_Friday/camp_partic.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("open failed");
            return;
        }
        close(fd);
    }
}