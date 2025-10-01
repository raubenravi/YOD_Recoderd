/**
 * @file storage.hpp
 * @brief Header file for the Storage and NvsBoundary classes.
 *
 * This file contains the declaration of classes for managing non-volatile storage (NVS).
 * It provides an abstraction layer for NVS operations and a storage class for session management.
 */

/**
 * @class NvsBoundaryAbstract
 * @brief Abstract base class for NVS operations.
 *
 * This class defines the interface for reading, writing, and erasing data in non-volatile storage.
 */

/**
 * @class NvsBoundary
 * @brief Concrete implementation of NvsBoundaryAbstract.
 *
 * This class provides the functionality to interact with the NVS, including writing,
 * reading, and erasing data using the ESP-IDF NVS API.
 */

/**
 * @class Storage
 * @brief A class for managing session data in non-volatile storage.
 *
 * The Storage class uses an NvsBoundaryAbstract instance to manage session IDs
 * and provides methods to set, get, and clear the session ID.
 */

#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <cstdint>
#include <nvs.h>

class NvsBoundaryAbstract {
public:
    virtual ~NvsBoundaryAbstract() = default;
    virtual void writeUint64(const char* key, uint64_t value) = 0;
    virtual uint64_t readUint64(const char* key, uint64_t defaultValue = 0) = 0;
    virtual void eraseData(const char* key) = 0;
};

class NvsBoundary : public NvsBoundaryAbstract
{
public:
    NvsBoundary();
    ~NvsBoundary() override;
    void writeUint64(const char* key, uint64_t value) override;
    uint64_t readUint64(const char* key, uint64_t defaultValue = 0) override;
    void eraseData(const char* key) override;

private:
    nvs_handle_t nvsHandle;
};

class Storage {
public:
    explicit Storage(NvsBoundaryAbstract& nvsBoundary);
    void setSessionId(uint64_t sessionId);
    uint64_t getSessionId();
    void clearSessionId();

private:
    NvsBoundaryAbstract& nvsService;
};



#endif // STORAGE_HPP
