#pragma once
#include "PtrHandle.h"

template <class AllocatorType>
class Allocator{

public:
    template<class PointerType>
    inline PtrHandle<PointerType> allocateWithHandle() {
        return allocator.allocateWithHandle<PointerType>();
    }

private:
    AllocatorType allocator;
};