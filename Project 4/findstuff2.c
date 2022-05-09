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
#include <math.h>
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void find_word(char str[])
{
    sleep(7);
    printf("Sorry have not completed this part of the assignment yet\n");
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
    char res[4000];
    while ((dir = readdir(d)) != NULL) 
    {
        int test = strcmp(dir->d_name, str);
        if(test == 0)
        {
            //printf("%s\n", dir->d_name);
            char temp[1000];
            getcwd(temp, 1000);
            strcpy(res, temp);
            found = true;
        }
    }   
    closedir(d);
    if(!found)
    {
        char unsucc[] = ">nothing found<\n";
        printf(">nothing found<\n");
        strcpy(res, unsucc);
    }
    clock_t end = clock();
    tot_time += (double)(end-start) / CLOCKS_PER_SEC;
    char seconds[10];
    //ftoa(tot_time, seconds, 8);
    char timed[] = "\ntime elaspsed in seconds: ";
    strcat(res, timed);
    strcat(res, seconds);
    printf("%s", res);
    return;
}

int main()
{
    int pid_lst[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
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
        if(strcmp(command, "q") == 0 || strcmp(command, "quit") == 0)
        {
            int q = 0;
            for(q; q < 10; q++)
            {
                //printf("%d\n", pid_lst[q]);
                if(pid_lst[q] != 0)
                {
                    //printf("%d", pid_lst[q]);
                    kill(pid_lst[q], SIGKILL);
                    pid_lst[q] = 0;
                }
            }
            return 1;
        }
        if(strcmp(command, "list") == 0)
        {
            int r = 0;
            for(r; r < 10; r++)
            {
                if(pid_lst[r] != 0)
                {
                    printf("child process %d PID: %d\n", r, pid_lst[r]);
                }
            }
            continue;
        }
        if(strcmp(command, "kill") == 0)
        {
            int k = atoi(entry);
            kill(pid_lst[k-1], SIGKILL);
            pid_lst[k-1] = 0;
            continue;
        }
        int pid = fork();
        if(pid == 0)
        {
            if(quote)
            {
                find_word(entry);
                return 1;
            }
            else
            {
                double total = 0.0;
                clock_t begin = clock();
                find_file(entry);
                clock_t stop = clock();
                total += (double)(stop-begin) / CLOCKS_PER_SEC;
                printf("%f", total);
                return 1;
            }
        }
        else
        {
            int p = 0;
            for(p; p < 10; p++)
            {
                if(pid_lst[p] == 0)
                {
                    pid_lst[p] = pid;
                    //printf("%d, %d\n", pid_lst[p], pid);
                    break;
                }
            }
        }
        //printf("%s, %s, %s, %s\n", command, entry, flag1, flag2);
    }
    return 1;
}