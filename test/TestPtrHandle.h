#ifndef TESTPTRHANDLE_H
#define TESTPTRHANDLE_H

#include <PoolAllocator.h>
#include <PtrHandle.h>

struct Composite {
	char a;
};

PoolAllocator g_mem;
int g_counter = 0;

void testAll();
void testEvadingTheScope();
void testCopyingHandle();
PtrHandle<Composite> copyCompositeHandle();

void testAll() {

	testEvadingTheScope();
	testCopyingHandle();
}

struct Vector {
	float x,y,z;
};

void testEvadingTheScope() {
	
	{
		PtrHandle<Composite> myHComp = copyCompositeHandle();
		myHComp->a = ++g_counter;
		g_mem.printTilNext();
	}
	g_mem.defragment();
	{
		PtrHandle<Composite> myHComp = copyCompositeHandle();
		myHComp->a = ++g_counter;
		g_mem.printTilNext();
	}
	g_mem.printTilNext();

	PtrHandle<Vector> pVec = g_mem.allocateWithHandle<Vector>();

	pVec->x = 123;

	g_mem.printTilNext();
}

void testCopyingHandle() {

}

PtrHandle<Composite> copyCompositeHandle() {

	PtrHandle<Composite> hComp = g_mem.allocateWithHandle<Composite>();
	return hComp;
}

#endif /*TESTPTRHANDLE_H*/
