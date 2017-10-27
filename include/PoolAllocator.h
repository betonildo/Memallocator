#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H

#include <vector>
#include <memory>
#include <assert.h>

class PoolAllocator {

public:
    explicit PoolAllocator(size_t poolsize) {
        mPoolIndex = 0;
        mPoolsize = poolsize;
        mPool = new byte[mPoolsize];
        mFree_list.reserve(mPoolsize);
    }

    explicit PoolAllocator(const PoolAllocator& other) = default;

    ~PoolAllocator() {
        delete mPool;
        mFree_list.clear();
    }

    template<typename Type, typename... TypeArgs>
    std::shared_ptr<Type> create(TypeArgs&&... args) {
        // get pointer for byte correspondent and reinterpret it as the type used
        byte* byte_array = this->allocate(sizeof(Type));
        Type* object_ptr = reinterpret_cast<Type*>(byte_array);

        // call object constructor
        construct(object_ptr, std::forward<TypeArgs>(args)...);

        // custom deleter of this shared_ptr
        auto custom_deleter = std::bind(&PoolAllocator::free_pool_space<Type>, this, object_ptr);
        return std::shared_ptr<Type>(object_ptr, custom_deleter);
    }

private:
    typedef unsigned char byte;
    typedef std::pair<unsigned int, size_t> IndexSize;
    
    std::vector<IndexSize> mFree_list;
    unsigned int mPoolIndex;
    size_t mPoolsize;
    byte* mPool;

    byte* allocate(size_t size) {

        // search first on free list
        size_t free_list_size = mFree_list.size();
        for (unsigned int free_object_index = 0; free_object_index < free_list_size; free_object_index++) {
            auto& free_object = mFree_list[free_object_index];
            if (size == free_object.second) {
                // remove free list entry (PERFORMANCE CRITICAL?)
                mFree_list.erase(mFree_list.begin() + free_object_index);
                return &mPool[free_object.first];
            }
        }

        // pool allocation exceeds
        assert(mPoolIndex < mPoolsize);
        
        // get next byte address, pass to the next byte and return the address from pool
        byte* byte_address = &mPool[mPoolIndex];
        mPoolIndex += size;
        return byte_address;
    }

    template<typename Type>
    void free_pool_space(Type* object_ptr) {
        // get addresses to free
        uintptr_t pool_address = reinterpret_cast<uintptr_t>(mPool) >> 2; // divide by 4, because it is 4 byte based address
        uintptr_t byte_address = reinterpret_cast<uintptr_t>(object_ptr) >> 2; // divide by 4, because it is 4 byte based address
        
        // TODO: Need to treat platform endianess (MAYBE?)
        uintptr_t free_index = abs(byte_address - pool_address);
        mFree_list.emplace_back(free_index, sizeof(Type));

        // call destructor
        object_ptr->~Type();
    }

    // ugly constructor call syntax
    template<typename Type, typename... TypeArgs>
    void construct(Type* object_ptr, TypeArgs&&... args) {
        ::new((void*)object_ptr) Type(std::forward<TypeArgs>(args)...);
    }
};

#endif /*POOLALLOCATOR_H*/