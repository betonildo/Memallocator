#include <Definitions.h>
#include <CustomTypes.h>

class StackAllocator {

    byte* stack;
    uint_fast64_t stackPtr;

public:

    inline void setStackSize(size_t stackSize) {
        stackPtr = 0;
        stack = new byte[stackSize];
    }
    
    template<class T>
    inline T& allocate() {
        return (T&)*allocate(sizeof(T));
    }

    inline byte* allocate(size_t count) {

        byte* allocPtrAddress = &stack[stackPtr];
        stackPtr = stackPtr + count;
        stack[stackPtr] = count;
        stackPtr++;
        return allocPtrAddress;
    }

    inline void pop() {
        stackPtr = stackPtr - stack[stackPtr - 1] - 1;
    }
    
    inline void clear() {
        stackPtr = 0;
    }

    inline void erase() {
        
        for(U64 i = 0; i < stackPtr; i++) {
            //printf("%p = %d\n", &stack[i], stack[i]);
            stack[i] = 0;
        }
    }

    inline void printStack() {
        printf("CUSTOM STACK\n");
        printf("--------------\n");

        for(U64 i = 0; i < stackPtr; i++) {
            printf("%p = %d\n", &stack[i], stack[i]);
        }

        printf("--------------\n");
    }
};