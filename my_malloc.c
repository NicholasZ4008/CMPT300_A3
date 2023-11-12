#include "my_malloc.h"
typedef struct Block{
    size_t size;
    struct Block* left;
    struct Block* right;
} Block;

static bool is_malloc_init = false;
static char memory_pool[MEMORY_POOL_SIZE];
static List* free_block_list = NULL;

void my_malloc_init(){
    free_block_list = List_create();
    
    Block* initial_block = (Block*) memory_pool;
    initial_block->size = MEMORY_POOL_SIZE;
    initial_block->left = NULL;
    initial_block->right = NULL;
    List_append(free_block_list, initial_block);
}

void* my_malloc(size_t size){
    if(!is_malloc_init){
        my_malloc_init();
    }
    size_t total_size = size + sizeof(Block);

    void* temp = List_first(free_block_list);
    Block** current_block = (Block**)&temp;
    while(*current_block){
        //Space left
        if((*current_block)->size >= total_size){
            //Split block if larger than needed
            if(((*current_block)->size)>total_size){
                Block* new_block = (Block*)((char*)(*current_block)+total_size);
                new_block->size = (*current_block)->size - total_size;
                new_block->left = NULL;
                new_block->right = NULL;
                if(total_size <= MEMORY_POOL_SIZE/2){
                    //Split if enough space left
                    (*current_block)->size = total_size;
                }else{
                    new_block->size += (*current_block)->size;
                    free(new_block);
                    new_block = NULL;
                }
                *current_block = new_block;
            }
            return (char*)(*current_block) + sizeof(Block);
        }
        if(total_size < (*current_block)->size){
            current_block = &(*current_block)->left;
        } else {
            current_block = &(*current_block) -> right;
        }
    }
    return NULL;
}
void my_free(void* block){
    
}