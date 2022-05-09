#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#define _OPEN_SYS_ITOA_EXT

int main(int argc, char *argv[])
{
    double total = 0.0;
    clock_t begin = clock();
    int count = atoi(argv[2]);
    int i = 0;
    char t[20];
    //fprintf(stderr, "%s\n", argv[1]);
    for(i; i < count; i++)
    {
        sprintf(t, "%d", i);
        int pid = fork();
        if(pid == 0)
        {
            execv(argv[1], (char *[]){"test", t, argv[2], NULL});
            return 0;
        }
    }
    wait(NULL);
    //fprintf(stderr, "this is daddy program\n");
    //sleep(3);
    clock_t stop = clock();
    total += (double)(stop-begin) / CLOCKS_PER_SEC;
    printf("It took %f seconds to do it!\n", total);
    return 0;
}