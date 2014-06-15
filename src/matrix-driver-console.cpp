#include "matrix-driver-console.h"

MatrixDriverConsole::MatrixDriverConsole()
{
    // Determine window size based on matrix size.  Pixel width is doubled to
    // make each pixel closer to square.  2 is added to leave room for a border.
    size_t const height = ROWS + 2;
    size_t const width = (2 * COLUMNS) + 2;
    
    // Resize and move main window to not overlap new window
    wresize(stdscr, LINES-height, COLS);
    mvwin(stdscr, height, 0);
    wrefresh(stdscr);
    
    // Create new ncurses window at top right of screen
    // First the window the full screen width so refreshing it will clear any
    // text left over from the old stdscr.
    matrixWindow = newwin(height, COLS, 0, 0);
    wrefresh(matrixWindow);
    wresize(matrixWindow, height, width);
    curs_set(0);
    box(matrixWindow, 0, 0);
    wrefresh(matrixWindow);
}

void MatrixDriverConsole::update()
{
    // Do nothing if the state hasn't changed
    if (stateChanged)
    {
        for (size_t y = 0; y < ROWS; y++)
        {
            // Move the cursor to the beginning of the line
            wmove(matrixWindow, 1+y, 1);
            
            for (size_t x = 0; x < COLUMNS; x++)
            {
                // If the current pixel is set, write a block in the pixel.
                // Otherwise, write a blank space.
                chtype c = ' ';
                if (getPixel(x, y))
                {
                    c |= A_REVERSE;
                }
                waddch(matrixWindow, c);
                waddch(matrixWindow, c);
            }
        }
        
        // Refresh the window
        wrefresh(matrixWindow);
        
        // Reset stateChanged flag now that the matrix has been updated
        stateChanged = false;
    }
    else
    {
        DBG_PRINTF("MatrixDriverConsole::update(): stateChanged is false, do nothing.\n");
    }
}