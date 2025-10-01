#include <stdio.h>
#include <memory>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "audioModem.hpp"
#include "menuController.hpp"
#include "buttonBoundary.hpp"
#include "m5Scanner.hpp"
#include "tascamBoundary.hpp"
#include "audioAnalyzer.hpp"
#include "displayController.hpp"
#include "RealTimeClock.hpp"
#include "taskHandler.hpp"
#include "headers/WordCountQueueObserver.hpp"
#include "gpioController.hpp"
#include "headers/hardware_config.hpp"

extern "C" void app_main(void) {
    ESP_LOGI("YOD_RECORDER", "Starting initialization...");

    // GPIO Service and Controller
    gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
    GpioController gpioController;
    gpioController.initialize();
    gpioController.setGpio2Level(0);

    // Buttons
    ButtonBoundary buttonResearch(BUTTON_RESEARCH_PIN, ObserverId::ResearchPause, GPIO_INTR_POSEDGE);
    ButtonBoundary buttonSelect(BUTTON_SELECT_PIN, ObserverId::Start, GPIO_INTR_POSEDGE);
    ButtonBoundary buttonStop(BUTTON_STOP_PIN, ObserverId::Stop, GPIO_INTR_POSEDGE);
    ButtonBoundary buttonPatient(BUTTON_PATIENT_PIN, ObserverId::PatientPause, GPIO_INTR_NEGEDGE);
    buttonResearch.initialize();
    buttonSelect.initialize();
    buttonStop.initialize();
    buttonPatient.initialize();

    // Queue
    QueueHandle_t countQueue = xQueueCreate(5, sizeof(uint8_t));

    // Display and RTC
    SSD1306_t display_dev;
    i2c_master_init(&display_dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    auto display = std::make_unique<DisplayController>(&display_dev, countQueue);
    RealTimeClock rtcClock(display_dev._i2c_bus_handle);
    rtcClock.initialize();

    // Peripherals
    M5Scanner scanner(display_dev._i2c_bus_handle);
    scanner.initialize();
    AudioModem modem(23000, 21000, AUDIO_MODEM_PIN);
    modem.initialize();
    TascamBoundary tascamBoundary(TASCAM_UART_NUM, new QueueHandle_t);
    tascamBoundary.initialize();

    // Observer and Speaker
    WordCountQueueObserver wordCountQueueObserver(countQueue);
    Speaker speaker(SPEAKER_PIN);
    speaker.initialize();

    // Menu Controller
    NvsBoundary nvsBoundaryInstance;
    auto storage = std::make_unique<Storage>(nvsBoundaryInstance);
    auto menu = std::make_unique<MenuController>(
        display.get(), storage.get(), scanner, buttonSelect, buttonPatient, buttonStop,
        tascamBoundary, modem, nvsBoundaryInstance , countQueue, rtcClock, speaker, gpioController
    );

    // Set Listeners
    buttonSelect.setListener(*menu);
    buttonStop.setListener(*menu);
    buttonPatient.setListener(*menu);
    buttonResearch.setListener(*menu);
    wordCountQueueObserver.setListener(*display);

    // Start Tasks
    //TODO nog naar array veranderen
    std::vector<Observer*> observers = {&buttonPatient, &buttonSelect, &buttonStop, &buttonResearch, &wordCountQueueObserver}; 
    TaskHandler taskHandler(observers, countQueue, *menu);
    taskHandler.startTasks();

    ESP_LOGI("YOD_RECORDER", "Initialization complete. Starting main loop...");
    while (true) {
        vTaskDelay(portMAX_DELAY);
    }
}