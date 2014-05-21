#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef DEBUG
#define DGB_PRINTF(...)     (fprintf(stderr, __VA_ARGS__))
#else
#define DGB_PRINTF(...)
#endif

#endif