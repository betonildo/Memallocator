#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <string.h>
#include <EnvTypes.h>

#define IN_USE 255
#define FREE 0x00

using namespace std;

/*
NOTE: 	OFFSETs need a replacement on different ENDIANNESS for differents CPU. This is made for Intel only that is currently Little Endian.
		The sign need to change to '+' on Big Endian architectures
*/

class Memallocator {
	
private:
	byte* heapSpace;
	U64 nextFree;

	byte** ptrToSpace;
	U64 ptrToSpaceCurrentIndex;

	struct HeapBlockHeader {
		byte isBlockInUse;
		byte blockSize;
	};

public:
	Memallocator(U64 min, U64 Max);

	template<class T>
	inline T& allocateClass() {
		T** pointerForClassBlock = this->allocate<T>();
		return **pointerForClassBlock;
	}

	template<class T>
	inline T** allocate() {

		
		// set size and next free space and 
		U64 structureSize = sizeof(T);
		U64 sizeOfBlockHeader = (U64)sizeof(HeapBlockHeader);

		printf("Structure size: %d, Block Size: %d\n", structureSize, sizeOfBlockHeader);

		HeapBlockHeader* heapBlockHeader = (HeapBlockHeader*)&heapSpace[nextFree];
		heapBlockHeader->isBlockInUse = IN_USE;
		heapBlockHeader->blockSize = structureSize;

		// Get the pointer to the first address of the allocated space
		U64 indexOfFreeAddress = nextFree + sizeOfBlockHeader;

		// calculate next free block
		nextFree += structureSize + sizeOfBlockHeader;
		
		return setPointerToSpaceAndReturnTypeCasted<T>(indexOfFreeAddress);
	}

	template<class T>
	inline T** setPointerToSpaceAndReturnTypeCasted(U64& indexOfFreeAddress) {

		ptrToSpace[ptrToSpaceCurrentIndex] = &heapSpace[indexOfFreeAddress];
		return (T**)&ptrToSpace[ptrToSpaceCurrentIndex++];
	}

	template<class T>
	inline void deallocate(T* block) {
		byte* bytePtrToFlagAreaOfFreeOrNotNextBytes = ((byte*)block - sizeof(HeapBlockHeader));
		*bytePtrToFlagAreaOfFreeOrNotNextBytes = FREE;
	}

	void deallocate(void* block) {
		byte* bytePtrToFlagAreaOfFreeOrNotNextBytes = ((byte*)block - sizeof(HeapBlockHeader));
		*bytePtrToFlagAreaOfFreeOrNotNextBytes = FREE;
	}

	inline void replaceMemoryPointersStartingAtDowning(byte* physicAddress, byte blockSize) {

		for(U64 i = 0; i <= ptrToSpaceCurrentIndex; i++) {

			if (ptrToSpace[i] >= physicAddress)
				ptrToSpace[i] -= blockSize;
		}
	}

	// NOT THREAD SAFE
	void defragment();
	void printTilNext();
};

#endif