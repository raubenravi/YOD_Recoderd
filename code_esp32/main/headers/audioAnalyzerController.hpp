/**
 * @file audioAnalyzerController.hpp
 * @brief Header file for the AudioAnalyzerController class
 * @author YOD Recorder Team  
 * @date 2025
 */

#ifndef AUDIO_ANALYZER_CONTROLLER_HPP
#define AUDIO_ANALYZER_CONTROLLER_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "audioAnalyzer.hpp" // Assumes audioAnalyzer.hpp defines the AudioAnalyzer class
#include "esp_log.h"

/**
 * @class AudioAnalyzerController
 * @brief A controller class for managing the AudioAnalyzer and its FreeRTOS task.
 */
class AudioAnalyzerController {
public:
    /**
     * @brief Constructor for the AudioAnalyzerController class.
     * 
     * @param countQueue Queue handle to send data to.
     */
    AudioAnalyzerController(QueueHandle_t countQueue);

    /**
     * @brief Destructor for the AudioAnalyzerController class.
     */
    ~AudioAnalyzerController();

    /**
     * @brief Starts the audio analyzer task.
     * 
     * @param taskName Name for the FreeRTOS task. Default is "audio_analyzer_ctrl_task".
     * @param stackSize Stack size for the task. Default is 4096.
     * @param priority Priority of the task. Default is 5.
     * @param coreId Core to pin the task to. Default is tskNO_AFFINITY (no affinity).
     * @return True if the task was successfully started, false otherwise.
     */
    bool startTask(const char *taskName = "audio_analyzer_ctrl_task",
                   uint32_t stackSize = 4096,
                   UBaseType_t priority = 5,
                   BaseType_t coreId = tskNO_AFFINITY);

private:
    /**
     * @brief Static wrapper function to be called by FreeRTOS task creation.
     * 
     * @param pvParameters Pointer to the parameters passed to the task.
     */
    static void taskRunner(void *pvParameters);

    /**
     * @brief Member function containing the actual task logic.
     */
    void runAnalysis();

    QueueHandle_t countQueue;        /**< Queue to send data to. */
    AudioAnalyzer audioAnalyzer;     /**< Instance of the audio analyzer. */
    TaskHandle_t taskHandle;         /**< Handle for the created task. */
    static const char* TAG;          /**< Logging tag. */
};

#endif // AUDIO_ANALYZER_CONTROLLER_HPP