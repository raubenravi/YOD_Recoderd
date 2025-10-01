#include "WordCountQueueObserver.hpp"
#include "esp_log.h"

static const char* TAG = "WordCountQueueObserver";

WordCountQueueObserver::WordCountQueueObserver(QueueHandle_t queue) : wordCountQueue(queue), listener(nullptr) {}

void WordCountQueueObserver::setListener(Listener& listener) {
    this->listener = &listener;
}

void WordCountQueueObserver::update() {
    if (uxQueueMessagesWaiting(wordCountQueue) > 0) {
        ESP_LOGI(TAG, "Data is available in the word count queue.");
        // Notify the listener if one is set
        if (listener != nullptr) {
            listener->notify(ObserverId::AudioDataAvailable);
        }
        // Add logic here to handle the available data.
    } else {
        // ESP_LOGI(TAG, "No data in the word count queue.");
    }
}

ObserverId WordCountQueueObserver::getId() const {
    return ObserverId::AudioDataAvailable;
}
