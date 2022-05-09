#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <sys/sysmacros.h>
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void stayalivec(int c)
{

    printf("\nhahah I'm still alive\n");
    child();
}

void stayalive(int c)
{
    sleep(1);
    return;
}

int child()
{
    signal(SIGINT, stayalivec);
    signal(SIGQUIT, stayalivec);
    signal(SIGTSTP, stayalivec);
    signal(SIGTERM, stayalivec);
    signal(SIGKILL, stayalivec);
    while(true)
    {
        char input[20];
        struct stat sb;
        printf(ANSI_COLOR_BLUE "my supergreat fancy program assignment three");
        printf(ANSI_COLOR_RESET "$ ");
        scanf("%s", input);
        int test = strcmp(input, "list");
        if(test==0)
        {   
            DIR *d;
            struct dirent *dir;
            d = opendir(".");
            while ((dir = readdir(d)) != NULL) 
            {
                printf("%s\n", dir->d_name);
            }   
            closedir(d);
            continue;
        }
        test = strcmp(input, "q");
        if(test == 0)
        {
            return 1;
        }
        else
        {   
            int succ = stat(input, &sb);
            if(succ != 0)
            {
                perror(__FUNCTION__);
            }
            
                printf("File type:                ");
                switch (sb.st_mode & S_IFMT) {
                case S_IFBLK:  printf("block device\n");            break;
                case S_IFCHR:  printf("character device\n");        break;
                case S_IFDIR:  printf("directory\n");               break;
                case S_IFIFO:  printf("FIFO/pipe\n");               break;
                case S_IFLNK:  printf("symlink\n");                 break;
                case S_IFREG:  printf("regular file\n");            break;
                case S_IFSOCK: printf("socket\n");                  break;
                default:       printf("unknown?\n");                break;
                }

                printf("I-node number:            %ju\n", (long) sb.st_ino);

                printf("Mode:                     %jo (octal)\n", (unsigned long) sb.st_mode);

                printf("Link count:               %ju\n", (long) sb.st_nlink);
                printf("Ownership:                UID=%ju   GID=%ju\n", (long)sb.st_uid, (long) sb.st_gid);

                printf("Preferred I/O block size: %jd bytes\n", (long) sb.st_blksize);
                printf("File size:                %jd bytes\n", (long) sb.st_size);
                printf("Blocks allocated:         %jd\n", (long) sb.st_blocks);

                printf("Last status change:       %s", ctime(&sb.st_ctime));
                printf("Last file access:         %s", ctime(&sb.st_atime));
                printf("Last file modification:   %s", ctime(&sb.st_mtime));
        }
    }
    
}

void parent()
{
    signal(SIGINT, stayalive);
    signal(SIGQUIT, stayalive);
    signal(SIGTSTP, stayalive);
    signal(SIGTERM, stayalive);
    signal(SIGKILL, stayalive);
    while(true)
    {
        wait(NULL);
        sleep(10);
        int pid = fork();
        if(pid == 0)
        {
            child();
        }
    }
}

int main()
{
    int pid = fork();
    if(pid == 0)
    {
       int r = child();
    }
    else
    {
        parent();
        return 1;
    }
    return 0;
}