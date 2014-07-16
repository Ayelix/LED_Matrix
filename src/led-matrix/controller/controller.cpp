#include <led-matrix/controller/controller.hpp>

#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <boost/thread/lock_guard.hpp>

#include <led-matrix/debug/debug.hpp>
#include <led-matrix/timer/timer.hpp>

MatrixController::MatrixController(MatrixDriver * const driver)
: m_driver(driver)
, m_modes(MatrixMode::MATRIX_MODE_ID_COUNT, NULL)
{
    // Initialize mode list contents
    MatrixMode::MatrixModeID id = static_cast<MatrixMode::MatrixModeID>(0);
    std::vector<MatrixMode *>::iterator iter = m_modes.begin();
    while (iter != m_modes.end())
    {
        // Create the mode for the current mode ID
        *iter = MatrixMode::createMode(id, m_driver);
        // If createMode failed, remove the list entry and move the iterator to
        // the next entry.
        if (NULL == *iter)
        {
            DBG_PRINTF("MatrixController::MatrixController(): Unable to create mode for ID %u.\n", id);
            iter = m_modes.erase(iter);
        }
        // Otherwise, move the iterator to the next entry
        else
        {
            iter++;
        }
        id = static_cast<MatrixMode::MatrixModeID>(id + 1);
    }
    
    if (!m_modes.empty())
    {
        DBG_PRINTF(
            "MatrixController::MatrixController(): Mode list initialized with %u modes.\n",
            m_modes.size());
    }
    // Controller cannot operate with an empty mode list
    else
    {
        throw std::runtime_error(
            "MatrixController::MatrixController(): Unable to create any modes");
    }
    
    // Start up in the first mode in the list (initializes m_currentModeIndex)
    setMode(0u);
}

MatrixController::~MatrixController()
{
    // Free all modes in the mode list
    std::vector<MatrixMode *>::iterator iter = m_modes.begin();
    while (iter != m_modes.end())
    {
        MatrixMode::destroyMode(*iter);
    }
}

void MatrixController::update()
{
    // Simply call update() for the current mode
    m_modes.at(m_currentModeIndex)->update();
}

void MatrixController::setMode(MatrixMode const * mode)
{   
    boost::lock_guard<boost::recursive_mutex> guard(m_mutex);
    
    // Search the mode list for the given mode
    std::vector<MatrixMode *>::iterator modeIter = 
        std::find(m_modes.begin(), m_modes.end(), mode);
    // If the mode was found in the list, get its index and call
    // setMode(unsigned int)
    if (modeIter != m_modes.end())
    {
        setMode(modeIter - m_modes.begin());
    }
    // Otherwise, use index 0
    else
    {
        DBG_PRINTF("MatrixController::setMode(MatrixMode *): mode not found.\n");
        setMode(0u);
    }
}

void MatrixController::setMode(unsigned int modeIndex)
{
    boost::lock_guard<boost::recursive_mutex> guard(m_mutex);
    
    // If modeIndex is invalid, reset it to 0
    if (modeIndex >= m_modes.size())
    {
        DBG_PRINTF("MatrixController::setMode(unsigned int): invalid mode index %u (only %u modes).\n",
            modeIndex, m_modes.size());
        modeIndex = 0;
    }
    
    // Update the current mode index and begin the new mode
    m_currentModeIndex = modeIndex;
    MatrixMode * const mode = m_modes[m_currentModeIndex];
    mode->begin();
    DBG_PRINTF("Set current mode to: %s\n", mode->getName().c_str());
}

unsigned int MatrixController::nextMode()
{
    boost::lock_guard<boost::recursive_mutex> guard(m_mutex);
    
    // Update the current mode and return the new index
    setMode((m_currentModeIndex + 1) % m_modes.size());
    return m_currentModeIndex;
}