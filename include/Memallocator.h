#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <string.h>

#define IN_USE 255
#define FREE 0x00
#define OFFSET_TO_FREE_FLAG 2
#define OFFSET_TO_STRUCTURE_SIZE_BYTE 1
#define OFFSET_FROM_HEADER_OF_THE_STRUCTURE 2
typedef unsigned char byte;
typedef size_t U64;
using namespace std;
/*
NOTE: 	OFFSETs need a replacement on different ENDIANNESS for differents CPU. This is made for Intel only that is currently Little Endian.
		The sign need to change to '+' on Big Endian architectures
*/

struct HeapBlockHeader {
	byte isBlockInUse;
	byte blockSize;
};

class Memallocator {
	
private:
	byte* heapSpace;
	U64 nextFree;

	byte** ptrToSpace;
	U64 ptrToSpaceCurrentIndex;


public:
	Memallocator(U64 min, U64 Max);

	template<class T>
	T& allocateClass() {
		T** pointerForClassBlock = this->allocate<T>();
		return *pointerForClassBlock[0];
	}

	template<class T>
	T** allocate() {

		
		// set size and next free space and 
		U64 structureSize = sizeof(T);
		U64 sizeOfBlockHeader = sizeof(HeapBlockHeader);

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

	// void** allocate(U64 sizeInBytes) {

	// } 

	template<class T>
	void deallocate(T* block) {
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