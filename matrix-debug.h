#ifndef _DEBUG_H
#define _DEBUG_H

#include <ncurses.h>

#ifdef DEBUG
#include <cstdio>
extern FILE *DBG_logfile;
extern void DBG_INIT();
#define DBG_PRINTF(...) {printw(__VA_ARGS__); refresh(); \
                        fprintf(DBG_logfile, __VA_ARGS__);}

#else
#define DBG_INIT(...)
#define DBG_PRINTF(...) 
#endif

#endif