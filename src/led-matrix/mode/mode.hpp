/* matrix-mode - abstract class to represent modes of operation for the matrix.
 */
#ifndef _MATRIX_MODE_H
#define _MATRIX_MODE_H

#include <string>

#include <led-matrix/driver/driver.hpp>

class MatrixMode
{
public:
    // All available mode types
    typedef enum
    {
        // Idle/off state - all pixels off
        MATRIX_MODE_ID_IDLE, //MatrixModeIdle
        // Scrolling text mode
        MATRIX_MODE_ID_TEXT, //not yet implemented
        // VU meter mode
        MATRIX_MODE_ID_VU, //not yet implemented
        // Sine wave mode
        MATRIX_MODE_ID_SINE, //not yet implemented
        
        // Add new modes above so the count remains correct
        MATRIX_MODE_ID_COUNT //not a valid mode
    } MatrixModeID;
    
    // Get the ID of a mode
    MatrixModeID getID() {return m_id;};
    
    // Enter this mode.  Clears the matrix and performs all necessary setup to
    // begin updating this mode via update().
    // If this method is overridden, MatrixMode::begin() must be called by the
    // subclass method.
    virtual void begin();
    
    // Update the animation for this mode.  Should be called as frequently as
    // possible to keep internal timing as accurate as possible.
    // It is expected that needsUpdate() will be called by implementations of
    // this method to determine if it is time for the animation to be updated.
    virtual void update() = 0;
    
    // TODO: Get the settings for a mode
    // std::list<MatrixModeSetting> const & getSettings();
    
    // Allocate memory for and create a new mode of the given type which will
    // use the given driver.
    // The returned pointer should be freed with destroyMode().
    static MatrixMode * createMode(MatrixModeID id, MatrixDriver * driver);
    
    // Destroy the given mode, freeing memory allocated by createMode().
    static void destroyMode(MatrixMode * mode);
    
protected:
    // Initialize a mode of the given type with the given update delay.
    // Constructor is protected because createMode() should be used to create
    // the proper implementation for a mode.
    MatrixMode(MatrixModeID id, long int delayMs, MatrixDriver * driver);
    
    // Returns true if this mode needs to be updated (i.e. if the update delay
    // has elapsed since the last check).  It is expected that this method will
    // be called from update().
    bool needsUpdate();
    
    // Virtual destructor as recommended by:
    // http://stackoverflow.com/a/318137
    virtual ~MatrixMode() {};
    
private:
    // The ID of this mode
    MatrixModeID m_id;
    // Update delay for this mode (milliseconds)
    long int const m_delayMs;
    // Driver updated by this mode
    MatrixDriver * const m_driver;
    
    // TODO: List of settings for this mode
    // std::list<MatrixModeSetting *> m_settings;
    
    // Check an ID and throw an invalid_argument exception if it's invalid.
    // Error string is of the format:
    // "<prefix>: Invalid mode ID <id>"
    static void checkID(MatrixModeID m_id, std::string const & prefix);
};

#endif