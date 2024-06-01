# HEAP_MANAGEMENT_SCHEME
This is the project of implementation of heap management scheme. Implementing the 'allocate', and 'free' functions ensure that adjacent free blocks are merged together using first fit algorithm.
#include <stddef.h>
stddef.h is required because the definition for size_t datatype is found there.
#define HEAPSIZE 10000
char Heap[HEAPSIZE];
This is the declaration of the array which is considered as our memory. We get a contiguous allocation of memory by using an array. Here, I assumed a block of memory of size 10,000 bytes char heap[10000]; (assuming that the storage for a character is 1 byte in the machine) and all the data structures and the allocated memory blocks reside in this same chunk of memory.
typedef struct MetaData_Tag
{
    int size;
    char status;//'f for free ';a for allocated//
    struct MetaData_Tag *next;
} MetaData;

MetaData *heap_ptr = (void *)Heap;
Here we initialize a pointer of type "MetaData", named heap_ptr pointing to the starting address of the chunk of memory we created before. This freeList pointer will point to the start of the linked list of metadata blocks.The starting address of the array (memory) should be casted to type void so that we are able to allocate blocks of memory which are of different datatypes.(int, char, float etc.)
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
This is where we initialize the first metadata block, update it to refer to the next block of memory. The size of the block that it refers to is (10000 bytes- the_size_of_one_MetaData_block) To indicate that the block is not yet allocated, we set the free flag status to f. And the first metadata block has no next metadata block yet. So we set next to NULL.along with it we print the starting and ending address of heap.
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
We use the First-fit-algorithm to find a free block to allocate memory. Assume that we get a request to allocate a block of memory of size 500 bytes. Starting from the first metadata block we can go on searching for the first block which has enough space to allocate. If we get the free block sizes to be 250, 130 and 750 in order, we will allocate the block of size 750.

If we find a free block which exactly fits the required size, we don't need to do the splitting. So this function is only required if we have what is more than required.
Here we create a new metadata block pointer called "merge_ptr". And it should point to the location provided by passing(setting aside) a block of memory which is equal to the_size_of_the_metadata_block_we_considered + the_size_requested_to_be_allocated

Then this new pointer points to the metadata block referring to the next free chunk. As you can see from the code, I have set the attributes of both the new and fitting_slot metadata blocks accordingly.
The allocate and free functions are explained below.
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
There can be a situation where you have consecutive blocks that are set free by deallocating after they were previously allocated. This results in external fragmentation which will cause the MyAlloc() function to return a NULL pointer although we have enough memory to allocate. Therefore we use a function called merge() to join the consecutive free blocks by removing the metadata blocks lying in between.

After we allocate memory for some purpose, it is a really good practice to deallocate that memory if you have finished using it, so that it can be used by another person for his memory requirement. We use the MyFree() function for that.

It takes the pointer to a block of memory previously allocated as a parameter.
also in functions of allocate and free we check for address range
