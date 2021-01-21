#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

struct mem_block_struct_t {
    int is_free;
    int size;
};

void own_free_fun(void *firstbyte) 
{
    struct mem_block_struct_t *mcb;
    mcb = firstbyte - sizeof(struct mem_block_struct_t);     /* Backup from the given pointer to find the mem_block_struct_t */
    mcb->is_free = 1;                                        /* marking the block as available */

    printf("Allocated memory is successfully freed\n");
}

void *own_malloc_func(size_t block_size)
{
    void *block = NULL;
    block = sbrk(block_size);
    printf("address of allocated memory block: %p\n",block);
    if(block == (void*)-1)
    {
        printf("Unable to allocate memory\n");
        return (void *)-1;
    }
    return block;
}

int main()
{
    char *size = NULL;
    void *ptr = NULL;
    ptr = own_malloc_func(sizeof(size));
    if(ptr == NULL)
        exit(1);
    printf("base address of variable: %p\n",ptr);
    own_free_fun(&ptr);
    return 0;
}