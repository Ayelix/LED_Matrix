#include <led-matrix/debug/debug.hpp>
#if defined(MATRIX_DEBUG)

#include <cstdlib>

char DBG_LOGFILE_NAME[] = "matrix-log.txt";
FILE * DBG_logfile = NULL;

// Operations to perform at exit, used with atexit()
static void DBG_shutdown()
{
    endwin();
    fclose(DBG_logfile);
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
    atexit(DBG_shutdown);
    DBG_PRINTF("Debug mode initialized.  Log file is \"%s\".\n", DBG_LOGFILE_NAME);
}

#endif