#include <iostream>
#include <stdio.h>
#include <Memallocator.h>

class ExempleData {
public:
	byte d;
};

class DataTest {
public:
	U64 a,b,c,d,e;
};

int main(int argc, char** argv) {
	
	U64 _2MB = 1024 * 1024 * 2;
	U64 _4MB = 1024 * 1024 * 4;
	U64 _40MiB = 1024 * 1024 * 40;

	U64 test = 200000;
	printf("%d\n", test);

	Memallocator h(_4MB, _4MB);

	ExempleData** exPtr = h.allocate<ExempleData>();
	ExempleData** exPtr2 = h.allocate<ExempleData>();
	ExempleData** exPtr3 = h.allocate<ExempleData>();
	
	ExempleData* exPtr_ = *exPtr;
	printf("FIRST PTR ADDRESS: %d\n",exPtr);
	exPtr_->d = 3;
	exPtr2[0]->d = 123;
	(*exPtr3)->d = 98;

	h.printTilNext();

	h.deallocate<ExempleData>(*exPtr2);

	h.printTilNext();

	h.defragment();

	h.printTilNext();

	ExempleData** exPtr4 = h.allocate<ExempleData>();

	(*exPtr4)->d = 64;

	h.printTilNext();

	(*exPtr3)->d = 250;

	h.printTilNext();


	ExempleData** exPtr5 = h.allocate<ExempleData>();
	ExempleData** exPtr6 = h.allocate<ExempleData>();
	ExempleData** exPtr7 = h.allocate<ExempleData>();
	ExempleData** exPtr8 = h.allocate<ExempleData>();
	ExempleData** exPtr9 = h.allocate<ExempleData>();
	ExempleData** exPtr10 = h.allocate<ExempleData>();
	
	(*exPtr5)->d = 88;
	(*exPtr6)->d = 89;
	(*exPtr7)->d = 90;
	(*exPtr8)->d = 91;
	(*exPtr9)->d = 92;
	(*exPtr10)->d = 93;

	h.deallocate<ExempleData>(*exPtr7);
	h.deallocate<ExempleData>(*exPtr9);
	h.deallocate<ExempleData>(*exPtr10);

	h.printTilNext();

	DataTest** exPtr11 = h.allocate<DataTest>();
	h.defragment();

	bool bToDiscoverSize = h.allocateClass<bool>();
	
	ExempleData& exRef = h.allocateClass<ExempleData>();
	exRef.d = 75;
	
	h.printTilNext();

	bToDiscoverSize = true;

	(*exPtr2)->d = 67;

	h.deallocate((void*)*exPtr2);

	h.printTilNext();

	return 0;
}