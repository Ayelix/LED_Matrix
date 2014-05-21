#ifdef DEBUG
#include "matrix-debug.h"

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
    atexit(DBG_endwin);
    DBG_PRINTF("Debug mode initialized.\n");
}

#endif