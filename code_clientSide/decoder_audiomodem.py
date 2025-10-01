import numpy as np
from scipy.io import wavfile
from scipy.signal import butter, filtfilt

# Function to apply a bandpass filter to the audio data
def _bandpass_filter(data, lowcut, highcut, samplerate, order=5):
    nyquist = 0.5 * samplerate
    low = lowcut / nyquist
    high = highcut / nyquist
    if high >= 1.0:
        high = 0.99
    if low <= 0:
        low = 0.01
    b, a = butter(order, [low, high], btype='band')
    return filtfilt(b, a, data)

# Function to read a WAV audio file and convert it to mono if necessary
def _read_audio_file(filename):
    samplerate, data = wavfile.read(filename)
    if data.ndim > 1:
        data = np.mean(data, axis=1).astype(data.dtype)
    return samplerate, data

# Function to normalize a chunk of audio data
def _normalize_chunk(chunk, norm_type="rms", target=1.0):
    chunk = chunk.astype(np.float64)
    if norm_type == "rms":
        value = np.sqrt(np.mean(chunk**2))
    elif norm_type == "max":
        value = np.max(np.abs(chunk))
    else:
        raise ValueError("norm_type must be 'rms' or 'max'")
    if value < 1e-8:
        return chunk
    return chunk * (target / value)

# Function to analyze a single chunk of audio data
def _analyze_single_chunk(chunk, samplerate, freq1, freq2):
    N = len(chunk)
    dominant_tone = 0
    volume = 0.0
    if N >= 5:
        yf = np.fft.fft(chunk)
        xf = np.fft.fftfreq(N, 1 / samplerate)
        xf_pos = xf[:N // 2]
        yf_mag_pos = np.abs(yf[:N // 2])
        if N // 2 > 0:
            idx_freq1 = np.argmin(np.abs(xf_pos - freq1))
            mag_freq1 = yf_mag_pos[idx_freq1]
            idx_freq2 = np.argmin(np.abs(xf_pos - freq2))
            mag_freq2 = yf_mag_pos[idx_freq2]
            if mag_freq1 > mag_freq2:
                dominant_tone = 0
                volume = mag_freq1
            elif mag_freq2 > mag_freq1:
                dominant_tone = 1
                volume = mag_freq2
    return dominant_tone, volume

# Main function to analyze audio data
def analyze_audio(
    filename=None, 
    audio_data=None, 
    samplerate=None, 
    chunk_duration=0.05, 
    freq1=3000, 
    freq2=4000, 
    verbose=True,
    use_normalization=True,
    norm_type="rms",
    normalization_target=1.0,
    use_bandpass=True,
    lowcut=20000,
    highcut=None
):
    if audio_data is not None and samplerate is not None:
        data = audio_data
    elif filename is not None:
        samplerate, data = _read_audio_file(filename)
        if samplerate is None or data is None:
            return
    else:
        if verbose:
            print("Error: Either filename or (audio_data + samplerate) must be provided")
        return

    if use_bandpass:
        if highcut is None:
            highcut = samplerate // 2 - 1000
        if verbose:
            print(f"Applying bandpass filter: {lowcut}Hz - {highcut}Hz")
        data = _bandpass_filter(data, lowcut, highcut, samplerate)

    samples_per_chunk = int(samplerate * chunk_duration)
    if samples_per_chunk == 0:
        if verbose:
            print(f"Error: chunk_duration ({chunk_duration}s) is too short for the sample rate ({samplerate}Hz), resulting in 0 samples per chunk.")
        return

    total_samples = len(data)
    if total_samples == 0:
        if verbose:
            print("Audio file is empty or could not be read properly.")
        return

    start_time_sec = None
    current_sample_pos = 0
    dominant_tones = []
    while current_sample_pos < total_samples:
        start_sample = current_sample_pos
        end_sample = min(current_sample_pos + samples_per_chunk, total_samples)
        chunk = data[start_sample:end_sample]
        if len(chunk) == 0:
            break 
        if use_normalization:
            chunk = _normalize_chunk(chunk, norm_type=norm_type, target=normalization_target)
        current_time_sec = start_sample / samplerate
        dominant_tone_str, volume = _analyze_single_chunk(chunk, samplerate, freq1, freq2)
        if volume > 150 and start_time_sec is None:
            start_time_sec = start_sample / samplerate
        if start_time_sec is not None:
            dominant_tones.append(dominant_tone_str)
        is_partial_chunk_str = " (partial)" if (end_sample - start_sample) < samples_per_chunk and end_sample == total_samples else ""
        if verbose:
            print(f"{current_time_sec:<10.2f} | {dominant_tone_str:<25} | {volume:<15.2f}{is_partial_chunk_str}")
        current_sample_pos = end_sample

    if verbose:
        print(dominant_tones)
    decoded_bits = []
    for i in range(0, len(dominant_tones) - len(dominant_tones) % 5, 5):
        chunk = dominant_tones[i:i+5]
        count_ones = chunk.count(1)
        count_zeros = chunk.count(0)
        if count_ones > count_zeros:
            decoded_bits.append(1)
        else:
            decoded_bits.append(0)
    if verbose:
        print(f"Decoded bits: {decoded_bits}")
    if verbose:
        print("-" * 55)
    if start_time_sec is not None:
        if verbose:
            print(f"Start time of the signal: {start_time_sec:.2f} seconds")
        return decoded_bits
    else:
        if verbose:
            print("No significant signal detected.")
        return None

if __name__ == "__main__":
    decoded_bits = analyze_audio(
        filename="audio_files/250905_0124S34.wav", 
        chunk_duration=0.004, 
        freq1=21_000, 
        freq2=23_000,
        use_normalization=True,
        norm_type="max",
        normalization_target=5.0,
        use_bandpass=True,
        lowcut=20000,
        verbose=True
    )
    if decoded_bits:
        numbers = []
        for i in range(0, len(decoded_bits), 8):
            if i + 8 <= len(decoded_bits):
                byte_bits = decoded_bits[i:i+8]
                number = sum(bit * (2 ** idx) for idx, bit in enumerate(byte_bits))
                numbers.append(number)
                print(f"Decoded number {len(numbers)}: {number} (bits: {byte_bits})")
        print(f"\nAll decoded numbers: {numbers[:10]}")
        print(f"Total numbers decoded: {len(numbers)}")
    else:
        print("No numbers could be decoded.")


