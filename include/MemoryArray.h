#ifndef MEMORYARRAY_H
#define MEMORYARRAY_H

#include <PtrHandle.h>

template<class T, size_t size>
class MemoryArray {

	PtrHandle<T> m_ptr;
public:


	inline MemoryArray(PtrHandle<T>& handle) : m_ptr(handle) {

	}

	inline T& operator[](size_t index) {
		return m_ptr.getPtr()[index];
	}
};


#endif /*MEMORYARRAY_H*/
