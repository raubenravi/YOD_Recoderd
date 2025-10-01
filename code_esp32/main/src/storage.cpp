#include "storage.hpp"
#include <iostream>
#include <cstdint>
#include <nvs_flash.h>

#define STORAGE_NAMESPACE "storage"
#define SESSION_ID_KEY "session_id"

// Constructor for NvsBoundary
NvsBoundary::NvsBoundary() { 
    nvs_flash_init();
}

// Destructor for NvsBoundary
NvsBoundary::~NvsBoundary() {
    // Cleanup code for NvsBoundary
}

// Write uint64_t to NVS
void NvsBoundary::writeUint64(const char* key, uint64_t value) {
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK) {
        std::cerr << "Failed to open NVS: " << esp_err_to_name(err) << std::endl;
        return;
    }

    err = nvs_set_u64(nvsHandle, key, value);
    if (err != ESP_OK) {
        std::cerr << "Failed to write data: " << esp_err_to_name(err) << std::endl;
    } else {
        nvs_commit(nvsHandle);
    }

    nvs_close(nvsHandle);
}

// Read uint64_t from NVS
uint64_t NvsBoundary::readUint64(const char* key, uint64_t defaultValue) {
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &nvsHandle);
    if (err != ESP_OK) {
        std::cerr << "Failed to open NVS: " << esp_err_to_name(err) << std::endl;
        return defaultValue;
    }

    uint64_t value = defaultValue;
    err = nvs_get_u64(nvsHandle, key, &value);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        std::cerr << "Key not found in NVS: " << key << std::endl;
        value = defaultValue;
    } else if (err != ESP_OK) {
        std::cerr << "Failed to read data: " << esp_err_to_name(err) << std::endl;
        value = defaultValue;
    }

    nvs_close(nvsHandle);
    return value;
}

// Erase data from NVS
void NvsBoundary::eraseData(const char* key) {
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK) {
        std::cerr << "Failed to open NVS: " << esp_err_to_name(err) << std::endl;
        return;
    }

    err = nvs_erase_key(nvsHandle, key);
    if (err != ESP_OK) {
        std::cerr << "Failed to erase data: " << esp_err_to_name(err) << std::endl;
    } else {
        nvs_commit(nvsHandle);
    }

    nvs_close(nvsHandle);
}

// Constructor for Storage, injecting NVS_boundary_abstract
Storage::Storage(NvsBoundaryAbstract& nvsBoundary) : nvsService(nvsBoundary) {
    // Initialize other members if necessary
}

// Set session ID
void Storage::setSessionId(uint64_t sessionId) {
    if (sessionId >= 254) {
        sessionId = 0;
    }
    nvsService.writeUint64(SESSION_ID_KEY, sessionId);
}

// Get session ID
uint64_t Storage::getSessionId() {
    return nvsService.readUint64(SESSION_ID_KEY, 0);
}

// Clear session ID
void Storage::clearSessionId() {
    nvsService.eraseData(SESSION_ID_KEY);
}
