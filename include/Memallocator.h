#ifndef HEAP_H
#define HEAP_H

#include <Definitions.h>
#include <CustomTypes.h>
#include <CustomMacros.h>
#include <Debug.h>

#define IN_USE 255
#define FREE 0x00

using namespace std;

/*
NOTE: 	OFFSETs need a replacement on different ENDIANNESS for differents CPU. This is made for Intel only that is currently Little Endian.
		The sign need to change to '+' on Big Endian architectures
*/

class Memallocator {
	
private:
	struct HeapBlockHeader {
		U64 isBlockInUse;
		U64 blockSize;
	};

public:

	static byte* heapSpace;
	static U64 nextFree;

	static byte** ptrToSpace;
	static U64 ptrToSpaceCurrentIndex;

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
		return (T**)allocate(sizeof(T) * count);
	}

	template<class T>
	inline T** allocate() {

		// set size and next free space and
		return (T**)allocate(sizeof(T));
	}

	inline byte** allocate(size_t structureSize) {

		U64 sizeOfBlockHeader = (U64)sizeof(HeapBlockHeader);

		// fill block header
		HeapBlockHeader* heapBlockHeader = (HeapBlockHeader*)&heapSpace[nextFree];
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
	inline void deallocate(T* block) {
		byte* bytePtrToFlagAreaOfFreeOrNotNextBytes = ((byte*)block - sizeof(HeapBlockHeader));
		*bytePtrToFlagAreaOfFreeOrNotNextBytes = FREE;
		delete block;
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