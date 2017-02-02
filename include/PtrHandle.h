#ifndef PTRHANDLE_H
#define PTRHANDLE_H

#include <Definitions.h>

#if defined(DEBUG)
	#include <Debug.h>
#endif

template<class T>
class PtrHandle {

private:
	unsigned int m_refCounter;
	T** m_ptrPtr;
	void* m_allocator;
	void(*m_deallocateWithAllocatorFunction)(void*, void*);

public:


	inline PtrHandle(T** ptr, void* allocator, void (*deallocateWithAllocatorFunction)(void*, void*)) {
		m_refCounter = 1;
		m_ptrPtr = ptr;
		m_allocator = allocator;
		m_deallocateWithAllocatorFunction = deallocateWithAllocatorFunction;
	}

	inline PtrHandle(PtrHandle& other) {		
		m_refCounter = other.m_refCounter;
		m_ptrPtr = other.m_ptrPtr;
		m_allocator = other.m_allocator;
		m_deallocateWithAllocatorFunction = other.m_deallocateWithAllocatorFunction;
		other.m_refCounter += 1;
	}

	inline ~PtrHandle() {

		m_refCounter--;
		if (m_refCounter == 0) {
			m_deallocateWithAllocatorFunction(m_allocator, *m_ptrPtr);
		}
	}

	inline PtrHandle& operator=(const PtrHandle& other) {
		*this(other);		
		return *this;
	}

	inline T** getPtr() const {
		return m_ptrPtr;
	}
	
	inline T& get() {
		return *m_ptrPtr[0];
	}
};

#endif /*PTRHANDLE_H*/
