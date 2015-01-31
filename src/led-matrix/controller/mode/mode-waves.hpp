/* mode-waves - MatrixMode implementation for showing "waves" resulting from
   oscillating pixels at slightly offset frequencies.  Source:
   http://www.reddit.com/r/gifs/comments/2on8si/connecting_to_server_so_mesmerizing/cmow0sz
 */
#ifndef _MATRIX_MODE_WAVES_H
#define _MATRIX_MODE_WAVES_H

#include <led-matrix/controller/mode/mode.hpp>

class MatrixModeWaves : public MatrixMode
{
public:
    MatrixModeWaves(MatrixDriver * driver);

    void begin();
    void update();
    
private:
    static MatrixMode::MatrixModeID const s_MODE_ID;
    static long int const s_DELAY_MS;
    static std::string const s_NAME_STR;
    static std::string const s_DESCRIPTION_STR;
    
    // Counter used to track current position in the animation
    unsigned long m_timeStep;
};

#endif