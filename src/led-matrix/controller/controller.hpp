/* matrix-controller - class to handle high-level control of the matrix, like
 * animations, timing, etc.
 */
#ifndef _MATRIX_CONTROLLER_H
#define _MATRIX_CONTROLLER_H

#include <led-matrix/driver/driver.hpp>
#include <led-matrix/font/font.hpp>

#include <string>

class MatrixController
{
public:
    // Modes for the controller identify the current animation.
    typedef enum
    {
        // Idle/off state - all pixels off
        MATRIX_CONTROLLER_MODE_IDLE,
        // Scrolling text mode
        MATRIX_CONTROLLER_MODE_TEXT,
        // VU meter mode
        MATRIX_CONTROLLER_MODE_VU,
        // Sine wave mode
        MATRIX_CONTROLLER_MODE_SINE,
        
        // Add new modes above this line so the count stays correct.
        MATRIX_CONTROLLER_MODE_COUNT
    } ControllerMode;
    
    // Create a MatrixController which will use the given driver
    MatrixController(MatrixDriver * const driver);
    
    // Update current animation state if needed.  This method should be called
    // as frequently as possible (i.e. in an event loop) to keep the internal
    // timing as accurate as possible.
    void update();
    
    // Get/Set the current mode
    ControllerMode getMode() {return mode;}
    void setMode(ControllerMode mode);
    // Move to the next mode
    void nextMode();
    
    /***************************************************************************
     * Idle mode methods
     **************************************************************************/
    // Turn off all pixels and sit idle
    void enterIdleMode();
    
    /***************************************************************************
     * Text mode methods
     **************************************************************************/
    // Enter text mode with or without specifying new text to scroll.
    // If no text is specified, the last set text is used.
    void enterTextMode(std::string const & text);
    void enterTextMode();
    // Update scrolling text.  If the current mode is not text mode, this will
    // set the text to be used next time text mode is entered.
    // NOTE: it's recommended to add a space at the end of the string to add
    // space when the string "wraps around."
    void setText(std::string text);
    
    /***************************************************************************
     * VU meter mode methods
     **************************************************************************/
    void enterVUMode();
    
    /***************************************************************************
     * VU meter mode methods
     **************************************************************************/
    void enterSineMode();
    
private:
    // MatrixDriver to be used by this controller
    MatrixDriver * const driver;
    
    // Current state
    ControllerMode mode;
    
    // Default delay used in update*Mode methods which don't require a different
    // delay
    static long int const ANIMATION_DELAY_MS;
    
    /***************************************************************************
     * Text mode private members
     **************************************************************************/
    static std::string const TEXT_DEFAULT;
    static long int const TEXT_SCROLL_DELAY_MS;
    // Text to scroll across the matrix
    std::string scrollingText;
    // Current character entering the matrix
    std::string::const_iterator scrollingTextPosition;
    // Current column entering the matrix of the current character
    size_t currentCharPosition;
    void updateTextMode();
    
    /***************************************************************************
     * VU meter mode private members
     **************************************************************************/
    static long int const VU_UPDATE_DELAY_MS;
    void updateVUMode();
    
    /***************************************************************************
     * Sine wave mode private members
     **************************************************************************/
    void updateSineMode();
    
    /***************************************************************************
     * Misc helper methods
     **************************************************************************/
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
};

#endif