#ifndef PTRHANDLE_H
#define PTRHANDLE_H

class PoolAllocator;
#include <PoolAllocator.h>

template<class T>
class PtrHandle {

private:
	unsigned int m_refCounter;
	PoolAllocator* m_memRef;
	T** m_ptrPtr;

public:

	inline PtrHandle(T** ptr, PoolAllocator* mem) {
		m_memRef = mem;
		m_refCounter = 1;
		m_ptrPtr = ptr;
	}

	inline PtrHandle(PtrHandle& other) {		
		m_refCounter = other.m_refCounter;
		m_ptrPtr = other.m_ptrPtr;

		m_memRef = other.m_memRef;
		other.m_refCounter += 1;
	}

	inline ~PtrHandle() {

		m_refCounter--;
		if (m_refCounter == 0)
			m_memRef->deallocate(*m_ptrPtr);
	}

	inline PtrHandle& operator=(const PtrHandle& other) {
		*this(other);		
		return *this;
	}

	inline T& operator*() {
		return get();
	}
	
	inline T* getPtr() const {
		return m_ptrPtr[0];
	}
	
	inline T& get() {
		return *m_ptrPtr[0];
	}
};

#endif /*PTRHANDLE_H*/
