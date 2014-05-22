#include "matrix-debug.h"
#ifdef DEBUG

#include <stdlib.h>

FILE * DBG_logfile = NULL;

// Wrapper for endwin(), used for atexit()
static void DBG_endwin()
{
    endwin();
}

void DBG_INIT()
{
    // Open debug log file
    DBG_logfile = fopen("matrix-log.txt", "w");
    if (NULL == DBG_logfile)
    {
        perror("Unable to open log file: ");
        exit(EXIT_FAILURE);
    }

    // Setup ncurses mode
    initscr();
    scrollok(stdscr, true);
    atexit(DBG_endwin);
    DBG_PRINTF("Debug mode initialized.\n");
}

#endif