#include "menuController.hpp"
#include "esp_log.h"
#include <string.h>
#include <limits>

MenuController::MenuController(DisplayController *displayController, Storage *storage, M5Scanner &scanner, ButtonBoundary &selectButton, ButtonBoundary &startButton, ButtonBoundary &stopButton, TascamBoundary &tascamBoundary, AudioModem &audioModem, NvsBoundary &nvs, QueueHandle_t countQueue, RealTimeClock &rtc, Speaker &speaker, GpioController &gpioController)
    : currentState(State::LOGING),
      display(displayController),
      storage(storage),
      currentSelection(1),
      scanner(scanner),
      selectButton(selectButton),
      startButton(startButton),
      stopButton(stopButton),
      tascamBoundary(tascamBoundary),
      audioModem(audioModem),
      nvs(nvs),
      audioCountQueue(countQueue),
      rtc(rtc),
      speaker(speaker),
      gpioController(gpioController),
      inSession(false),
      numberScanned(false) {}

MenuController::~MenuController() {}

void MenuController::notify(ObserverId buttonId) {
    switch (buttonId) {
        case ObserverId::CodeScanner:
            patientNumber = scanner.getCode();
            display->displayText(1, (char*)patientNumber);
            numberScanned = true;
            [[fallthrough]];
        default:
            menuTask(buttonId);
            break;
    }
}

void MenuController::startRecording() {
    struct tm current_time;
    rtc.getTime(current_time);
    uint64_t sessionId = storage->getSessionId();

    if (!inSession) {
        sessionId = (sessionId < std::numeric_limits<uint64_t>::max()) ? sessionId + 1 : 0;
        storage->setSessionId(sessionId);
    }

    tascamBoundary.startRecording();
    // 1, 2, 3 are for syncing
    audioModem.transmit(1);
    audioModem.transmit(2);
    audioModem.transmit(3);
    //sesionId and time info sending
    audioModem.transmit(sessionId);
    audioModem.transmit(current_time.tm_min);
    audioModem.transmit(current_time.tm_hour);
    audioModem.transmit(current_time.tm_mday);
    audioModem.transmit(current_time.tm_mon);
    audioModem.transmit(current_time.tm_year);

    if (numberScanned && !inSession) {
        //to let the receiver know that patient number will be send
        audioModem.transmit(1);
        audioModem.transmit(patientNumber[0]);
        audioModem.transmit(patientNumber[1]);
        patientNumber = nullptr;
        numberScanned = false;
    } else {
        // Indicate no patient number is being sent
        audioModem.transmit(2);
    }
    //to let the receiver know that patient number will be send
    audioModem.transmit(3);
    inSession = true;
    speaker.beep(500);
    display->displayTextX3(2, "Rec...", false);
    vTaskDelay(pdMS_TO_TICKS(200));
}

void MenuController::menuTask(ObserverId buttonId) {
    switch (currentState) {
        case State::LOGING:
            if (scanner.checkIfCodeIsScanned()) {
                patientNumber = scanner.getCode();
                numberScanned = true;
                display->clear();
                display->displayText(2, "Patient number scanned");
                ESP_LOGI("SCANNER", "Patient number: %s", (char *)patientNumber);
                currentState = State::IDLE;
            }
            break;
        case State::IDLE:
            if (buttonId == ObserverId::Start) {
                startRecording();
                currentState = State::RECORDING;
            } else if (buttonId == ObserverId::Stop) {
                inSession = false;
                display->clear();
                display->displayText(2, "Log in patient");
                currentState = State::LOGING;
            }
            break;
        case State::RECORDING:
            if (buttonId == ObserverId::Stop) {
                tascamBoundary.stopRecording();
                display->clear();
                display->displayText(2, "Log in patient");
                currentState = State::LOGING;
                inSession = false;
                numberScanned = false;
                patientNumber = nullptr;
            } else if (buttonId == ObserverId::PatientPause) {
                gpioController.setLow();
            } else if (buttonId == ObserverId::ResearchPause) {
                gpioController.setHigh();
            }
            break;
    }
}

MenuController::State MenuController::getCurrentState() const {
    return currentState;
}
