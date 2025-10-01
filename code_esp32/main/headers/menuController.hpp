#ifndef MENU_CONTROLLER_HPP
#define MENU_CONTROLLER_HPP

#include "displayController.hpp"
#include "driver/gpio.h"
#include "storage.hpp"
#include "m5Scanner.hpp"
#include "buttonBoundary.hpp"
#include "tascamBoundary.hpp"
#include "audioModem.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "string"
#include "RealTimeClock.hpp"
#include "speaker.hpp"
#include "gpioController.hpp"

constexpr struct StatusStrings {
    const char *LOGING = "Loging";
    const char *IDLE = "Idle";
    const char *RECORDING = "Recording";
} statusStrings;

class MenuController : public Listener
{
    public:
        enum class State
        {
            LOGING,
            IDLE,
            RECORDING
        };
        /**
         * @brief Constructs a MenuController object.
         * @param displayController Pointer to the DisplayController object.
         * @param storage Pointer to the Storage object.
         * @param scanner Reference to the M5Scanner object.
         * @param selectButton Reference to the select ButtonBoundary.
         * @param startButton Reference to the start ButtonBoundary.
         * @param stopButton Reference to the stop ButtonBoundary.
         * @param tascamBoundary Reference to the TascamBoundary object.
         * @param audioModem Reference to the AudioModem object.
         * @param nvs Reference to the NvsBoundary object.
         * @param countQueue Queue handle for audio count.
         * @param rtc Reference to the RealTimeClock object.
         * @param speaker Reference to the Speaker object.
         * @param gpioController Reference to the GpioController object.
         */
        MenuController(DisplayController *displayController, Storage *storage, M5Scanner &scanner, ButtonBoundary &selectButton, ButtonBoundary &startButton, ButtonBoundary &stopButton, TascamBoundary &tascamBoundary, AudioModem &audioModem ,NvsBoundary & nvs, QueueHandle_t countQueue, RealTimeClock &rtc, Speaker &speaker, GpioController &gpioController);

        /**
         * @brief Destructor for MenuController.
         */
        ~MenuController();

        /**
         * @brief Main menu task loop. Handles user input and menu navigation.
         */
        void menuTask(ObserverId buttonId);

        void notify(ObserverId buttonId) override;

        /**
         * @brief Gets the current state of the menu controller.
         * @return Current state.
         */
        State getCurrentState() const;

    private:
       

        State currentState; ///< Current state of the menu controller.

        DisplayController *display;       ///< Pointer to the display controller.
        Storage *storage;                 ///< Pointer to the storage object.
        int currentSelection = 1;        ///< Current menu selection index.
        M5Scanner & scanner;              ///< Reference to the scanner object.
        ButtonBoundary &selectButton;    ///< Reference to the select button.
        ButtonBoundary &startButton;     ///< Reference to the start button.
        ButtonBoundary &stopButton;      ///< Reference to the stop button.
        TascamBoundary & tascamBoundary; ///< Reference to the Tascam boundary.
        AudioModem &audioModem;          ///< Reference to the AudioModem object.
        NvsBoundary nvs;                 ///< NVS boundary object.
        QueueHandle_t audioCountQueue;  ///< Queue handle for audio count.
        RealTimeClock &rtc;             ///< Reference to the RealTimeClock object.
        Speaker &speaker;               ///< Reference to the Speaker object.
        GpioController &gpioController; ///< Reference to the GpioController object.

        void startRecording();
        bool inSession = false;
        uint8_t *patientNumber = nullptr;
        bool numberScanned = false; ///< Flag to indicate if a number has been scanned
};

#endif // MENU_CONTROLLER_HPP