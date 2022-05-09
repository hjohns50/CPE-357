#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
float count;
void catcher(int msg)
{
    int i = 0;
    for(i; i < count; i++)
    {
        fprintf(stderr, ".");
    }
    //count --;
    if(count > 0)
    {
        fprintf(stderr, "\n");
    }
}
int main(int argc, char *argv[])
{
    bool cmd_arg = false;
    if(argc > 3)
    {
        cmd_arg = true;
    }
    count = 2.0*(atof(argv[1]));
    struct itimerval it_val;
    signal(SIGALRM, catcher);
    it_val.it_value.tv_sec = .5;
    it_val.it_value.tv_usec =    (500*1000) % 1000000;	
    it_val.it_interval = it_val.it_value;
    setitimer(ITIMER_REAL, &it_val, NULL);
    while(1)
    {
        pause();
        count--;
        if(count == 0)
        {
            int pid = fork();
            if(pid == 0)
            {
                
                execv(argv[2], (char *[]){argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], NULL});
                exit(0);
            }
            else
            {
                wait(NULL);
                return 1;
            }
        }
    }
    return 1;
}

