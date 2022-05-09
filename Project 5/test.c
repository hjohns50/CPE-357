#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int fd;
    int *shared;
    int par_id = atoi(argv[1]);
    int par_count = atoi(argv[2]);
    if(par_id==0)
    {
        //TODO: init the shared memory for A,B,C, ready. shm_open with C_CREAT here! then ftruncate! then mmap
        fd = shm_open("shared", O_RDWR | O_CREAT, 0777);
        ftruncate(fd, 100*sizeof(int));
        shared = (int*) mmap(NULL, 100*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        shared[0] = 69;
    }
    else
    {
        //TODO: init the shared memory for A,B,C, ready. shm_open withOUT C_CREAT here! NO ftruncate! but yes to mmap
        
        while((fd = shm_open("shared", O_RDWR, 0777)) < 0);
        shared = (int*) mmap(NULL, 100*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        sleep(2); //needed for initalizing synch
        fprintf(stderr, "I am program: %d with shared mem: %d\n", par_id, shared[0]);
    }

    fprintf(stderr, "This is program: %s with %s many programs\n", argv[1], argv[2]);
    close(fd);
    shm_unlink("shared");
    munmap(shared, 100*sizeof(int));
    return 0;
}