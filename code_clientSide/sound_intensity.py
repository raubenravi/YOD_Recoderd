import numpy as np
from scipy.io import wavfile
from scipy.signal import csd


"""
This one is based on the paper:Determination of the Sound Intensity Vector Field from Synchronized Sound Pressure Waveforms 



"""

def get_intensity_at_frequency(
    wav_path,
    target_freq=500.0,        # Doelfrequentie
    d=0.15,                   # Microfoonafstand in meters (kanaal 1 -> kanaal 2 richting)
    rho=1.3,                  # Luchtdichtheid in kg/m³
    c=343.0,                  # Geluidssnelheid in m/s
    nperseg=4096,             # Segmentlengte voor spectra
    noverlap=0.1,             # Overlapfactor (float in 0-1 of aantal samples)
    fmin=20.0,                # Minimum frequentie in Hz
    fmax=2500.0,              # Maximum frequentie in Hz
    sample_to_pascal=1.0      # Kalibratiefactor voor druk (Pa per genormaliseerde sample)
):

    """Bepaal de actieve intensiteit op een doelfrequentie met behulp van de
    cross-power spectral density. Zonder kalibratie (sample_to_pascal=1.0)
    levert de functie relatieve waarden.
    """

    # Lees audiobestand
    fs, x = wavfile.read(wav_path)
    x = x.astype(np.float64)

    # Controleer of het stereo is
    if x.ndim == 1 or x.shape[1] < 2:
        raise ValueError("Bestand moet minstens 2 kanalen hebben (stereo).")

    # Haal de twee kanalen op (kanaal 1 ligt dichter bij de bron dan kanaal 2)
    p1, p2 = x[:, 0], x[:, 1]

    # Normaliseer naar [-1, 1] bereik
    if np.issubdtype(x.dtype, np.integer):
        maxint = np.iinfo(x.dtype).max
        p1, p2 = p1 / maxint, p2 / maxint

    # Pas optionele drukkalibratie toe
    p1 *= sample_to_pascal
    p2 *= sample_to_pascal

    # Stel segmentgrootte en overlap vast
    nperseg = int(nperseg)
    noverlap = int(round(noverlap * nperseg)) if isinstance(noverlap, float) else int(noverlap)

    # Bepaal aliasvrije bovengrens op basis van microfoonafstand
    alias_limit = 0.999 * (c / (2.0 * d))
    if fmax is None:
        fmax = alias_limit
    else:
        fmax = min(fmax, alias_limit)

    # Bereken cross-power spectral density
    f, P21 = csd(
        p2,
        p1,
        fs=fs,
        window="hann",
        nperseg=nperseg,
        noverlap=noverlap,
        scaling="density",
        return_onesided=True,
    )

    # Maak frequentiemasker
    mask = (f >= max(fmin, 1.0)) & (f <= min(fmax, fs * 0.5 * 0.999))
    f_sel = f[mask]

    if f_sel.size == 0:
        raise ValueError("Frequentiemasker is leeg. Controleer fmin en fmax waarden.")

    # Actieve intensiteit uit de imaginaire component van het cross-spectrum
    omega = 2.0 * np.pi * f_sel
    S21 = P21[mask]
    intensity = -np.imag(S21) / (rho * d * omega)
    # Negatieve intensiteit geeft aan dat het geluid kanaal 2 eerder bereikt
    # (faseomkering t.o.v. de aanname dat kanaal 1 dichter bij de bron is).

    # Vind de dichtstbijzijnde frequentie bij de doelfrequentie
    freq_idx = int(np.argmin(np.abs(f_sel - target_freq)))
    actual_freq = f_sel[freq_idx]
    intensity_at_freq = intensity[freq_idx]

    return {
        "target_frequency": target_freq,
        "actual_frequency": actual_freq,
        "intensity": intensity_at_freq,
    }
