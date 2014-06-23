/* matrix-mode - abstract class to represent modes of operation for the matrix.
 */
#ifndef _MATRIX_MODE_H
#define _MATRIX_MODE_H

class MatrixMode
{
public:
    // All available mode types
    typedef enum
    {
        // Idle/off state - all pixels off
        MATRIX_MODE_ID_IDLE,
        // Scrolling text mode
        MATRIX_MODE_ID_TEXT,
        // VU meter mode
        MATRIX_MODE_ID_VU,
        // Sine wave mode
        MATRIX_MODE_ID_SINE,
        
        // Add new modes above so the count remains correct (not a valid mode)
        MATRIX_MODE_ID_COUNT
    } MatrixModeID;
    
    // Get the ID of a mode
    MatrixModeID getID() {return m_id;};
    
    // TODO: Get the settings for a mode
    // std::list<MatrixModeSetting> const & getSettings();
    
    // Allocate memory for and create a new mode of the given type.  The
    // returned pointer should be freed with destroyMode().
    static MatrixMode * createMode(MatrixModeID id);
    
    // Destroy the given mode, freeing memory allocated by createMode().
    static void destroyMode(MatrixMode * mode);
    
protected:
    // Initialize a mode of the given type.  Constructor is protected because
    // createMode() should be used to create the proper implementation for a
    // mode.
    MatrixMode(MatrixModeID id);
    
    // Virtual destructor as recommended by:
    // http://stackoverflow.com/a/318137
    virtual ~MatrixMode() {};
    
private:
    // The ID of this mode
    MatrixModeID m_id;
    // TODO: List of settings for this mode
    // std::list<MatrixModeSetting> m_settings;
};

#endif