
#include <stdint.h>
#include <iostream>
#include <memory>
#include <vector>
#include "PoolAllocator.h"

#pragma pack(push, 2)
class TestClass{
    
public:
    std::string mname;
    short mb;

    TestClass(int b, std::string name){
        mname = name;
        mb = b;

        std::cout << mname << " = " << mb << " Constructing..." << std::endl;
    }

    ~TestClass() {
        std::cout << mname << " = " << mb << " Destructing.." << std::endl;
    }
};
#pragma pack(pop)

std::ostream& operator<<(std::ostream& os, const TestClass& tc) {
    os << tc.mname << " = " << tc.mb;
    return os;
}

int main() {

    PoolAllocator pool(1024);
    {
        std::shared_ptr<TestClass> tptr = pool.create<TestClass>(123, "Primeiro");
        std::cout << tptr->mb << std::endl;
    }

    std::shared_ptr<TestClass> tptr = pool.create<TestClass>(321, "Segundo");    
    std::shared_ptr<TestClass> tptr2 = pool.create<TestClass>(456, "Terceiro");


    std::cout << tptr->mb << std::endl;
    std::cout << tptr2->mb << std::endl;

    std::cout << "HERE" << std::endl;

    return 0;
}