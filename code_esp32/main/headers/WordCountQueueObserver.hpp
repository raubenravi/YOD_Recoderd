#ifndef WORD_COUNT_QUEUE_OBSERVER_HPP
#define WORD_COUNT_QUEUE_OBSERVER_HPP

#include "Observer.hpp"
#include "listener.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

/**
 * @class WordCountQueueObserver
 * @brief An observer that checks for data in the word count queue.
 */
class WordCountQueueObserver : public Observer {
public:
    /**
     * @brief Construct a new Word Count Queue Observer object.
     * @param queue The handle to the word count queue to be observed.
     */
    explicit WordCountQueueObserver(QueueHandle_t queue);

    /**
     * @brief Sets the listener to be notified when data is available.
     * @param listener Reference to the listener object.
     */
    void setListener(Listener& listener);

    /**
     * @brief Checks if data is available in the queue when notified.
     */
    void update() override;

    /**
     * @brief Gets the identifier for this observer.
     * @return The ButtonId for audio data availability.
     */
    ObserverId getId() const override;

private:
    QueueHandle_t wordCountQueue; ///< Handle to the word count queue.
    Listener* listener; ///< Pointer to the listener to notify when data is available.
};

#endif // WORD_COUNT_QUEUE_OBSERVER_HPP
