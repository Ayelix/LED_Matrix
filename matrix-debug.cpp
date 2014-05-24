#include "matrix-debug.h"
#ifdef DEBUG

#include <stdlib.h>

char DBG_LOGFILE_NAME[] = "matrix-log.txt";
FILE * DBG_logfile = NULL;

// Wrapper for endwin(), used for atexit()
static void DBG_endwin()
{
    endwin();
}

void DBG_INIT()
{
    // Open debug log file
    DBG_logfile = fopen(DBG_LOGFILE_NAME, "w");
    if (NULL == DBG_logfile)
    {
        perror("Unable to open log file: ");
        exit(EXIT_FAILURE);
    }

    // Setup ncurses mode
    initscr();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();
    atexit(DBG_endwin);
    DBG_PRINTF("Debug mode initialized.  Log file is \"%s\".\n", DBG_LOGFILE_NAME);
}

#endif