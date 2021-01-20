#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

void *own_malloc(size_t size)
{
    void *block;
    block = sbrk(size);
    printf("address of allocated block is : %p\n",block);
    if(block == (void*)-1)
        return -1;
    return block;
}

int main()
{
    int *size;
    size = (int *)own_malloc(sizeof(size));
    if(size == NULL)
        printf("memory is not allocated\n");
    printf("base address of variable:%p\n",size);
    return 0;
}