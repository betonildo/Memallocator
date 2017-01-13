#include <Memallocator.h>

Memallocator::Memallocator(U64 min, U64 Max) {
    heapSpace = new byte[min];
    ptrToSpace = new byte*[min];
    

    for(U64 i = 0; i < min; i++)
        ptrToSpace[i] = new byte[1];

    nextFree = 0;
    ptrToSpaceCurrentIndex = 0;
}

void Memallocator::defragment() {

    U64 sizeOfBlockHeader = sizeof(HeapBlockHeader);

    U64 nextIncrement = 0;
    for(U64 i = 0; i < nextFree; i += nextIncrement) {

        HeapBlockHeader* heapBlockHeaderTestingFree = (HeapBlockHeader*)&heapSpace[i];        
        byte blockFlag = heapBlockHeaderTestingFree->isBlockInUse;

        if (blockFlag == IN_USE) {
            nextIncrement = heapBlockHeaderTestingFree->blockSize + sizeOfBlockHeader;;
        }
        else {
            
            // increment to not look at the same place again
            U64 freeBlockIncrement = 0;

            for(U64 j = i; j < nextFree; j += freeBlockIncrement) {
                
                HeapBlockHeader* blockHeader = (HeapBlockHeader*)&heapSpace[i];
                byte freeFlag = blockHeader->isBlockInUse;
                freeBlockIncrement = blockHeader->blockSize + sizeOfBlockHeader;

                if (freeFlag == FREE) {

                    // memory defragmentation is working                  
                    U64 heapNextOccupiedIndex = j + freeBlockIncrement;
                    nextFree -= freeBlockIncrement;
                    memcpy(&heapSpace[j], &heapSpace[heapNextOccupiedIndex], nextFree);

                    // replace memory pointers to pointing address
                    byte* physicAddressOfFreeBlock = &heapSpace[j + sizeOfBlockHeader];
                    replaceMemoryPointersStartingAtDowning(physicAddressOfFreeBlock, freeBlockIncrement);

                    // THIS NEEDS TO BE AT THIS END OF BLOCK, OTHERWISE IT WILL BE AN INFINITY LOOP
                    // watch for this same location again if we moved to some another free block
                    freeBlockIncrement = 0;
                }
            }
    
            nextIncrement = freeBlockIncrement;
        }
    }
}

void Memallocator::printTilNext() {
    
    printf("HEAP BLOCK:\n");   
    
    for(U64 i = 0; i < nextFree; i++) {
        printf("Address: %p, %ld = %ld\n", &heapSpace[i], i, heapSpace[i]);
    }
}