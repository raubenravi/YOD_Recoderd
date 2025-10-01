/**
 * @file recoderAbstract.hpp
 * @brief Abstract base class for a recorder interface.
 * 
 * This file defines the RecoderAbstract class, which provides a virtual interface
 * for recorder operations such as starting, stopping, and idling.
 */

#ifndef RECODER_ABSTRACT_HPP
#define RECODER_ABSTRACT_HPP

#include <cstdint>

/**
 * @class RecoderAbstract
 * @brief Abstract base class for a recorder interface.
 * 
 * This class defines the interface for a recorder, including methods for starting,
 * stopping, and idling the recorder. Derived classes must implement these methods.
 */
class RecoderAbstract {
public:
    /**
     * @brief Virtual destructor for the RecoderAbstract class.
     */
    virtual ~RecoderAbstract() = default;

    /**
     * @brief Start the recording process.
     */
    virtual void startRecording() = 0;

    /**
     * @brief Stop the recording process.
     */
    virtual void stopRecording() = 0;

    /**
     * @brief Set the recorder to an idle state.
     */
    virtual void idle() = 0;
};

#endif // RECODER_ABSTRACT_HPP