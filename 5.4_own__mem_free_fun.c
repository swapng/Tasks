#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t glob_mutex_lock;

struct mem_struct_block_t{
    size_t block_sz;
    unsigned int is_free;
    struct mem_struct_block_t *next;
};

typedef struct mem_struct_block_t header_t;
header_t *head = NULL;
header_t *last = NULL;

header_t *get_free_mem_block(size_t sz)
{
    header_t *trav = head;
    while(trav !=NULL)
    {
        if(trav->block_sz && trav->is_free >=sz)
            return trav;
        trav = trav->next;
    }
    return NULL;
}

void own_free(void *firstbyte)
{
    header_t *memory_cntrl_block;
    memory_cntrl_block = firstbyte - sizeof(header_t);      /* Backup from the given pointer to find the mem_block_struct_t */
    memory_cntrl_block->is_free = 1;                        /* marking the block as available */
    printf("Dynamically allocated memory is freed\n");
}

void *own_malloc_fun(size_t block_sz)
{
    header_t *tmp_head;
    size_t tot_size;
    void *block = NULL;
    if(!block_sz)
        return NULL;
    
    pthread_mutex_lock(&glob_mutex_lock);
    tmp_head = get_free_mem_block(block_sz); 
    if(tmp_head) {
        tmp_head->is_free = 0;
        pthread_mutex_unlock(&glob_mutex_lock);
        return (void *)(tmp_head+1);
    }
    tot_size = sizeof(header_t)+block_sz;
    block = sbrk(tot_size);
    //printf("address of allocated block is : %p\n",block);
    //own_free(&block);
    if(block == (void*)-1) {
        pthread_mutex_unlock(&glob_mutex_lock);
        return (void *)-1;
    }
    tmp_head = block;
    tmp_head->block_sz = block_sz;
    tmp_head->is_free = 0;
    tmp_head->next = NULL;

    if(!head)
        head = tmp_head;
    if(last)
        last->next = tmp_head;
    last = tmp_head;
    pthread_mutex_unlock(&glob_mutex_lock);
    return (void*)(head+1);
}

void main()
{
    int *size = NULL;
   
    size = own_malloc_fun(sizeof(size));
    if(size == NULL){
        printf("memory is not allocated\n");
        exit(1);
    }
    printf("allocated block of memory address is : %p\n",size);
    own_free(&size);
    exit(0);
}