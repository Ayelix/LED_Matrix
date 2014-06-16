#include "matrix-timer.h"

#include <csignal>
#include <atomic>
#include <sys/time.h>

#include "matrix-debug.h"

namespace MatrixTimer
{
    // Anonymous namespace for "private" items
    namespace
    {
        bool timerInitialized = false;
        volatile bool timerFlag;
        
        void sigalrmHandler(int signum)
        {
            timerFlag = true;
        }
    }
    
    void startTimer(long int millis)
    {
        // Cancel any active timer
        struct itimerval delay;
        delay.it_interval.tv_sec = 0;
        delay.it_interval.tv_usec = 0;
        delay.it_value.tv_sec = 0;
        delay.it_value.tv_usec = 0;
        if (setitimer(ITIMER_REAL, &delay, NULL) < 0)
        {
            DBG_PRINTF("ERROR: unable to cancel timer in startTimer.\n");
        }
        
        // Set up the signal handler if needed
        if (!timerInitialized)
        {
            struct sigaction sa;
            sa.sa_handler = sigalrmHandler;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = SA_RESTART;

            if (sigaction(SIGALRM, &sa, NULL) != 0)
            {
                DBG_PRINTF(
                    "ERROR: startTimer failed to register signal handler.\n");
                return;
            }
            
            timerInitialized = true;
        }
        
        // Clear the timer flag
        timerFlag = false;
        
        // Start the new timer
        delay.it_value.tv_sec = millis / 1000;
        long int const secInMillis = (millis / 1000) * 1000;
        delay.it_value.tv_usec =  (millis - secInMillis) * 1000;
        if (setitimer(ITIMER_REAL, &delay, NULL) < 0)
        {
            DBG_PRINTF("ERROR: unable to set timer in startTimer.\n");
        }
    }
    
    bool checkTimer()
    {
        //TODO: if this is not accurate enough, consider changing to a design
        // based on polling system time every time checkTimer is called.
        return timerFlag;
    }
}