
/**
 * @enum ButtonId
 * @brief Enumeration of button identifiers used in the observer pattern.
 * 
 * This enumeration defines the different types of buttons that can be observed
 * and identified within the system. Each button has a specific purpose related
 * to the recording functionality.
 */
#ifndef OBSERVER_HPP
#define OBSERVER_HPP


enum class ObserverId {
    Start,
    Stop,
    PatientPause,
    ResearchPause,
    CodeScanner,
    AudioDataAvailable
};


/**
 * @class Observer
 * @brief Abstract base class for implementing observer pattern.
 */
class Observer
{
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes.
     */
    virtual ~Observer() = default;

    /**
     * @brief Pure virtual function to be implemented by derived classes.
     * Called when the observed object needs to notify observers of changes.
     */
    virtual void update() = 0;

    /**
     * @brief Pure virtual function to get the button ID.
     * @return The ButtonId associated with this observer.
     */
    virtual ObserverId getId() const = 0;
};

#endif // OBSERVER_HPP
