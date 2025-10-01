// m5ScannerController.hpp
#ifndef M5_SCANNER_CONTROLLER_HPP
#define M5_SCANNER_CONTROLLER_HPP

#include "m5Scanner.hpp"
#include "esp_log.h"
#include "esp_err.h"
#include <string>
#include "observer.hpp"
#include "Listener.hpp"



/**
 * @class M5ScannerController
 * @brief Simple controller class for managing M5Scanner operations
 * 
 * This class provides a simplified interface for controlling the M5Scanner.
 * It implements the Observer pattern to handle scanner events and notifies
 * registered listeners when QR codes are successfully scanned.
 * 
 * The controller manages the scanner lifecycle including initialization,
 * code retrieval, and event handling. It acts as an intermediary between
 * the low-level M5Scanner hardware interface and higher-level application logic.
 */
class M5ScannerController : public Observer {
private:
    /** @brief Reference to the M5Scanner instance */
    M5Scanner& scanner;
    
    /** @brief Pointer to the listener that receives scanner notifications */
    Listener* listener;

public:
    /**
     * @brief Constructor for M5ScannerController
     * @param scanner Reference to M5Scanner instance to be controlled
     */
    M5ScannerController(M5Scanner& scanner);
    
    /**
     * @brief Destructor for M5ScannerController
     * 
     * Cleans up resources and ensures proper shutdown of scanner operations.
     */
    ~M5ScannerController();
    
    /**
     * @brief Initialize the scanner
     * @return esp_err_t Error status (ESP_OK on success)
     * 
     * Sets up the scanner in continuous trigger mode and prepares it for operation.
     */
    esp_err_t init();
    
    /**
     * @brief Get the scanned QR code data
     * @return Pointer to character array containing the scanned data, or nullptr if no code available
     * 
     * Retrieves the most recently scanned QR code data from the scanner.
     * The returned pointer should not be freed by the caller.
     */
    char* getCode();

    /**
     * @brief Set the listener for scanner events
     * @param listener Pointer to the listener that will receive notifications
     */
    void setListener(Listener* listener);

    /**
     * @brief Get the unique identifier for this controller
     * @return ButtonId The unique identifier (CodeScanner)
     */
    ObserverId getId() const override;
    
    /**
     * @brief Update method called when scanner state changes
     * 
     * This method is called as part of the Observer pattern when the scanner
     * detects a new QR code. It notifies the registered listener if a code
     * has been successfully scanned.
     */
    void update() override;


};

#endif // M5_SCANNER_CONTROLLER_HPP
