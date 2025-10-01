#pragma once

#include "esp_err.h"

/**
 * @class AudioAnalyzer
 * @brief A class for analyzing audio signals using FFT.
 * A lot of the code comes from the ESP-DSP library examples.
 */
class AudioAnalyzer {
  public:
    /**
     * @brief Constructor for the AudioAnalyzer class.
     *
     * @param nSamples Number of samples to analyze. Default is 1024.
     * @param sampleRate Sampling rate in Hz. Default is 10000.0f.
     */
    AudioAnalyzer(int nSamples = 1024, float sampleRate = 10000.0f);

    /**
     * @brief Destructor for the AudioAnalyzer class.
     */
    ~AudioAnalyzer();

    /**
     * @brief Initializes the audio analyzer.
     *
     * @return ESP_OK on success, or an error code on failure.
     */
    esp_err_t init();

    /**
     * @brief Samples input audio data.
     */
    void sampleInput();

    /**
     * @brief Computes the FFT of the sampled audio data.
     */
    void computeFft();

    /**
     * @brief Prints the results of the FFT analysis.
     */
    void printResults();

    /**
     * @brief Checks if a word is detected in the audio signal.
     *
     * @return True if a word is detected, false otherwise.
     */
    bool isWord();

  private:
    /**
     * @brief Number of samples to analyze.
     */
    int N;

    /**
     * @brief Sampling rate in Hz.
     */
  float sampleRate;

  /**
   * @brief Measured sampling rate in Hz based on the last capture.
   */
  float measuredSampleRate;

    /**
     * @brief Input audio data buffer.
     */
    float *x1;

    /**
     * @brief Secondary input audio data buffer.
     */
    float *x2;

    /**
     * @brief Windowing function buffer.
     */
    float *wind;

    /**
     * @brief FFT result buffer.
     */
    float *yCf;

    /**
     * @brief Secondary FFT result buffer.
     */
    float *y1Cf;

    /**
     * @brief Tertiary FFT result buffer.
     */
    float *y2Cf;

    /**
     * @brief Summed FFT results buffer.
     */
    float *sumY;

    /**
     * @brief Index of the peak bin in the FFT result.
     */
    int peakBin;

    /**
     * @brief Value of the peak in the FFT result.
     */
    float peakVal;

    /**
     * @brief Frequency of the peak in the FFT result.
     */
    float peakFreq;
};