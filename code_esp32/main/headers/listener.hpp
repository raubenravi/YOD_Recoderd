#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "observer.hpp" // Include to get ButtonId enum

/**
 * @class Listener
 * @brief Abstract base class for handling button events.
 * 
 * This class provides an interface for objects that want to listen to button events.
 * Classes that inherit from Listener must implement the notify method.
 */
class Listener
{
public:
    /**Listener
     * @brief Virtual destructor to ensure proper cleanup in derived classes.
     */
    virtual ~Listener() = default;

    /**
     * @brief Pure virtual method called when a button event occurs.
     * 
     * This method must be implemented by derived classes to handle button events.
     * @param buttonId The identifier of the button that was pressed
     */
    virtual void notify(ObserverId buttonId) = 0;
};

#endif // LISTENER_HPP
