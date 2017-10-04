#include <iostream>
#include <string>
#include "TestPtrHandle.h"


int main(int argc, char** argv) {

	testAll();
	
	// PoolAllocator pool;

	// PtrHandle<int> phi = pool.allocateWithHandle<int>();
	// phi.get() = 255;
	// *phi = 123;
	// pool.printTilNext();

	// auto fixedArray = pool.allocateArray<Vectorc, 10>();

	// fixedArray[0] = { 255, 255, 255 };
	// fixedArray[1] = { 255, 255, 255 };

	// pool.printTilNext();

	std::cin.get();

	return 0;
}