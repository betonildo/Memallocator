#ifndef TESTPTRHANDLE_H
#define TESTPTRHANDLE_H

#include <Memallocator.h>
#include <PtrHandle.h>

struct Composite {
	int a;
};

Memallocator g_mem;
int g_counter = 0;

void testAll();
void testEvadingTheScope();
void testCopyingHandle();
PtrHandle<Composite> copyCompositeHandle();

void testAll() {

	testEvadingTheScope();
	testCopyingHandle();
}

void testEvadingTheScope() {

	g_mem.printTilNext();
	{
		PtrHandle<Composite> myHComp = copyCompositeHandle();
		myHComp.get().a = g_counter++;
	}
	g_mem.printTilNext();

	{
		PtrHandle<Composite> myHComp = g_mem.allocateWithHandle<Composite>();
		myHComp.get().a = g_counter++;
	}
	g_mem.printTilNext();
}

void testCopyingHandle() {

}

PtrHandle<Composite> copyCompositeHandle() {

	PtrHandle<Composite> hComp = g_mem.allocateWithHandle<Composite>();
	hComp.get().a = g_counter++;
	g_mem.printTilNext();
	return hComp;
}

#endif /*TESTPTRHANDLE_H*/
