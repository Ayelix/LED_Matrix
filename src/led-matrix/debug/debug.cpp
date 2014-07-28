#include <led-matrix/debug/debug.hpp>
#if defined(MATRIX_DEBUG)

#include <cstdlib>
#include <cstdarg>
#include <cstdio>
#include <csignal>

static FILE * DBG_logfile = NULL;
static bool DBG_noTerminal = false;

// Operations to perform at exit, used with atexit()
static void DBG_shutdown()
{
    if (!DBG_noTerminal)
    {
        endwin();
    }
    fclose(DBG_logfile);
}

// Signal handler for SIGINT, just calls exit()
static void DBG_sigint_handler(int signum)
{
    exit(EXIT_FAILURE);
}

void DBG_INIT(std::string const & logfile, bool noTerminal)
{
    DBG_noTerminal = noTerminal;
    
    // Open debug log file
    DBG_logfile = fopen(logfile.c_str(), "w");
    if (NULL == DBG_logfile)
    {
        perror("Unable to open log file: ");
        exit(EXIT_FAILURE);
    }
    
    // Set up SIGINT handler to exit cleanly
    if (SIG_ERR == signal(SIGINT, DBG_sigint_handler))
    {
        perror("Unable to set up SIGINT handler: ");
        exit(EXIT_FAILURE);
    }

    // Setup ncurses mode
    if (!DBG_noTerminal)
    {
        initscr();
        scrollok(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        noecho();
        atexit(DBG_shutdown);
    }
    DBG_PRINTF("Debug mode initialized.  Log file is \"%s\" and terminal output is %s.\n",
        logfile.c_str(),
        (DBG_noTerminal) ? ("disabled") : ("enabled"));
}

void DBG_PRINTF(char const * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(DBG_logfile, fmt, args);
    if (!DBG_noTerminal)
    {
        vwprintw(stdscr, fmt, args);
        refresh();
    }
    va_end(args);
}

#endif