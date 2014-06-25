/* matrix-controller - class to handle high-level control of the matrix, like
 * animations, timing, etc.
 */
#ifndef _MATRIX_CONTROLLER_H
#define _MATRIX_CONTROLLER_H

#include <led-matrix/driver/driver.hpp>
#include <led-matrix/mode/mode.hpp>

#include <string>
#include <vector>

class MatrixController
{
public:
    // Create a MatrixController which will use the given driver
    MatrixController(MatrixDriver * const driver);
    
    // Update the current mode if needed.  This method should be called
    // as frequently as possible (i.e. in an event loop) to keep the internal
    // timing as accurate as possible.
    void update();
    
    // Get the current mode
    MatrixMode * getMode() const {return m_modes.at(m_currentModeIndex);}
    // Set the current mode by a pointer to the mode itself
    void setMode(MatrixMode const * mode);
    // Set the current mode by index in the list provided by getModes()
    void setMode(unsigned int modeIndex);
    // Move to the next mode.  Returns the index of the new mode in the list
    // provided by getModes()
    unsigned int nextMode();
    
    // Get the list of modes available with this controller
    std::vector<MatrixMode *> const & getModes() const {return m_modes;}
    
private:
    // MatrixDriver to be used by this controller
    MatrixDriver * const m_driver;
    
    // Modes available to the controller
    std::vector<MatrixMode *> m_modes;
    
    // Index of current mode in m_modes
    unsigned int m_currentModeIndex;
    
    /***************************************************************************
     * Misc helper methods
    // Write one column of pixels (from a font character) at the given column.
    void writeCharacterColumn(uint16_t columnValue, size_t col);
    // Plot the given level (normalized to 0-100) on the matrix, using the given
    // plot type/style and optionally filling all points below the given level.
    typedef enum
    {
        // Bar-graph type plot going from bottom to top
        MATRIX_CONTROLLER_PLOT_TYPE_VERTICAL,
        // Bar-graph type plot going from left to right
        MATRIX_CONTROLLER_PLOT_TYPE_HORIZONTAL,
        // Shift matrix left and plot the new level on the rightmost column
        MATRIX_CONTROLLER_PLOT_TYPE_SHIFTING,
        
        MATRIX_CONTROLLER_PLOT_TYPE_COUNT
    } PlotType;
    void plotLevel(unsigned int level,
        PlotType type = MATRIX_CONTROLLER_PLOT_TYPE_VERTICAL,
        bool fill = false);
    */
};

#endif