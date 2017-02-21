#include <PoolAllocator.h>
#include <memallocator_local_definitions.h>

PoolAllocator::PoolAllocator() {
    
	const UIM _1MiB = 1024 * 1024 * 1;

	heapSpace = (byte*)malloc(_1MiB);
	ptrToSpace = (byte**)malloc(_1MiB);

	nextFree = 0;
	ptrToSpaceCurrentIndex = 0;
}

PoolAllocator::PoolAllocator(UIM min, UIM Max) {
    
	heapSpace = (byte*)malloc(min);
	ptrToSpace = (byte**)malloc(min);

	nextFree = 0;
	ptrToSpaceCurrentIndex = 0;
}

byte** PoolAllocator::allocate(size_t structureSize, bool useBlockNumber, UIM blockNumber) {

	UIM sizeOfBlockHeader = (UIM)sizeof(HeapBlockHeader);

	// fill block header
	HeapBlockHeader* heapBlockHeader = (HeapBlockHeader*)&heapSpace[nextFree];
	heapBlockHeader->blockNumber = useBlockNumber ? blockNumber : ++blocksAllocatedCounter;
	heapBlockHeader->isBlockInUse = IN_USE;
	heapBlockHeader->blockSize = structureSize;

	// Get the pointer to the first address of the allocated space
	UIM indexOfFreeAddress = nextFree + sizeOfBlockHeader;

	// calculate next free block
	nextFree += structureSize + sizeOfBlockHeader;
	return setPointerToSpaceAndReturnTypeCasted(indexOfFreeAddress);
}

byte** PoolAllocator::reallocateBytes(byte** structurePtr, size_t newStructureSize) {

	HeapBlockHeader* currBlockHeader = reinterpret_cast<HeapBlockHeader*>(*structurePtr - sizeof(HeapBlockHeader));
	HeapBlockHeader* nextBlockHeader = reinterpret_cast<HeapBlockHeader*>(*structurePtr + currBlockHeader->blockSize);

	// simplified approach
	// if the pointer points to the last heap allocated, just increment the size in bytes
	uintptr_t freePointerAddress = reinterpret_cast<uintptr_t>(&heapSpace[nextFree]) - reinterpret_cast<uintptr_t>(&heapSpace[0]);

	if (nextFree <= freePointerAddress && currBlockHeader->blockNumber > nextBlockHeader->blockNumber) {
		nextFree += (newStructureSize - currBlockHeader->blockSize);
		currBlockHeader->blockSize = newStructureSize;
	}
	// else, reserve new space at the end of the heap and copy bytes to there
	else if (nextBlockHeader->isBlockInUse == IN_USE) {
		currBlockHeader->isBlockInUse = FREE;
		byte** newPtr = allocate(newStructureSize, true, currBlockHeader->blockNumber);
		memcpy(*newPtr, *structurePtr, currBlockHeader->blockSize);
		structurePtr[0] = newPtr[0];
	}
	else {
		// recursively search for contiguos space that can contain the amount of memory been requested
		UIM cumulativeBlocks = currBlockHeader->blockSize;
		while (cumulativeBlocks < newStructureSize && nextBlockHeader->isBlockInUse == FREE) {

			cumulativeBlocks += nextBlockHeader->blockSize;
			nextBlockHeader = reinterpret_cast<HeapBlockHeader*>(*structurePtr + cumulativeBlocks + sizeof(HeapBlockHeader));
		}

		// if we can't allocate memory contiguouly, then giveup and allocate at the end
		if (cumulativeBlocks < newStructureSize) {
			currBlockHeader->isBlockInUse = FREE;
			byte** newPtr = allocate(newStructureSize, true, currBlockHeader->blockNumber);
			memcpy(*newPtr, *structurePtr, currBlockHeader->blockSize);
			structurePtr[0] = newPtr[0];
		}
		else {
			currBlockHeader->blockSize = cumulativeBlocks;
		}
	}

	return structurePtr;
}

void PoolAllocator::defragment() {

    UIM sizeOfBlockHeader = sizeof(HeapBlockHeader);

    UIM nextIncrement = 0;
    for(UIM i = 0; i < nextFree; i += nextIncrement) {

        HeapBlockHeader* heapBlockHeaderTestingFree = (HeapBlockHeader*)&heapSpace[i];        
        byte blockFlag = heapBlockHeaderTestingFree->isBlockInUse;

        if (blockFlag == IN_USE) {
            nextIncrement = heapBlockHeaderTestingFree->blockSize + sizeOfBlockHeader;;
        }
        else {
            
            // increment to not look at the same place again
            UIM freeBlockIncrement = 0;

            for(UIM j = i; j < nextFree; j += freeBlockIncrement) {
                
                HeapBlockHeader* blockHeader = (HeapBlockHeader*)&heapSpace[i];
                byte freeFlag = blockHeader->isBlockInUse;
                freeBlockIncrement = blockHeader->blockSize + sizeOfBlockHeader;

                if (freeFlag == FREE) {

                    // memory defragmentation is working                  
                    UIM heapNextOccupiedIndex = j + freeBlockIncrement;
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

void PoolAllocator::printTilNext() {
    
    printf("HEAP BLOCK:\n");   
    
    for(UIM i = 0; i < nextFree; i++) {
        printf("Address: %p, %d = %u\n", &heapSpace[i], i, heapSpace[i]);
    }
}