/**
 * @file audioAnalyzerController.cpp
 * @brief Implementation of the AudioAnalyzerController class
 * @author YOD Recorder Team
 * @date 2025
 */

#include "audioAnalyzerController.hpp"
#include <esp_log.h>

const char* AudioAnalyzerController::TAG = "AudioAnalyzerCtrl";

AudioAnalyzerController::AudioAnalyzerController(QueueHandle_t countQueue)
    : countQueue(countQueue), taskHandle(NULL) {
    // Initialize the audio analyzer
    audioAnalyzer.init();
    ESP_LOGI(TAG, "AudioAnalyzerController initialized.");
}

AudioAnalyzerController::~AudioAnalyzerController() {
    if (taskHandle != NULL) {
        ESP_LOGI(TAG, "Deleting audio analyzer task.");
        vTaskDelete(taskHandle);
        taskHandle = NULL;
    }
    ESP_LOGI(TAG, "AudioAnalyzerController destroyed.");
}

bool AudioAnalyzerController::startTask(const char *taskName, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId) {
    if (taskHandle != NULL) {
        ESP_LOGW(TAG, "Task already started.");
        return false;
    }

    BaseType_t result;
    if (coreId == tskNO_AFFINITY) {
        result = xTaskCreate(
            taskRunner,     // Function to implement the task
            taskName,       // Name of the task
            stackSize,      // Stack size in words
            this,           // Task input parameter (pointer to this instance)
            priority,       // Priority of the task
            &taskHandle     // Task handle
        );
    } else {
        result = xTaskCreatePinnedToCore(
            taskRunner,
            taskName,
            stackSize,
            this,
            priority,
            &taskHandle,
            coreId
        );
    }

    if (result != pdPASS) {
        ESP_LOGE(TAG, "Failed to create audio analyzer task. Error code: %d", result);
        taskHandle = NULL;
        return false;
    }

    ESP_LOGI(TAG, "Audio analyzer task '%s' created successfully.", taskName);
    return true;
}

void AudioAnalyzerController::taskRunner(void *pvParameters) {
    AudioAnalyzerController *controller = static_cast<AudioAnalyzerController *>(pvParameters);
    if (controller) {
        controller->runAnalysis();
    }
    // Should not reach here if runAnalysis is an infinite loop,
    // but good practice to delete task if it exits.
    ESP_LOGI(TAG, "Audio analyzer task runner exiting.");
    vTaskDelete(NULL);
}

void AudioAnalyzerController::runAnalysis() {
    uint8_t count = 0;
    uint8_t i = 0;
    TickType_t lastWakeTime = xTaskGetTickCount(); // Initialize lastWakeTime

    ESP_LOGI(TAG, "Audio analysis loop started.");

    while (1) {
        if ((xTaskGetTickCount() - lastWakeTime) >= pdMS_TO_TICKS(1500)) {
            audioAnalyzer.sampleInput();
            audioAnalyzer.computeFft();
            count += audioAnalyzer.isWord();
            lastWakeTime = xTaskGetTickCount(); // Update lastWakeTime after processing
            i += 1;
        }

        if (i >= 9) {
            ESP_LOGI(TAG, "Analysis cycle complete. Count = %d. Sending to queue.", count);
            if (countQueue != NULL) {
                if (xQueueSend(countQueue, &count, pdMS_TO_TICKS(100)) != pdPASS) {
                    ESP_LOGE(TAG, "Failed to send count to queue.");
                } else {
                    ESP_LOGI(TAG, "Sent count %d to queue.", count);
                }
            } else {
                ESP_LOGE(TAG, "CountQueue handle is NULL. Cannot send data.");
            }
            i = 0;
            count = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks
    }
}