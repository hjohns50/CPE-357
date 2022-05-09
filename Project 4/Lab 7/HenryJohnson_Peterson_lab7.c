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


int main()
{
    char *text;
    text = (char *) mmap(NULL, 1000, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    int *flag;
    flag = (int *) mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    flag[0] = 1;
    int *p1;
    int *p2;
    p1 = (int *) mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    p2 = (int *) mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    int pid = fork();

    if(pid == 0)
    {
        char str1[] = "ssssssssss ssssssssss ssssssssss ssssssssss ssssssssss ssssssssss ssssssssss ssssssssss ssssssssss ssssssssss ssssssssss ssssssssss\n";
        char str2[] = "tttttttttt tttttttttt tttttttttt tttttttttt tttttttttt tttttttttt tttttttttt tttttttttt tttttttttt tttttttttt tttttttttt tttttttttt\n";
        for(int i = 0; ; i++)
        {
            p2[0] = 1;
            while(flag[0] == 0 && p1[0] == 1);
            if(i%2 == 0)
            {
                strcpy(text, str1);
            }
            else
            {
                strcpy(text, str2);
            }
            flag[0] = 0;
            p2[0] = 0;
        }
        return 0;
    }
    else
    {
        char res[1000];
        while(true)
        {
            p1[0] = 1;
            while(flag[0] == 1 && p2[0] == 1);
            strcpy(res, text);
            printf("%s", res);
            flag[0] = 1;
            p1[0] = 0;
        }
    }

    return 0;
}