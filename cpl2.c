#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#define HEAPSIZE 10000
char Heap[HEAPSIZE];

typedef struct MetaData_Tag
{
    int size;
    char status;//'f for free ';a for allocated//
    struct MetaData_Tag *next;
} MetaData;

MetaData *heap_ptr = (void *)Heap;

void Initialize()
{
    heap_ptr->size = (HEAPSIZE - (sizeof(MetaData)));
    printf("Heap size is %d bytes \n", heap_ptr->size);
    printf("Size of Meta Data structure is %d bytes \n", sizeof(MetaData));
    heap_ptr->status = 'f';
    heap_ptr->next = NULL;
    printf("Address of starting of the heap: %d \n", 0);
    printf("Ending address of the heep is: %d\n", HEAPSIZE);
    printf("Heap block is initialized \n\n");
}

void *Allocate(int size_requested)
{
    MetaData *trail, *lead;
    void *ret_ptr;
    trail = lead = ret_ptr = (void *)Heap;

    int startingIndex = 0; // Initialize starting index

    while ((lead->size < size_requested) || ((lead->status == 'a') && (lead != NULL)))
    {
        trail = lead;
        lead = lead->next;
        startingIndex =startingIndex+ (trail->size + sizeof(MetaData)); // Updating starting index
    }

    if ((lead->next == NULL) && (lead->size < size_requested))
    {
        printf("No free space left to allocate %d bytes of memory \n", size_requested);
        ret_ptr = NULL;
    }
    else
    {
        if (lead->size == size_requested)
        {
            lead->status = 'a';
            printf("Allocated %d bytes of memory at Index %d - %d \n", size_requested, startingIndex, startingIndex + size_requested - 1);
            ret_ptr = lead;
        }
        else
        {
            MetaData *new_block = (void *)(((void *)lead) + sizeof(MetaData) + size_requested);
            new_block->size = ((lead->size) - size_requested - sizeof(MetaData));
            new_block->next = lead->next;
            lead->size = size_requested;
            printf("Allocated %d bytes of memory at Index %d - %d \n", size_requested, startingIndex, startingIndex + size_requested - 1);
            lead->next = new_block;
            lead->status = 'a';
            ret_ptr = new_block;
        }
    }
    return ret_ptr;
}

void Merge()
{
    MetaData *merge_ptr, *temp;
    merge_ptr = heap_ptr;

    while (merge_ptr->next != NULL)
    {
        if ((merge_ptr->status == 'f') && ((merge_ptr->next)->status == 'f'))
        {
            merge_ptr->size += (merge_ptr->next->size) + sizeof(MetaData);
            merge_ptr->next = merge_ptr->next->next;
            printf(" We Merged the block \n");
        }
        else
        {
            temp = merge_ptr;
            merge_ptr = merge_ptr->next;
        }
    }
    printf("Merge function is done \n\n");
}

void Free(void *free_ptr)
{
    if ((((void *)Heap) <= (free_ptr)) && (((void *)(Heap + 10000)) >= (free_ptr)))
    {
        MetaData *ptr = (void *)Heap;
        int index = 0; // Initializing index

        while (ptr->next != free_ptr)
        {
            index += (ptr->size + sizeof(MetaData)); // Updating index
            ptr = ptr->next;
        }

        printf("Freed location at Index %d \n", index);
        ptr->status = 'f';
        printf("Freed the block \n");
        Merge();
    }
    else
    {
        printf("Entered block of address to be freed is invalid \n");
    }
}

void DisplayHeap()
{
    MetaData *traverse_ptr = (void *)Heap;
    int index = 0;

    while (traverse_ptr != NULL)
    {
        printf("Index %d - %d  ", index, index + traverse_ptr->size - 1);
        printf("%c\n", traverse_ptr->status);

        index += traverse_ptr->size;
        traverse_ptr = traverse_ptr->next;
    }
    printf("\n\n");
}

int main()
{ 
    Initialize();
    int *memory1 = (int *)Allocate(150 * sizeof(int));
    DisplayHeap();
    unsigned long long int *memory2 = (unsigned long long int *)Allocate(150 * sizeof(unsigned long long int));
    DisplayHeap();
    int *memory3 = (int *)Allocate(100 * sizeof(int));
    DisplayHeap();

    Free(memory2);
    printf("Heap memory status after freeing memoery2 is as follows: \n");
    DisplayHeap();

    Free(memory3);
    printf("Heap memory status after freeing memoery3 is as follows: \n");
    DisplayHeap();

    Free(memory1);
    printf("Heap memory status after freeing memoery1 is as follows: \n");
    DisplayHeap();

    return 0;
}