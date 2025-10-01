/**
 * @file taskHandler.hpp
 * @brief Task management and coordination for the YOD Recorder system
 * 
 * This file defines the TaskHandler class which is responsible for managing
 * and coordinating various FreeRTOS tasks in the YOD Recorder system.
 * It handles task creation, scheduling, and communication between different
 * system components through observers and message queues.

 */

#ifndef TASK_HANDLER_HPP
#define TASK_HANDLER_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "observer.hpp"
#include <vector>

// Forward declaration
class MenuController;

/**
 * @class TaskHandler
 * @brief Manages FreeRTOS tasks and coordinates system operations
 * 
 * The TaskHandler class is responsible for creating and managing various
 * FreeRTOS tasks that handle different aspects of the YOD Recorder system.
 * It coordinates communication between tasks using the Observer pattern
 * and FreeRTOS queues.
 * 
 * Key responsibilities:
 * - Task creation and lifecycle management
 * - Observer pattern implementation for inter-task communication
 * - Queue-based message passing between tasks
 * - Integration with the menu controller for user interface operations
 * 
 * @note This class uses static task functions to comply with FreeRTOS
 *       task creation requirements.
 */
class TaskHandler {
public:
    /**
     * @brief Constructs a new TaskHandler object
     * 
     * Initializes the task handler with references to the observer list,
     * communication queue, and menu controller. These references are stored
     * for use during task creation and execution.
     * 
     * @param observers Reference to vector of Observer pointers for task communication
     * @param countQueue Reference to FreeRTOS queue handle for inter-task messaging
     * @param menuController Reference to the menu controller for UI operations
     * 
     * @note All parameters are stored as references, so the caller must ensure
     *       that the referenced objects remain valid for the lifetime of this TaskHandler.
     */
    TaskHandler(std::vector<Observer*>& observers, QueueHandle_t& countQueue, MenuController& menuController);

    /**
     * @brief Starts all managed FreeRTOS tasks
     * 
     * Creates and starts the various FreeRTOS tasks that are managed by this
     * TaskHandler. This includes the observer update task and audio analyzer task.
     * Each task is created with appropriate priority, stack size, and parameters.
     * 
     * @note This method should be called after all system components have been
     *       initialized and are ready for operation.
     * 
     * @warning Calling this method multiple times may result in duplicate tasks.
     *          Ensure it is called only once during system initialization.
     */
    void startTasks();

private:
    /**
     * @brief Reference to the vector of observers for inter-task communication
     * 
     * This vector contains pointers to all Observer objects that need to be
     * notified of system events and state changes.
     */
    std::vector<Observer*>& observers;

    /**
     * @brief Reference to the FreeRTOS queue for inter-task messaging
     * 
     * This queue is used for passing messages and data between different
     * tasks in the system, particularly for count-related information.
     */
    QueueHandle_t& countQueue;

    /**
     * @brief Reference to the menu controller for UI operations
     * 
     * The menu controller manages user interface interactions and is
     * integrated with the task system for responsive user experience.
     */
    MenuController& menuController;

    /**
     * @brief Static task function for handling observer updates
     * 
     * This static function serves as the entry point for the FreeRTOS task
     * that handles observer pattern notifications. It runs in its own task
     * context and processes observer updates as they occur.
     * 
     * @param pvParameters Pointer to task parameters (typically TaskHandler instance)
     * 
     * @note This function runs in an infinite loop and should never return.
     *       It implements the FreeRTOS task function signature requirement.
     */
    static void observerUpdateTask(void *pvParameters);

    /**
     * @brief Static task function for audio analysis operations
     * 
     * This static function serves as the entry point for the FreeRTOS task
     * that handles audio analysis operations. It processes audio data and
     * performs analysis tasks in its own execution context.
     * 
     * @param pvParameters Pointer to task parameters (typically TaskHandler instance)
     * 
     * @note This function runs in an infinite loop and should never return.
     *       It implements the FreeRTOS task function signature requirement.
     */
    static void audioAnalyzerTask(void *pvParameters);
};

#endif // TASK_HANDLER_HPP
