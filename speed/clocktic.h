
// fetch a representation of the cpu time, in "tics"

// very fast, to assure decent performance for high-volume.

// also, very machine-dependant

// redundancy checker
#pragma once

#if defined(__unix__) || defined(__linux__)
#include <ctime>
#elif defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#endif

// Return some time value, in "ticks".  Use other funcs to convert to millisecs
// Note: UNIX in tics, DOS/NT in millisecs
inline unsigned long clock_tics( )
{
#if defined(__unix__) || defined(__linux__)
    return (unsigned long) clock();
#elif defined(_WIN64) || defined(_WIN32)
    return GetTickCount() ;
#else
    return (unsigned long )
     ( ( * ( unsigned long * ) 0x0040006c ) * 1000 / 18.203 ) ;
#endif
}
