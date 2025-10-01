#include "taskHandler.hpp"
#include "esp_log.h"
#include "audioAnalyzer.hpp" // Assuming this is the correct path
#include "menuController.hpp"
#include "esp_timer.h"

// Define TAG for logging
static const char *TAG = "TaskHandler";

TaskHandler::TaskHandler(std::vector<Observer*>& observers, QueueHandle_t& countQueue, MenuController& menuController)
    : observers(observers), countQueue(countQueue), menuController(menuController) {
}

void TaskHandler::startTasks() {
    xTaskCreatePinnedToCore(
        observerUpdateTask,
        "ObserverUpdateTask",
        4096,
        this,
        5,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        audioAnalyzerTask,
        "AudioAnalyzerTask",
        4096,
        this,
        5,
        NULL,
        1
    );
}

void TaskHandler::observerUpdateTask(void *pvParameters) {
    TaskHandler* taskHandler = static_cast<TaskHandler*>(pvParameters);
    while (true) {
        for (Observer* obs : taskHandler->observers) {
            obs->update();
            // Small delay between observer updates to prevent blocking
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        // Longer delay to ensure other tasks can run
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void TaskHandler::audioAnalyzerTask(void *param)
{
    TaskHandler* taskHandler = static_cast<TaskHandler*>(param);
    QueueHandle_t queue = taskHandler->countQueue;
    MenuController& menuController = taskHandler->menuController;
    
    AudioAnalyzer audioAnalyzer;
    audioAnalyzer.init();    

    uint8_t count = 0;
    uint8_t i = 0;
    TickType_t lastWakeTime = xTaskGetTickCount(); // Initialize lastWakeTime
    int64_t startTime = 0;
    uint8_t consecutiveWords = 0;
    TickType_t lastLogTime = xTaskGetTickCount();

    ESP_LOGI(TAG, "Audio analysis loop started.");
    
    while (1)
    {
        // Only analyze audio when in RECORDING state
        if (menuController.getCurrentState() == MenuController::State::RECORDING) {
            if (i == 0) {
                startTime = esp_timer_get_time();
            }
            
            if ((xTaskGetTickCount() - lastWakeTime) >= pdMS_TO_TICKS(250)) {
                lastWakeTime = xTaskGetTickCount();
                audioAnalyzer.sampleInput();
                audioAnalyzer.computeFft();
                // audioAnalyzer.printResults();
                
                if(audioAnalyzer.isWord()){
                    consecutiveWords++;
                } else {
                    consecutiveWords = 0;
                }

                if(consecutiveWords == 2){
                    count++;
                    consecutiveWords = 0; // Reset after counting
                }
                // Update lastWakeTime after processing
                i += 1;
            }

            // Log ratio every 10 seconds
            if ((xTaskGetTickCount() - lastLogTime) >= pdMS_TO_TICKS(10000)) {
                float currentRatio = (i > 0) ? (float)count / i * 2 : 0;
                ESP_LOGI(TAG, "Intermediate Ratio: %.2f after %d samples", currentRatio, i);
                lastLogTime = xTaskGetTickCount();
            }

            if (i >= 240) {
                int64_t endTime = esp_timer_get_time();
                float ratio = (i > 0) ? (float)count / i * 2 : 0;
                ESP_LOGI(TAG, "Analysis cycle complete. Count = %d, Samples = %d, Ratio = %.2f, Time = %lld ms", count, i, ratio, (endTime - startTime) / 1000);
                
                // Send count to queue
                if (queue != NULL) { 
                    if (xQueueSend(queue, &count, pdMS_TO_TICKS(100)) != pdPASS) {
                        ESP_LOGE(TAG, "Failed to send count to queue from audio_analyzer_task");
                    } else {
                        ESP_LOGI(TAG, "Audio_analyzer_task: Sent count %d to queue", count);
                    }
                } else {
                    ESP_LOGE(TAG, "Audio_analyzer_task: count_queue handle is NULL.");
                }
                
                i = 0;
                count = 0;
                consecutiveWords = 0;
            }
        } else {
            // Reset counters when not recording
            i = 0;
            count = 0;
            consecutiveWords = 0;
            lastWakeTime = xTaskGetTickCount();
            lastLogTime = xTaskGetTickCount();
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks
    }
}
