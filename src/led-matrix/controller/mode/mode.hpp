/* mode.hpp - abstract class to represent modes of operation for the matrix.
 */
#ifndef _MATRIX_MODE_H
#define _MATRIX_MODE_H

#include <string>
#include <vector>

#include <led-matrix/driver/driver.hpp>
#include <led-matrix/controller/mode/setting/setting.hpp>

class MatrixMode
{
public:
    // All available mode types
    typedef enum
    {
        // Idle/off state - all pixels off
        MATRIX_MODE_ID_IDLE, //MatrixModeIdle
        // Scrolling text mode
        MATRIX_MODE_ID_TEXT, //MatrixModeText
        // VU meter mode
        MATRIX_MODE_ID_VU, //MarixModeVu
        // Sine wave mode
        MATRIX_MODE_ID_SINE, //MatrixModeSine
        
        // Add new modes above so the count remains correct
        MATRIX_MODE_ID_COUNT //not a valid mode
    } MatrixModeID;
    
    // Get the ID of a mode
    MatrixModeID getID() const {return m_id;};
    
    // Get the name/description of a mode
    std::string getName() const {return m_nameStr;}
    std::string getDescription() const {return m_descriptionStr;}
    
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
    
    // Get the settings for a mode
    std::vector<MatrixSetting *> const & getSettings() const
        {return m_settings;}
    
    // Allocate memory for and create a new mode of the given type which will
    // use the given driver.
    // Returns NULL or throws std::runtime_error or std::invalid_argument on
    // failure.
    // The returned pointer should be freed with destroyMode().
    static MatrixMode * createMode(MatrixModeID id, MatrixDriver * driver);
    
    // Destroy the given mode, freeing memory allocated by createMode().
    // Throws std::runtime_error or std::invalid_argument on failure.
    static void destroyMode(MatrixMode * mode);
    
    // Virtual destructor as recommended by:
    // http://stackoverflow.com/a/318137
    virtual ~MatrixMode();
    
protected:
    // Initialize a mode of the given type with the given update delay.
    // Constructor is protected because createMode() should be used to create
    // the proper implementation for a mode.
    MatrixMode(MatrixModeID id, std::string name, std::string description,
        long int delayMs, MatrixDriver * driver);
    
    // Returns true if this mode needs to be updated (i.e. if the update delay
    // has elapsed since the last check).  It is expected that this method will
    // be called from update().
    bool needsUpdate();
    
    // Driver updated by this mode
    MatrixDriver * const m_driver;
    
    // List of settings for this mode
    // Initialized to empty in constructor and MatrixSetting::destroySetting is
    // called on all elements in destructor.
    std::vector<MatrixSetting *> m_settings;
    
    // Plot the given level (in the range 0-100) on the matrix, using the given
    // plot type/style and optionally setting all pixels below the given level.
    typedef enum
    {
        // Bar-graph type plot going from bottom to top
        MATRIX_MODE_PLOT_TYPE_VERTICAL,
        // Bar-graph type plot going from left to right
        MATRIX_MODE_PLOT_TYPE_HORIZONTAL,
        // Shift matrix left and plot the new level on the rightmost column
        MATRIX_MODE_PLOT_TYPE_SHIFTING,
        
        MATRIX_MODE_PLOT_TYPE_COUNT
    } PlotType;
    void plotLevel(unsigned int level,
        PlotType type = MATRIX_MODE_PLOT_TYPE_VERTICAL,
        bool fill = false);
    
private:
    // The ID of this mode
    MatrixModeID m_id;
    // String name of this mode
    std::string const m_nameStr;
    // String description of this mode
    std::string const m_descriptionStr;
    // Update delay for this mode (milliseconds)
    long int const m_delayMs;
    
    // Check an ID and throw an invalid_argument exception if it's invalid.
    // Error string is of the format:
    // "<prefix>: Invalid mode ID <id>"
    static void checkID(MatrixModeID m_id, std::string const & prefix);
};

#endif