/* matrix-debug - functions to write debug output to screen.
 *
 */
#ifndef _DEBUG_H
#define _DEBUG_H

#include <ncurses.h>
#include <string>

// Undefine if debug output no longer desired for all builds
#define MATRIX_DEBUG MATRIX_DEBUG

#if defined(MATRIX_DEBUG)
extern void DBG_INIT(std::string const & logfile, bool noTerminal = false);
extern void DBG_PRINTF(char const * fmt, ...);
                        
#else
#define DBG_INIT(...)
#define DBG_PRINTF(...) 
#endif

#endif