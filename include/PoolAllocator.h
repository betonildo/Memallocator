#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H

#include <PtrHandle.h>
#include <MemoryArray.h>
#include <memallocator_local_includes.h>
#include <memallocator_local_definitions.h>

/* TODO: Use the last bit to represent is the block is occupied or not
#define MINUS_SIGN 0x80000000
#define ALL_BUT_ONE 0x7fffffff
*/
using namespace std;

/*
NOTE: 	OFFSETs need a replacement on different ENDIANNESS for differents CPU. This is made for Intel only that is currently Little Endian.
		The sign need to change to '+' on Big Endian architectures
*/

class PoolAllocator {
	
private:

	struct HeapBlockHeader {
		unsigned short blockNumber;
		unsigned short isBlockInUse;
		UIM blockSize;
	};
	byte* heapSpace;
	UIM nextFree;

	byte** ptrToSpace;
	UIM ptrToSpaceCurrentIndex;

	UIM blocksAllocatedCounter;

public:

	PoolAllocator();
	PoolAllocator(UIM min, UIM Max);

	template<class T>
	PtrHandle<T> allocateWithHandle() {
		T** allocatedPtr = allocate<T>();
		return PtrHandle<T>(allocatedPtr, [=](T** memPtr){
			this->deallocate(*memPtr);
		});
	}
	
	template<class T, size_t size>
	inline MemoryArray<T, size> allocateArray() {
		T** ptr = allocateArray<T>(size);
		PtrHandle<T> ptrHandle(ptr, [=](T** memPtr){
			this->deallocate(*memPtr);
		});
		MemoryArray<T, size> memarray(ptrHandle);
		return memarray;
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

	byte** allocate(size_t structureSize, bool useBlockNumber, UIM blockNumber);

	inline byte** setPointerToSpaceAndReturnTypeCasted(UIM& indexOfFreeAddress) {
		ptrToSpace[ptrToSpaceCurrentIndex] = &heapSpace[indexOfFreeAddress];
		return &ptrToSpace[ptrToSpaceCurrentIndex++];
	}

	template<class T>
	inline T** reallocate(T** structurePtr, size_t newStructureSize) {
		return reinterpret_cast<T**>(reallocateBytes((byte**)structurePtr, newStructureSize));
	}

	byte** reallocateBytes(byte** structurePtr, size_t newStructureSize);
	
	template<class T>
	inline void deallocate(T* block) {		
		HeapBlockHeader* headerOfBlock = (HeapBlockHeader*)((byte*)block - sizeof(HeapBlockHeader));
		headerOfBlock->isBlockInUse = FREE;
	}
	
	inline void replaceMemoryPointersStartingAtDowning(byte* physicAddress, byte blockSize) {
		for(UIM i = 0; i <= ptrToSpaceCurrentIndex; i++)
			if (ptrToSpace[i] >= physicAddress) ptrToSpace[i] -= blockSize;
	}
	
	// NOT THREAD SAFE
	virtual void defragment();
	void printTilNext();
};

#endif /*POOLALLOCATOR_H*/