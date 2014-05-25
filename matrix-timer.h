/* matrix-timer - defines an interface for Linux's SIGALRM timer to provide
 * non-blocking delays for a single user.
 */
#ifndef _MATRIX_TIMER_H
#define _MATRIX_TIMER_H

namespace MatrixTimer
{
    // Set the timer to go off after the given number of milliseconds
    void startTimer(long int millis);
    
    // Check if the timer (set by the last call to startTimer) has expired
    bool checkTimer();
}

#endif