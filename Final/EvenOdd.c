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

bool isSorted(int *list, int size)
{
    int i = 0;
    for(i; i < size-1; i++)
    {
        if(list[i]>list[i+1])
        {
            return false;
        }
        
    }
    return true;
}
void odd_even_sort(int *list, int size, int *proc_list,int *synch_list, int num_proc, int proc_id)
{
    
    int *sorted = (int *) mmap(NULL, num_proc*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    sorted[proc_id] = 0;
    bool done = false;
    synch_list[proc_id] = 0;
    proc_list[proc_id] = 0;
    int sec_size = size/num_proc;
    // if(proc_id == (num_proc - 1))
    // {
    //     sec_size = size - (proc_id * sec_size);
    // }
    int start = proc_id * sec_size;
    int end = start + sec_size;
    if(proc_id == (num_proc - 1))
    {
        end = size - 1;
    }
    while(! isSorted(list, size))
    {
        
        sorted[proc_id] = 1;
        int e = start;
        int o = start + 1;
        int temp;
        for(e; e < end; e += 2)
        {
            if(list[e] > list[e+1])
            {
                sorted[0] = 0;
                temp = list[e];
                list[e] = list[e+1];
                list[e+1] = temp; 
            }
        }

        int i = 0;
        proc_list[proc_id] = proc_list[proc_id] + 1;
        synch_list[proc_id] = synch_list[proc_id] + 1; 
        for(i; i < num_proc; i++)
        {
            //fprintf(stderr, "synching, %d\n", proc_id);
            int temp = proc_list[proc_id];
            while(proc_list[i] < temp);
            
        }
        for(o; o < end; o += 2)
        {
            if(list[o] > list[o+1])
            {
                sorted[0] = 0;
                temp = list[o];
                list[o] = list[o+1];
                list[o+1] = temp; 
            }
        }
        int l = 0;
        proc_list[proc_id] = proc_list[proc_id] + 1;
        synch_list[proc_id] = synch_list[proc_id] + 1; 
        for(l; l < num_proc; l++)
        {
            //fprintf(stderr, "hey, %d\n", proc_id);
            int temp = proc_list[proc_id];
            while(proc_list[l] < temp);
            
        }
    }
    munmap(sorted, sizeof(int));
}
int main(int argc, char *argv[])
{
    double total = 0.0;
    clock_t begin = clock();
    char *text = calloc(1,1), buffer[2];
    if (isatty(fileno(stdin)))
    {
        printf( "Enter the list: " );
    }
    while( fgets(buffer, 2 , stdin)) /* break with ^D or ^Z */
    {
        // if(strcmp(buffer, "\n") == 0)
        // {
        //     break;
        //     //printf("hey");
        // }
        text = realloc(text, strlen(text)+1+strlen(buffer));
        strcat(text, buffer); 
        //printf("%s\n", buffer);
        if(strcmp(buffer, "\n") == 0)
        {
            break;
            //printf("hey");
        }
    }
    //printf("%lu", strlen(text));
    //printf("\ntext: %s", text);
    int *arr;
    arr = (int *) mmap(NULL, ((strlen(text)+1)/2) * sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    int p = 0;
    char delim[2] = " ";
    char *modified = strtok(text, delim);
    while(modified != NULL)
    {
        //printf("%s\n", modified);
        arr[p] = atoi(modified);
        p++;
        modified = strtok(NULL, delim);
    }
    
    int i = 0;
    printf("Original list: [");
    for(i; i < p; i++)
    {
        if(i == p-1)
        {
            printf("%d]\n", arr[p-1]);
            break;
        }
        printf("%d, ", arr[i]);
    }
    int num_proc = atoi(argv[1]);
    int *proccess_list = (int *) mmap(NULL, num_proc * sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    int *synch_list = (int *) mmap(NULL, num_proc * sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    int *pid_list = (int *) mmap(NULL, num_proc * sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    int status = 0;
    int proc_id;
    //int status;
    if((p/num_proc) < 2)
    {
        printf("too many proccesses for size of array\n");
        return -1;
    }
    int f = 0;
    
    for(f; f < num_proc; f++)
    {
        int pid = fork();
        if(pid == 0)
        {
            synch_list[f] = 0;
            proccess_list[f] = 0;
            pid_list[f] = pid;
            proc_id = f;
            odd_even_sort(arr, p, proccess_list, synch_list, num_proc, proc_id);
            //kill(pid, SIGKILL);
            exit(0);
        }
    }
    wait(NULL);
    clock_t stop = clock();
    total += (double)(stop-begin) / CLOCKS_PER_SEC;
    //sleep(1);
    int w = 0;
    // for(w; w < num_proc; w++)
    // {
    //     waitpid(pid_list[w], &status, WNOHANG);
    // }
    //wait(0);
    printf("Sorted list: [");
    for(i = 0; i < p; i++)
    {
        if(i == p-1)
        {
            printf("%d]\n", arr[p-1]);
            break;
        }
        printf("%d, ", arr[i]);
    }
    total = total * 1000;
    printf("Process: %d\n", num_proc);
    printf("Time to sort: %f ms\n", total);
    // for(w; w < num_proc; w++)
    // {
    //     kill(pid_list[w], SIGKILL);
    // }
    // munmap(arr, ((strlen(text)+1)/2) * sizeof(int));
    // munmap(proccess_list, num_proc*sizeof(int));
    // munmap(pid_list, num_proc*sizeof(int));
    // free(text);
    return 1;
}