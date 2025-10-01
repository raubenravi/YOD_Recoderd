#include "audioAnalyzer.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "esp_log.h"
#include "driver/adc.h"
#include "esp_timer.h"
#include "dsps_fft2r.h"
#include "dsps_wind.h"
#include "dsps_view.h"
#include "esp_dsp.h"
#include "esp_rom_sys.h"
#include "esp_heap_caps.h"

static const char *TAG = "AudioAnalyzer";

AudioAnalyzer::AudioAnalyzer(int nSamples, float sampleRate)
        : N(nSamples), sampleRate(sampleRate), measuredSampleRate(sampleRate),
            x1(nullptr), x2(nullptr), wind(nullptr), yCf(nullptr), y1Cf(nullptr),
            y2Cf(nullptr), sumY(nullptr), peakBin(0), peakVal(0), peakFreq(0)
{
    x1 = (float *)heap_caps_aligned_alloc(16, N * sizeof(float), MALLOC_CAP_8BIT);
    x2 = (float *)heap_caps_aligned_alloc(16, N * sizeof(float), MALLOC_CAP_8BIT);
    wind = (float *)heap_caps_aligned_alloc(16, N * sizeof(float), MALLOC_CAP_8BIT);
    yCf = (float *)heap_caps_aligned_alloc(16, N * 2 * sizeof(float), MALLOC_CAP_8BIT);
    y1Cf = &yCf[0];
    y2Cf = &yCf[N];
    sumY = (float *)heap_caps_aligned_alloc(16, (N / 2) * sizeof(float), MALLOC_CAP_8BIT);
}

AudioAnalyzer::~AudioAnalyzer() {
    free(x1);
    free(x2);
    free(wind);
    free(yCf);
    free(sumY);
}

esp_err_t AudioAnalyzer::init() {
    esp_err_t ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Not possible to initialize FFT. Error = %i", ret);
        return ret;
    }
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11);
    return ESP_OK;
}

void AudioAnalyzer::sampleInput() {
    // Derive delay from desired sample rate (in microseconds) and measure effective rate
    const float targetPeriodUs = 1e6f / sampleRate;
    const int delayUs = (int)lrintf(fmaxf(targetPeriodUs, 0.0f));

    const int64_t startTimeUs = esp_timer_get_time();

    for (int i = 0; i < N; i++) {
        int val = adc1_get_raw(ADC1_CHANNEL_5);
        x1[i] = ((float)val / 2048.0f) - 1.0f;
        x2[i] = ((float)val / 2048.0f) - 1.0f;
        if (i < N - 1 && delayUs > 0) {
            esp_rom_delay_us(delayUs);
        }
    }

    const int64_t elapsedUs = esp_timer_get_time() - startTimeUs;
    if (elapsedUs > 0) {
        measuredSampleRate = ((float)N * 1e6f) / (float)elapsedUs;
    }
    // for (int i = 0; i < 10; i++) {
    //     printf("x1[%d] = %f\n", i, x1[i]);
    // }
}

void AudioAnalyzer::computeFft() {
    dsps_wind_hann_f32(wind, N);
    for (int i = 0 ; i < N ; i++) {
        yCf[i * 2 + 0] = x1[i] * wind[i];
        yCf[i * 2 + 1] = x2[i] * wind[i];
    }
    unsigned int start_b = dsp_get_cpu_cycle_count();
    dsps_fft2r_fc32(yCf, N);
    unsigned int end_b = dsp_get_cpu_cycle_count();
    dsps_bit_rev_fc32(yCf, N);
    dsps_cplx2reC_fc32(yCf, N);

    // Calculate frequency resolution
    float freqResolution = (measuredSampleRate > 0.0f ? measuredSampleRate : sampleRate) / N;

    // Process all bins
      for (int i = 0 ; i < N / 2 ; i++) {
        y1Cf[i] = 10 * log10f((y1Cf[i * 2 + 0] * y1Cf[i * 2 + 0] + y1Cf[i * 2 + 1] * y1Cf[i * 2 + 1]) / N);
        y2Cf[i] = 10 * log10f((y2Cf[i * 2 + 0] * y2Cf[i * 2 + 0] + y2Cf[i * 2 + 1] * y2Cf[i * 2 + 1]) / N);
        // Simple way to show two power spectrums as one plot
        sumY[i] = fmax(y1Cf[i], y2Cf[i]);
    }
    peakBin = 0; // Start peak detection from the first bin
    peakVal = sumY[5];
    for (int i = 5; i < N / 2; i++) { // Iterate through all N/2 bins
        if (sumY[i] > peakVal) {
            peakVal = sumY[i];
            peakBin = i;
        }
    }
    peakFreq = peakBin * freqResolution;

    //ESP_LOGI(TAG, "FFT for %i complex points take %i cycles", N, end_b - start_b);
}

void AudioAnalyzer::printResults() {
    ESP_LOGW(TAG, "Power Spectrum (x1 only)");
    //dsps_view(y1Cf, N / 2, 64, 10,  -60, 40, '|');
    dsps_view(sumY, N / 2, 64, 10,  -60, 40, '|');
    ESP_LOGI(TAG, "Peak frequency: %.2f Hz (bin %d, value %.2f dB)", peakFreq, peakBin, peakVal);
}

bool AudioAnalyzer::isWord() {
    // ESP_LOGI(TAG, "Peak value: %.2f dB", peakVal);
    if (peakVal > -40 && peakFreq >= 300 && peakFreq <= 3300){
        return true; // Word detected if peak value is above threshold
    }else {
        return false; // No word detected
    }
}

//peak db 15, min 3 , dus denk 8