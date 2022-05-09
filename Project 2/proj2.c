#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stddef.h>


//Have a pagesize
#define PAGESIZE 4096 //or 2048, or 4096 ...
int heapsize = 0;
bool init = false;


typedef struct chunkhead
{
    unsigned int size;
    unsigned int info;
    unsigned char *next,*prev;
}chunkhead;

chunkhead *startofheap = NULL;

void *mymalloc(unsigned int i)
{
    chunkhead *temp;
    temp = sbrk(PAGESIZE);
    
    temp->info = 1;
    temp->size = PAGESIZE;
    temp->next = sbrk(0);
    if(heapsize == 0)
    {
        temp->prev = 0;
        startofheap = temp;
    }
    else
    {
        temp->prev = (unsigned char *) temp - PAGESIZE;
    }  
    void *tptr;
    tptr = temp + sizeof(chunkhead);
    heapsize++;
    return tptr;
}

void myfree(void* address)
{
    chunkhead * temp;
    temp = (chunkhead *) address - sizeof(chunkhead);
    temp->info = 0;
    chunkhead *tnext = (chunkhead *) temp->next;
    tnext->prev = temp->prev; 
    chunkhead *tprev = (chunkhead *)temp->prev;
    tprev->next = temp->next;
    heapsize--;
}

void analyze()
{
    printf("\n--------------------------------------------------------------\n");
    if(!startofheap)
    {
        printf("no heap");
        return;
    }
    chunkhead* ch = (chunkhead*)startofheap;
    for (int no=0; ch; ch = (chunkhead*)ch->next,no++)
    {
        printf("%d | current addr: %x |", no, ch);
        printf("size: %d | ", ch->size);
        printf("info: %d | ", ch->info);
        printf("next: %x | ", ch->next);
        printf("prev: %x", ch->prev);
        printf("\n");
    }
    printf("program break on address: %x\n",sbrk(0));
}
int main()
{
    int i;
    for(i = 0; i < 20 ; i++)
    {
        void *temp = mymalloc(1000);
    }
    analyze();
    
    return 0;
}