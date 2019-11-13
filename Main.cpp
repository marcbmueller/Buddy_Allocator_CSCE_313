#include "Ackerman.h"
#include "BuddyAllocator.h"
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
using namespace std;
void easytest(BuddyAllocator* ba){
  // be creative here
  // know what to expect after every allocation/deallocation cycle
  // here are a few examples
  ba->printlist();
  // allocating a byte
  char * mem = ba->alloc (1);
    char * mem2 = ba->alloc (1);
    char * mem3 = ba->alloc (1);
    
  // now print again, how should the list look now
  ba->printlist();
    
  ba->free (mem2);// give back the memory you just allocated
    ba->free (mem);
    ba->free(mem3);
    
    //ba->free(mem9);
  ba->printlist(); // shouldn't the list now look like as in the beginning

}

int main(int argc, char ** argv) {

    int basic_block_size = 128; // this is in bytes
    int memory_length = 64 * 1024 * 1024; // this is in kilo bytes
    int c;
    while((c = getopt(argc,argv,"b:s:"))!= -1){
        switch(c){
            case 'b':
                basic_block_size = atoi(optarg);
                break;
            case 's':
                memory_length = atoi(optarg);
                break;
            case '?':
                cout << "I am sorry but those are incorrect inputs";
                break;
                
            default: cout << "error, no arguments available" << endl;
        }
    }
    cout << "bbs: " << basic_block_size << endl;
    cout << "size: " << memory_length << endl;

  // create memory manager
  BuddyAllocator * allocator = new BuddyAllocator(basic_block_size, memory_length);


  // the following won't print anything until you start using FreeList and replace the "new" with your own implementation
  //easytest (allocator);

  
  // stress-test the memory manager, do this only after you are done with small test cases
  Ackerman* am = new Ackerman ();
  am->test(allocator); // this is the full-fledged test.
  
  // destroy memory manager
  delete allocator;
}
