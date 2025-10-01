// m5ScannerController.cpp
#include "m5ScannerController.hpp"

M5ScannerController::M5ScannerController(M5Scanner& scanner) 
    : scanner(scanner) {
}

M5ScannerController::~M5ScannerController() {
}

esp_err_t M5ScannerController::init() {
    scanner.setTriggerMode(1); // Set to continuous mode
    return ESP_OK;
}

char* M5ScannerController::getCode() {
    //scannedData data;
    return (char*)scanner.getCode();
}

void M5ScannerController::update() {
    if (scanner.checkIfCodeIsScanned()) {
       // uint8_t* code = scanner.getCode();
        if (scanner.checkIfCodeIsScanned()) {
            listener->notify(ObserverId::CodeScanner);
        }
    }
}

void M5ScannerController::setListener(Listener* listener) {
    this->listener = listener;
}

ObserverId M5ScannerController::getId() const {
    return ObserverId::CodeScanner;
}

