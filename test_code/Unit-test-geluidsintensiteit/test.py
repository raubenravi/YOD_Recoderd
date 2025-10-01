import os
import numpy as np
import matplotlib.pyplot as plt
from sound_intensity import get_intensity_at_frequency


def theoretical_intensity(distance_m, speaker_power_w=0.068):
    """Return theoretical intensity at distance_m for given speaker power."""

    if distance_m <= 0:
        return None
    return speaker_power_w / (4.0 * np.pi * distance_m ** 2)


if __name__ == "__main__":
    target_frequency = 500.0  # Hz
    speaker_power_w = 0.068   # Gegeven luidsprekervermogen
    test_folder = "test_intensity_recordings"
    sample_to_pascal = 1.0     # Stel in op bekende kalibratie (Pa per genormaliseerde sample)

    if not os.path.isdir(test_folder):
        raise FileNotFoundError(f"Testmap '{test_folder}' niet gevonden.")

    distance_mapping_cm = {

        "50.wav": 50,
        "75.wav": 75,
        "100.wav": 100,
        "125.wav": 125,
        "150.wav": 150,
    }

    print("Metingen per bestand (500 Hz)")
    print("=" * 72)
    print(
        "Opmerking: intensiteiten zijn absoluut als 'sample_to_pascal' op een\n"
        "gekalibreerde waarde staat; anders zijn de resultaten relatief."
    )

    measurements = []
    for filename, distance_cm in sorted(distance_mapping_cm.items(), key=lambda item: item[1]):
        wav_path = os.path.join(test_folder, filename)
        if not os.path.isfile(wav_path):
            print(f"Waarschuwing: '{filename}' ontbreekt, overslaan.")
            continue

        try:
            measurement = get_intensity_at_frequency(
                wav_path,
                target_freq=target_frequency,
                sample_to_pascal=sample_to_pascal,
            )
            measurement['file'] = filename
            measurement['distance_cm'] = distance_cm
            measurement['distance_m'] = distance_cm / 100.0
            measurement['theoretical_intensity'] = theoretical_intensity(
                measurement['distance_m'],
                speaker_power_w=speaker_power_w,
            )
            measurements.append(measurement)
        except Exception as exc:
            print(f"Fout bij verwerken van '{filename}': {exc}")

    if not measurements:
        raise RuntimeError("Geen metingen uitgevoerd.")

    print(
        f"{'Bestand':<12}{'Gemeten (cm)':>14}{'Freq (Hz)':>12}"
        f"{'Gemeten I (W/m²)':>20}{'Theo. I (W/m²)':>20}{'I Ratio':>12}"
    )
    print("-" * 80)

    for m in measurements:
        theoretical_I = m.get('theoretical_intensity')
        ratio = (
            abs(m['intensity']) / theoretical_I
            if theoretical_I not in (None, 0)
            else float('nan')
        )

        print(
            f"{m['file']:<12}{m['distance_cm']:>14.0f}{m['actual_frequency']:>12.1f}"
            f"{abs(m['intensity']):>20.6f}{theoretical_I:>20.6f}{ratio:>12.3f}"
        )

    # Plotting
    distances = [m['distance_m'] for m in measurements]
    gemeten_i = [abs(m['intensity']) for m in measurements]
    theo_i = [m['theoretical_intensity'] for m in measurements]

    plt.figure(figsize=(10, 6))
    plt.plot(distances, gemeten_i, 'bo-', label='Gemeten Intensiteit')
    plt.plot(distances, theo_i, 'ro--', label='Theoretische Intensiteit')
    plt.xlabel("Afstand (m)")
    plt.ylabel("Intensiteit (W/m²)")
    plt.title("Gemeten vs. Theoretische Intensiteit")
    plt.legend()
    plt.grid(True)
    plt.savefig('intensity_plot.png')
    print("\nPlot saved as intensity_plot.png")
