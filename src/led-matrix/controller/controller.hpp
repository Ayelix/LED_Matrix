/* matrix-controller - class to handle high-level control of the matrix, like
 * animations, timing, etc.
 */
#ifndef _MATRIX_CONTROLLER_H
#define _MATRIX_CONTROLLER_H

#include <led-matrix/driver/driver.hpp>
#include <led-matrix/controller/mode/mode.hpp>

#include <string>
#include <vector>
#include <boost/thread/recursive_mutex.hpp>

class MatrixController
{
public:
    // Create a MatrixController which will use the given driver
    MatrixController(MatrixDriver * const driver);
    
    //TODO: destructor must free all mode pointers
    
    // Update the current mode if needed.  This method should be called
    // as frequently as possible (i.e. in an event loop) to keep the internal
    // timing as accurate as possible.
    // NOT thread-safe
    void update();
    
    // Get the current mode
    // Thread-safe
    MatrixMode * getMode() const {return m_modes.at(m_currentModeIndex);}
    // Get the index of the current mode
    // Thread-safe
    unsigned int getModeIndex() const {return m_currentModeIndex;}
    // Set the current mode by a pointer to the mode itself
    // Thread-safe
    void setMode(MatrixMode const * mode);
    // Set the current mode by index in the list provided by getModes()
    // Thread-safe
    void setMode(unsigned int modeIndex);
    // Move to the next mode.  Returns the index of the new mode in the list
    // provided by getModes()
    // Thread-safe
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
    
    // Mutex to provide coarse-grained thread-safety
    boost::recursive_mutex m_mutex;
};

#endif