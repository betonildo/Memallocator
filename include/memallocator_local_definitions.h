#ifndef MEMALLOCATOR_LOCAL_DEFINITIONS_H
#define MEMALLOCATOR_LOCAL_DEFINITIONS_H

#include <memallocator_local_includes.h>

#ifndef _UIM_
#define _UIM_

#ifdef WIN32
typedef uint_fast32_t UIM;
#endif

#ifdef WIN64
typedef uint_fast64_t UIM;
#endif

#endif

#ifndef _BYTE_
#define _BYTE_
typedef uint_fast8_t byte;
#endif

#ifndef IN_USE
#define IN_USE 0x01
#endif

#ifndef FREE
#define FREE 0x00
#endif

#ifndef ONE
#define ONE 1
#endif

#endif /*MEMALLOCATOR_LOCAL_DEFINITIONS_H*/
