#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void find_word(char str[])
{
    printf("Sorry have not completed this part of the assignment yet");
    printf("%s \n", str);
    return;
}
void find_file(char str[])
{
    double tot_time = 0.0;
    clock_t start = clock();
    //printf("%s \n", str);
    struct dirent *dp;
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    bool found = false;
    while ((dir = readdir(d)) != NULL) 
    {
        int test = strcmp(dir->d_name, str);
        if(test == 0)
        {
            //printf("%s\n", dir->d_name);
            char temp[1000];
            getcwd(temp, 1000);
            printf("%s\n", temp);
            found = true;
        }
    }   
    closedir(d);
    if(!found)
    {
        printf(">nothing found<\n");
    }
    clock_t end = clock();
    tot_time += (double)(end-start) / CLOCKS_PER_SEC;
    printf("Time elaspsed: %f seconds\n", tot_time);
    return;
}

int main()
{
    while(true)
    {
        char entry[20];
        char command[20];
        char flag1[20];
        char flag2[20];
        bool b_lst[] = {false, false, false, false};
        char raw[100];
        printf(ANSI_COLOR_BLUE "findstuff");
        printf(ANSI_COLOR_RESET "$ ");
        fgets(raw, 100, stdin);
        //printf("%s", raw);
        const char s[2] = " ";
        char *token;
        token = strtok(raw, s);
        int b = 0;
        while( token != NULL ) 
        {
            if(b == 0)
            {
                strcpy(command, token);
                b++;
                b_lst[0] = true;
                token = strtok(NULL, s);
                //printf("%s\n", command);
                continue;
            }
            if(b == 1)
            {
                strcpy(entry, token);
                b++;
                b_lst[1] = true;
                token = strtok(NULL, s);
                //printf("%s\n", entry);
                continue;
            }
            if(b == 2)
            {
                
                strcpy(flag1, token);
                b++;
                b_lst[2] = true;
                token = strtok(NULL, s);
                //printf("%s\n", flag1);
                continue;
            }
            if(b == 3)
            {
                strcpy(flag2, token);
                b++;
                b_lst[3] = true;
                token = strtok(NULL, s);
                //printf("%s\n", flag2);
                continue;
            }
            if(b > 3)
            {
                printf("too many arguments\n");
            }
        }
        int n = 0;
        bool quote = false;
        for(n; n < 20; n++)
        {
            int brief = entry[n];
            if(brief == 34)
            {
                quote = true;
                break;
            }
        }
        command[strcspn(command, "\n")] = 0;
        entry[strcspn(entry, "\n")] = 0;
        flag1[strcspn(flag1, "\n")] = 0;
        flag2[strcspn(flag2, "\n")] = 0;
        //printf("%s, %s, %s, %s\n", command, entry, flag1, flag2);
        if(quote)
        {
            find_word(entry);
        }
        else
        {
            find_file(entry);
        }
        //printf("%s, %s, %s, %s\n", command, entry, flag1, flag2);
    }
    return 1;
}