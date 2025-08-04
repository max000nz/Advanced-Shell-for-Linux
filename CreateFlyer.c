#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

void CreateFlyer(char *firm, char *percent){
    // write to camp_partic.txt
    int camp = open("Black_Friday/camp_partic.txt", O_WRONLY | O_APPEND, 0644);
    if (camp == -1)
    {
        printf("camp_partic.txt doesn't exist, creating new\n");
        camp = open("Black_Friday/camp_partic.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    }

    write(camp, firm, strlen(firm));
    write(camp, "\n\n", strlen("\n\n"));
    close(camp);
    // camp_partic changed

    char filename[50];
    snprintf(filename, sizeof(filename), "Black_Friday/%s.txt", firm);

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror(filename);
        return;
    }

    char buf[100];
    char name[30];
    char price[10];

    snprintf(buf, sizeof(buf), "%s Sale\n", firm);
    write(fd, buf, strlen(buf));

    snprintf(buf, sizeof(buf), "%s%% off!\n", percent);
    write(fd, buf, strlen(buf));

    printf("Insert item and item cost(type Stop if you want to quit)\n");
    while (1)
    {
        printf("Insert item name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;
        if (strcmp(name, "Stop") == 0 || strcmp(name, "stop") == 0)
            break;
        printf("Insert item price: ");
        fgets(price, sizeof(price), stdin);
        price[strcspn(price, "\n")] = 0;
        if (strcmp(price, "Stop") == 0 || strcmp(price, "stop") == 0)
            break;
        int pos = 0;
        for (int i = 0; name[i] != '\0'; ++i)
            buf[pos++] = name[i];

        int total_width = 40;
        int name_len = strlen(name);
        for (int i = 0; i < total_width - name_len; ++i)
            buf[pos++] = '.';


        buf[pos++] = ' ';

        for (int i = 0; price[i] != '\0'; ++i)
            buf[pos++] = price[i];

        buf[pos++] = 'N';
        buf[pos++] = 'I';
        buf[pos++] = 'S';

        buf[pos++] = '\n';

        // write
        write(fd, buf, pos);
    }
    const char *arr[] = {
        "Enjoy!",
        "Shop now and have fun!",
        "No buy - you bad guy"};

    char choice[50];
    int choi;
    int num = 1;
    char end[50];
    
    printf("What is your choice?\n");
    for (int k = 0; k < 3; k++)
        {
            printf("%d. %s\n", num++, arr[k]);
        }
    printf("4. Other\n");
    scanf("%d", &choi);
    getchar();
    if(choi == 1) write(fd, "Enjoy!", strlen("Enjoy!"));
    else if(choi == 2) write(fd, "Shop now and have fun!", strlen("Shop now and have fun!"));
    else if(choi == 3) write(fd, "No buy - you bad guy", strlen("No buy - you bad guy"));
    else if (choi == 4)
    {
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0;
        write(fd, choice, strlen(choice));
    }
    close(fd);
}