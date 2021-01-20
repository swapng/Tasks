#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

pthread_mutex_t mutex_malloc_lock;
typedef char mem_align[8];  /* array is to maintain 8 bytes of memory block */

union mem_header{

    struct{
        size_t block_size;
        unsigned int is_free;
        union mem_header *next;
    }mem_struct_t;

    mem_align var_block;
};

typedef union mem_header header_t;
header_t *head = NULL;
header_t *last = NULL;

/*Below function to traverses the linked list and check if there is any already exist a block of memory 
and that is marked as free and can accomodate the given size */

header_t *get_free_mem_block(size_t size)
{
    header_t *trav = head;
    while(trav)
    {
        if(trav->mem_struct_t.block_size && trav->mem_struct_t.is_free >= size)
            return trav;
        trav = trav->mem_struct_t.next;
    }
    return NULL;
}

/* Below function is to allocate requested size of memory to the variable*/

void *own_malloc(size_t block_sz)
{
    header_t *header = NULL;
    size_t tot_size;
    void *block = NULL;
    
    if(!block_sz)                                       /* If requested size is 0 then return from here itself */
        return NULL;

    pthread_mutex_lock(&mutex_malloc_lock);
    header = get_free_mem_block(block_sz);
    if(header)                                          /* If a sufficiently large free block is found,marking that block as not-free 
                                                            and then return a pointer to that block*/
    {
        header->mem_struct_t.is_free=0;
        pthread_mutex_unlock(&mutex_malloc_lock);
        return (void*)(header+1);                       /* Returning first byte of next memory block */
    }

    tot_size = sizeof(header_t) + block_sz;             /* If not found a sufficiently large free block, then extending the heap by calling sbrk() and  
                                                            The heap has to be extended by a size that fits the requested size as well a header 
                                                            so that firstly adding header and requested size */
    block = sbrk(tot_size);
    //printf("address of allocated block is : %p\n",block);
    if(block == (void *)-1)
    {
        pthread_mutex_unlock(&mutex_malloc_lock);
        return (void *)-1;
    }
    
    header = block;         
    header->mem_struct_t.block_size = block_sz;         /* filling header with requested size */
    header->mem_struct_t.is_free = 0;                   /* marking that block as not-free  */
    header->mem_struct_t.next = NULL;                   
    if(!head)                                           /* updating head and last to reflect new node in the linked list */
        head = header;
    if(last)
        last->mem_struct_t.next = header;
    last = header;
    pthread_mutex_unlock(&mutex_malloc_lock);

    return (void*)(header+1);
}

/*Function to request no. of bytes of menory to allocate dynamically */

int main()
{
    double *size = NULL;
    size = (double *)own_malloc(sizeof(size));
    if(size == NULL)
    {
        printf("Unable to allocate memory\n");
        exit(1);
    }
    printf("allocated block of memory address is : %p\n",size);
    return 0;
}