#ifndef ENVTYPES_H
#define ENVTYPES_H

#include <Environment.h>

typedef uint_fast8_t byte;

#ifdef ENVIRONMENT64
typedef uint_fast64_t U64;
#else 
typedef uint_fast32_t U64;
#endif

#endif /*ENVTYPES_H*/