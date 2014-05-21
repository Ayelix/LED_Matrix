#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef DEBUG
#define DBG_PRINTF(...) fprintf(stderr, __VA_ARGS__)
#else
#define DBG_PRINTF(...) 
#endif

#endif