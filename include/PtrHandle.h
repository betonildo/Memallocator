#ifndef PTRHANDLE_H
#define PTRHANDLE_H

#include <functional>

template<class T>
class PtrHandle {

private:
	unsigned int m_refCounter;
	std::function<void(T** memPtr)> m_MemRescuer;
	T** m_ptrPtr;

public:

	inline PtrHandle(T** ptr, std::function<void(T** memPtr)> rescuer) {
		m_MemRescuer = rescuer;
		m_refCounter = 1;
		m_ptrPtr = ptr;
	}

	inline PtrHandle(const PtrHandle<T>& other) {		
		m_refCounter = other.m_refCounter;
		m_ptrPtr = other.m_ptrPtr;
		m_MemRescuer = other.m_MemRescuer;
		((PtrHandle<T>&)other).m_refCounter += 1;
	}

	inline ~PtrHandle() {

		m_refCounter--;
		if (m_refCounter == 0)
			m_MemRescuer(m_ptrPtr);
	}

	inline T* operator->() {
		return m_ptrPtr[0];
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
