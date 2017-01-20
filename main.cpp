#include <iostream>
#include <stdio.h>
#include <Memallocator.h>
#include <Array.h>

U64 _4MB = 1024 * 1024 * 4;

Memallocator h(_4MB, _4MB);


class ExempleData {
public:
	byte d;

	ExempleData() {
	}

	~ExempleData() {
		printf("calling destructor of ExempleData @ %p\n", this);
	}
};

class DataTest {
public:
	U64 a,b,c,d,e;
};

int main(int argc, char** argv) {	

	U64 test = 200000;
	printf("%d\n", test);


	ExempleData** exPtr = h.allocate<ExempleData>();
	ExempleData** exPtr2 = h.allocate<ExempleData>();
	ExempleData** exPtr3 = h.allocate<ExempleData>();
	
	ExempleData* exPtr_ = *exPtr;
	printf("FIRST PTR ADDRESS: %p\n",exPtr);
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

	

	DataTest& exPtr11 = h.allocateClass<DataTest>();	
	exPtr11.a = 168;

	printf("before defrag exPtr11 address: %p, and a address = %d\n", &exPtr11, exPtr11.a);

	h.printTilNext();
	
	h.defragment();

	printf("after defrag exPtr11 address: %p, and a address = %d\n", &exPtr11, exPtr11.a);

	h.printTilNext();

	
	bool& bToDiscoverSize = h.allocateClass<bool>();
	bToDiscoverSize = true;
	
	{
		ExempleData& exRef = h.allocateClass<ExempleData>();
		exRef.d = 75;
		h.deallocate<ExempleData>(&exRef);
		printf("?\n");
	}	

	h.printTilNext();

	ExempleData** arr = h.allocateArray<ExempleData>(10);
	(*arr)[0].d = 1;
	(*arr)[1].d = 2;
	(*arr)[2].d = 3;
	(*arr)[3].d = 4;
	(*arr)[4].d = 5;
	(*arr)[5].d = 6;
	(*arr)[6].d = 7;
	(*arr)[7].d = 8;
	(*arr)[8].d = 9;
	(*arr)[9].d = 10;

	h.printTilNext();

	h.defragment();

	(*arr)[0].d = 99;

	h.printTilNext();

	// templated dependency injected
	Array<ExempleData, Memallocator> array(10);
	array[0].d = 100;
	array[1].d = 101;
	array[2].d = 102;

	h.printTilNext();

	return 0;
}