#ifndef MEMTYPES_H
#define MEMTYPES_H

#include <Environment.h>

#include <cstdint>

#ifndef BYTE
typedef uint_fast8_t byte;
#endif


#ifdef ENVIRONMENT64
typedef uint_fast64_t U64;
#else 
typedef uint_fast32_t U64;
#endif

#endif /*MEMTYPES_H*/