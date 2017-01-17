#ifndef ARRAY_H
#define ARRAY_H

#include <Memallocator.h>
#include <MemTypes.h>

template<class T, Memallocator* mem>
class Array {

    T** array;
    U64 count;
    
public:

    Array(size_t count) {
        array = mem->allocateArray<T>(count);
        this->count = count;
    }

    T& operator[](unsigned int index) {
        return array[0][index];
    }
};


#endif /*ARRAY_H*/