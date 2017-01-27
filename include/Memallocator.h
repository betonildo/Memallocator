#ifndef HEAP_H
#define HEAP_H

#include <Definitions.h>
#include <CustomTypes.h>
#include <CustomMacros.h>
#include <Debug.h>
#include <Allocator.h>

#define IN_USE 0x01
#define FREE 0x00
#define ONE 1
/* TODO: Use the last bit to represent is the block is occupied or not
#define MINUS_SIGN 0x80000000
#define ALL_BUT_ONE 0x7fffffff
*/
using namespace std;

/*
NOTE: 	OFFSETs need a replacement on different ENDIANNESS for differents CPU. This is made for Intel only that is currently Little Endian.
		The sign need to change to '+' on Big Endian architectures
*/

class Memallocator : public Allocator {
	
private:

	struct HeapBlockHeader {
		unsigned short blockNumber;
		unsigned short isBlockInUse;
		U64 blockSize;
	};

public:

	static byte* heapSpace;
	static U64 nextFree;

	static byte** ptrToSpace;
	static U64 ptrToSpaceCurrentIndex;

	static U64 blocksAllocatedCounter;

	static bool wasInitialized;


	Memallocator();
	Memallocator(U64 min, U64 Max);

	template<class T>
	inline T& allocateClass() {
		T** pointerForClassBlock = this->allocate<T>();
		return **pointerForClassBlock;
	}

	template<class T>
	inline T** allocateArray(size_t count) {
		return (T**)allocate(sizeof(T) * count, false, -1);
	}

	template<class T>
	inline T** allocate() {

		// set size and next free space and
		return (T**)allocate(sizeof(T), false, -1);
	}

	inline virtual byte** allocate(size_t structureSize, bool useBlockNumber, U64 blockNumber) {
		
		U64 sizeOfBlockHeader = (U64)sizeof(HeapBlockHeader);

		// fill block header
		HeapBlockHeader* heapBlockHeader = (HeapBlockHeader*)&heapSpace[nextFree];
		heapBlockHeader->blockNumber = useBlockNumber ? blockNumber : ++blocksAllocatedCounter;
		heapBlockHeader->isBlockInUse = IN_USE;
		heapBlockHeader->blockSize = structureSize;

		// Get the pointer to the first address of the allocated space
		U64 indexOfFreeAddress = nextFree + sizeOfBlockHeader;

		// calculate next free block
		nextFree += structureSize + sizeOfBlockHeader;		
		return setPointerToSpaceAndReturnTypeCasted(indexOfFreeAddress);
	}

	inline byte** setPointerToSpaceAndReturnTypeCasted(U64& indexOfFreeAddress) {

		ptrToSpace[ptrToSpaceCurrentIndex] = &heapSpace[indexOfFreeAddress];
		return &ptrToSpace[ptrToSpaceCurrentIndex++];
	}

	template<class T>
	inline T** reallocate(T** structurePtr, size_t newStructureSize) {
		return reinterpret_cast<T**>(reallocateBytes((byte**)structurePtr, newStructureSize));
	}

	inline byte** reallocateBytes(byte** structurePtr, size_t newStructureSize) {

		HeapBlockHeader* currBlockHeader = reinterpret_cast<HeapBlockHeader*>(*structurePtr - sizeof(HeapBlockHeader));
		HeapBlockHeader* nextBlockHeader = reinterpret_cast<HeapBlockHeader*>(*structurePtr + currBlockHeader->blockSize);

		printf("NextBlock->isBlockInUse = %d\n", nextBlockHeader->isBlockInUse == IN_USE);
		// simplified approach
		// if the pointer points to the last heap allocated, just increment the size in bytes
		uintptr_t freePointerAddress = reinterpret_cast<uintptr_t>(&heapSpace[nextFree]) - reinterpret_cast<uintptr_t>(&heapSpace[0]);
		
		printf("nextFree = %d\n", nextFree);
		printf("freePointerAddress = %d\n", freePointerAddress);
		printf("currBlockHeader->blockNumber = %d\n", currBlockHeader->blockNumber);
		
		if (nextFree <= freePointerAddress && currBlockHeader->blockNumber > nextBlockHeader->blockNumber) {
			nextFree += (newStructureSize - currBlockHeader->blockSize);
			currBlockHeader->blockSize = newStructureSize;
			printf("Extending the last address space\n");
		}
		// else, reserve new space at the end of the heap and copy bytes to there
		else if (nextBlockHeader->isBlockInUse == IN_USE) {
			currBlockHeader->isBlockInUse = FREE;			
			byte** newPtr = allocate(newStructureSize, true, currBlockHeader->blockNumber);
			memcpy(*newPtr, *structurePtr, currBlockHeader->blockSize);
			structurePtr[0] = newPtr[0];
			printf("Reallocating memory\n");
		}
		else {
			// recursively search for contiguos space that can contain the amount of memory been requested
			U64 cumulativeBlocks = currBlockHeader->blockSize;
			printf("Next Block Size = %d\n", nextBlockHeader->blockSize);
			while(cumulativeBlocks < newStructureSize && nextBlockHeader->isBlockInUse == FREE) {
				
				cumulativeBlocks += nextBlockHeader->blockSize;
				printf("nextBlockHeader = %p\n", nextBlockHeader);
				nextBlockHeader = reinterpret_cast<HeapBlockHeader*>(*structurePtr + cumulativeBlocks + sizeof(HeapBlockHeader));
				printf("nextBlockHeader = %p\n", nextBlockHeader);
			}
			printf("nextBlockHeader = %p\n", nextBlockHeader);
			printf("cumulativeBlocks = %d\n", cumulativeBlocks);
			printf("newStructureSize = %d\n", newStructureSize);
			// if we can't allocate memory contiguouly, then giveup and allocate at the end
			if (cumulativeBlocks < newStructureSize) {
				currBlockHeader->isBlockInUse = FREE;			
				byte** newPtr = allocate(newStructureSize, true, currBlockHeader->blockNumber);
				memcpy(*newPtr, *structurePtr, currBlockHeader->blockSize);
				structurePtr[0] = newPtr[0];
				printf("Reallocating memory because we giveup\n");
			}
			else {
				currBlockHeader->blockSize = cumulativeBlocks;				
				printf("Got memory contiguously\n");
			}			
		}

		return structurePtr;
	}

	inline byte** findBytePointerFromHeapPointer(byte* heapPtr) {
		for(U64 i = 0; i <= ptrToSpaceCurrentIndex; i++) {
			if (heapPtr == ptrToSpace[i]) {
				return (byte**)&ptrToSpace[i];
			}
		}

		return NULL;
	}

	template<class T>
	inline uintptr_t deallocate(T* block) {		
		delete block;
		return reinterpret_cast<uintptr_t>(deallocate((void*) block));
	}

	inline uintptr_t deallocate(void* block) {
		byte* bytePtrToFlagAreaOfFreeOrNotNextBytes = ((byte*)block - sizeof(HeapBlockHeader));
		*bytePtrToFlagAreaOfFreeOrNotNextBytes = FREE;
		return reinterpret_cast<uintptr_t>(nullptr);
	}

	inline void replaceMemoryPointersStartingAtDowning(byte* physicAddress, byte blockSize) {

		for(U64 i = 0; i <= ptrToSpaceCurrentIndex; i++) {

			if (ptrToSpace[i] >= physicAddress)
				ptrToSpace[i] -= blockSize;
		}
	}	

	// NOT THREAD SAFE
	virtual void defragment();
	void printTilNext();
};

#endif