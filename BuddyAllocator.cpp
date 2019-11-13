#include "BuddyAllocator.h"
#include <iostream>
#include <math.h>
#include <cmath>
using namespace std;

BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length){
    //int num_blocks = ((log2(_total_memory_length) - log2(_basic_block_size))) + 1; // need to know how many levels we will have
    // Need to find the appriopriate side of blocks in terms of power of 2
    
    // ---------Setting up our code to work for numbers that arent powers of two
    int temp = 1;
    for(int k = 1; k <= _basic_block_size; k = k * 2){
        temp = k;
    }
    basic_block_size = temp;
    
    cout << "basic_block_size: " << basic_block_size << endl;
    
    int temp2 = 1;
    for(int j = 1; j <= _total_memory_length; j = j * 2){
        temp2 = j;
    }
    total_memory_size = temp2;
    
    int temp3 = 0;
    for(int r = basic_block_size; r < total_memory_size; r = r*2){
        temp3++;
    }
    supreme_index = temp3;
    
    //---------Setting up our code to work for numbers that arent powers of two
    
    start = new char[total_memory_size];
    for(int i = 1; i <= (total_memory_size / basic_block_size); i = i * 2){
        char* char_block = new char[total_memory_size];
        BlockHeader* base_block = (BlockHeader*) char_block;

        base_block->block_size = i * _basic_block_size;
        base_block->free = true;
        base_block->next= nullptr;

        LinkedList* LL_object = new LinkedList(); // create LL with that many levels
        if( i == total_memory_size/ basic_block_size){
            LL_object->insert(base_block);
        }
        FreeList.push_back(*LL_object);
    }
    cout << "Size of Freelist: " << FreeList.size() << endl;
}

BuddyAllocator::~BuddyAllocator (){
    delete start;
}


char* BuddyAllocator::alloc(int _length) { // length of how much memory the user wants
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */
    BlockHeader* output; // used for output
//    char* nb = nullptr;
    int needed_size = _length + sizeof(BlockHeader);
//    int possible_index = 0;
//    possible_index = log2(ceil(static_cast<double>(needed_size)
//                               / static_cast<double>(basic_block_size)));
    int possible_index_2 = 0; // index for find suitable block for allocation
    for(int l = basic_block_size; l < needed_size; l = l * 2 ){
        possible_index_2++;
    }
    
    if(possible_index_2 > supreme_index){
        return nullptr; // a case that is not possible for allocation
    }
    
    if(FreeList[possible_index_2].head){
        BlockHeader *nb = FreeList[possible_index_2].remove();
        return (char*)(nb + 1);
    }
    else{
        int j = possible_index_2;
            while(FreeList[j].head == NULL){
                if(j == FreeList.size() - 1){
                    cout << "Cannot Allocate Memory Anymore" << endl;
                    return nullptr;
                }
                j = j + 1;
            }
            while(j > possible_index_2){
                BlockHeader* b = FreeList[j].remove();
                BlockHeader* bb = split(b);
                FreeList[j-1].insert(bb);
                FreeList[j-1].insert(b);
                j--;
            }
        output = FreeList[possible_index_2].remove();
        output->free = false;
        }
    return (char*)(output + 1);
    }


int BuddyAllocator::free(char* _a) {
    // free a certain char* back to the FL
  /* Same here! */
    // attempt 2
    BlockHeader *b2b = (BlockHeader*)(_a - sizeof(BlockHeader));
    b2b->free = true;
    int o = -1;
    FreeList[log2(b2b->block_size / basic_block_size)].insert(b2b);
    while(o != supreme_index - 1){
        
        o = log2(b2b->block_size / basic_block_size);
        //cout << b2b->block_size << endl;
        //cout << "o: " << o <<  endl;
        BlockHeader* bb = getbuddy(b2b);
        
        if(!bb->free){
            return 1; // not free
        }
        else{
            FreeList[o].remove_with_block(bb);
            FreeList[o].remove_with_block(b2b);
            BlockHeader* m = merge(b2b,bb);
            FreeList[o + 1].insert(m);
            b2b = m;
        }
    }
  return 0;
}

BlockHeader* BuddyAllocator::split(BlockHeader* block){

    block->block_size = block->block_size / 2;
    BlockHeader* split1 = (BlockHeader*)((char *)block + block->block_size);
    split1->free = true;
    split1->block_size = block->block_size;
    return split1;
}

BlockHeader* BuddyAllocator::merge(BlockHeader* block1, BlockHeader* block2){

    BlockHeader* merge1;
    if (block2 > block1){
        merge1 = block1;
    }
    else{
        merge1 = block2;
    }
    merge1->block_size = merge1->block_size * 2;
    merge1->next = nullptr;
    return merge1;
}

BlockHeader* BuddyAllocator::getbuddy(BlockHeader* addr){
    return (BlockHeader*)((((char*)(addr)-start) ^ addr->block_size) + start);
}

void BuddyAllocator::printlist (){
  cout << "Printing the Freelist in the format \"[index] (block size) : # of blocks\"" << endl;
  for (int i=0; i<FreeList.size(); i++){
    cout << "[" << i <<"] (" << ((1<<i) * basic_block_size) << ") : ";  // block size at index should always be 2^i * bbs
    int count = 0;
    BlockHeader* b = FreeList [i].head;
    // go through the list from head to tail and count
    while (b){
      count ++;
      // block size at index should always be 2^i * bbs
      // checking to make sure that the block is not out of place
      if (b->block_size != (1<<i) * basic_block_size){
        cerr << "ERROR:: Block is in a wrong list" << endl;
        exit (-1);
      }
      b = b->next;
    }
    cout << count << endl;  
  }
}

