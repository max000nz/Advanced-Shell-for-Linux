#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

void MakeOrder(char *firm, char *name){
char *names[100];
char *prices[100];
int count = 0;

char file[50];
snprintf(file, sizeof(file), "Black_Friday/%s.txt", firm);

int fd = open(file, O_RDONLY);
if (fd == -1) {
    //perror("Company Not Found");
    return;
}

char c, line[256];
int pos = 0, lineNum = 0, percent = 0;
    while (read(fd, &c, 1) == 1) {
    if (c == '\n') {
        line[pos] = '\0';
        pos = 0;
        lineNum++;

        if (lineNum == 2) {
            sscanf(line, "%d", &percent);
        }
        else if (lineNum > 2) {
            char itemName[100] = {0};
            char price[20] = {0};

            char *dotPos = strstr(line, "...");
            if (!dotPos) continue;

            int nameLen = dotPos - line;
            strncpy(itemName, line, nameLen);
            itemName[nameLen] = '\0';

            while (nameLen > 0 && itemName[nameLen - 1] == ' ') {
                itemName[--nameLen] = '\0';
            }

            char *priceStart = dotPos;
            while (*priceStart && !isdigit(*priceStart)) priceStart++;

            strncpy(price, priceStart, sizeof(price) - 1);
            price[sizeof(price) - 1] = '\0';

            names[count] = strdup(itemName);
            prices[count] = strdup(price);
            count++;
        }
    } else {
        line[pos++] = c;
    }
    }
    close(fd);

    if (count == 0) {
        printf("No available items for this company.\n");
        return;
    }

    char filename[100];
    snprintf(filename, sizeof(filename), "Black_Friday/%s_Order/%s.txt", firm, name);
    int out = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out == -1) {
        //perror(filename);
        return;
    }

    char buf[100], item[30], qty[10];
    double summary = 0;
    snprintf(buf, sizeof(buf), "%s Order\n", firm);
    write(out, buf, strlen(buf));
    printf("Insert your order (Stop to finish):\n");
    while (1) {
        printf("Insert item name: ");
        fgets(item, sizeof(item), stdin);
        item[strcspn(item, "\n")] = 0;
        if (strcasecmp(item, "stop") == 0) break;
        int found = 0;
            for (int i = 0; i < count; i++) {
                if (strcmp(item, names[i]) == 0) {
                    found = 1;
                    break;
                }
            }
            while(!found){ 
                printf("Invalid item name, try again\n");
                printf("Insert valid item name: ");
                fgets(item, sizeof(item), stdin);
                item[strcspn(item, "\n")] = 0;
                if (strcasecmp(item, "stop") == 0){
                    found = 1;
                    break;
                }
                
                for (int i = 0; i < count; i++) {
                    if (strcmp(item, names[i]) == 0) {
                        found = 1;
                        break;
                    }
                }
            
            }
        printf("Insert item quantity: ");
        fgets(qty, sizeof(qty), stdin);
        qty[strcspn(qty, "\n")] = 0;
        if (strcasecmp(qty, "stop") == 0) break;

        for (int i = 0; i < count; i++) {
            if (strcmp(item, names[i]) == 0) {
                summary += atoi(prices[i]) * atoi(qty);
            }
        }

        int pos = 0;
        for (int i = 0; item[i]; ++i) buf[pos++] = item[i];
        buf[pos++] = ' '; buf[pos++] = '-'; buf[pos++] = ' ';
        for (int i = 0; qty[i]; ++i) buf[pos++] = qty[i];
        buf[pos++] = '\n';
        write(out, buf, pos);
    }
    summary *= (1.0 - percent / 100.0);
    printf("Total price: %g NIS (Confirm to approve / else cancel)\n", summary);

    char confirm[20];
    fgets(confirm, sizeof(confirm), stdin);
    confirm[strcspn(confirm, "\n")] = 0;

    if (strcasecmp(confirm, "confirm") == 0) {
        char total[100];
        snprintf(total, sizeof(total), "Total price: %g NIS\n", summary);
        write(out, total, strlen(total));
        printf("Order created!\n");
    } else {
        close(out);
        unlink(filename);
        printf("Order canceled.\n");
        return;
    }

    close(out);
}