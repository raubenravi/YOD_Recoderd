#!/usr/bin/env python3
"""
Audacity Audio Concatenator
This script concatenates all audio files in a folder into one continuous file
and opens it in Audacity for editing.
"""

import os
import sys
import subprocess
import glob
from pathlib import Path
import audacity_launcher

# Import required packages
import numpy as np
from scipy.io import wavfile
from scipy import signal


def get_audio_files(folder_path):
    """Get all audio files in the specified folder"""
    audio_extensions = ['*.wav', '*.mp3', '*.flac', '*.ogg', '*.aiff', '*.au', '*.m4a']
    audio_files = set()  # Use set to avoid duplicates
    
    for extension in audio_extensions:
        pattern = os.path.join(folder_path, extension)
        audio_files.update(glob.glob(pattern, recursive=False))
        # Also check for uppercase extensions
        pattern_upper = os.path.join(folder_path, extension.upper())
        audio_files.update(glob.glob(pattern_upper, recursive=False))
    
    return sorted(list(audio_files))

def find_audacity_executable():
    """Find Audacity executable on the system - DEPRECATED: Use audacity_launcher module instead"""
    return audacity_launcher.find_audacity_executable()

def load_audio_file(file_path):
    """Load an audio file using the best available method"""
    
    try:
        # Use scipy for WAV files
        sample_rate, audio_data = wavfile.read(file_path)
        return audio_data, sample_rate
    except Exception as e:
        print(f"⚠️ Scipy failed for {file_path}: {e}")
    
    print(f"❌ Could not load {file_path} - no suitable audio library available")
    return None, None

def concatenate_audio_files(folder_path, audio_files):
    """Concatenate all audio files into one continuous audio stream"""
    
    print(f"\n🎵 Concatenating {len(audio_files)} audio files...")
    
    all_audio_data = []
    target_sample_rate = None
    total_duration = 0
    
    for i, file_path in enumerate(audio_files):
        audio_data, sample_rate = load_audio_file(file_path)
        
        if audio_data is None:
            print(f"⚠️ Skipping {os.path.basename(file_path)} - could not load")
            continue
        
        # Set target sample rate from first successful file
        if target_sample_rate is None:
            target_sample_rate = sample_rate
        
        # Resample if necessary
        if sample_rate != target_sample_rate:
            num_samples = int(len(audio_data) * target_sample_rate / sample_rate)
            audio_data = signal.resample(audio_data, num_samples).astype(np.int16)
        
        # Handle stereo/mono conversion
        if len(audio_data.shape) > 1:
            audio_data = np.mean(audio_data, axis=1).astype(np.int16)
        
        duration = len(audio_data) / target_sample_rate
        total_duration += duration
        
        all_audio_data.append(audio_data)
        
        # Add a small gap between files (0.1 seconds of silence)
        if i < len(audio_files) - 1:
            gap_samples = int(0.1 * target_sample_rate)
            silence = np.zeros(gap_samples, dtype=np.int16)
            all_audio_data.append(silence)
            total_duration += 0.1
    
    if not all_audio_data:
        print("❌ No audio files could be processed!")
        return None
    
    # Concatenate all audio data
    combined_audio = np.concatenate(all_audio_data)
    
    print(f"✅ Combined audio: {total_duration:.1f}s ({total_duration/60:.1f} min)")
    
    # Create temp folder if it doesn't exist
    temp_folder = os.path.join(folder_path, "temp")
    os.makedirs(temp_folder, exist_ok=True)
    
    # Save the combined audio to temp folder
    output_filename = "concatenated_audio.wav"
    output_path = os.path.join(temp_folder, output_filename)
    
    try:
        wavfile.write(output_path, target_sample_rate, combined_audio)
        return output_path
    except Exception as e:
        print(f"❌ Failed to save combined audio: {e}")
        return None

def open_in_audacity(folder_path):
    """Concatenate all audio files and open the result in Audacity"""

    # Check if folder exists
    if not os.path.exists(folder_path):
        print(f"❌ Error: Folder '{folder_path}' does not exist")
        return False

    # Get audio files
    audio_files = get_audio_files(folder_path)

    if not audio_files:
        print(f"❌ No audio files found in '{folder_path}'")
        return False

    print(f"📁 Found {len(audio_files)} audio files")

    # --- DECODER: Call audio_decoder_one for each file before concatenating ---
    try:
        import audio_decoder_one
        print(f"\n🔎 Decoding audio from files...")
        
        all_decoded_bits = []
        
        for i, file_to_decode in enumerate(audio_files):
            print(f"Processing {i+1}/{len(audio_files)}: {os.path.basename(file_to_decode)}")
            
            try:
                samplerate, data = audio_decoder_one._read_audio_file(file_to_decode)
                if data is None:
                    continue
                    
                decoded_bits = audio_decoder_one.analyze_audio(audio_data=data, samplerate=samplerate, chunk_duration=0.04, freq1=21_000, freq2=23_000, verbose=False)
                
                if decoded_bits:
                    number = sum(bit * (2 ** idx) for idx, bit in enumerate(decoded_bits))
                    print(f"   Decoded: {decoded_bits} → {number}")
                    all_decoded_bits.append({
                        'file': file_to_decode,  # Store full path
                        'filename': os.path.basename(file_to_decode),
                        'bits': decoded_bits,
                        'number': number
                    })
                else:
                    all_decoded_bits.append({
                        'file': file_to_decode,  # Store full path
                        'filename': os.path.basename(file_to_decode),
                        'bits': None,
                        'number': None
                    })
            except Exception as e:
                print(f"   Error: {e}")
                all_decoded_bits.append({
                    'file': file_to_decode,  # Store full path
                    'filename': os.path.basename(file_to_decode),
                    'bits': None,
                    'number': None
                })
                
    except Exception as e:
        print(f"⚠️ Decoder error: {e}")

    # Group files by decoded value
    print(f"\n📊 Grouping files by decoded value...")
    
    groups = {}
    for result in all_decoded_bits:
        if result['number'] is not None:
            number = result['number']
            if number not in groups:
                groups[number] = []
            groups[number].append(result['file'])
        else:
            # Files with no decoded value go to a special group
            if 'no_decode' not in groups:
                groups['no_decode'] = []
            groups['no_decode'].append(result['file'])
    
    print(f"Found {len(groups)} groups:")
    for key, files in groups.items():
        if key == 'no_decode':
            print(f"   • No decode: {len(files)} files")
        else:
            print(f"   • Value {key}: {len(files)} files")
    
    # Create concatenated files for each group
    created_files = []
    for group_value, group_files in groups.items():
        if len(group_files) == 0:
            continue
            
        print(f"\n🎵 Processing group '{group_value}' with {len(group_files)} files...")
        
        # Create concatenated file for this group
        combined_file_path = concatenate_audio_files_for_group(folder_path, group_files, group_value)
        
        if combined_file_path:
            created_files.append(combined_file_path)
        else:
            print(f"❌ Failed to create file for group {group_value}")
    
    if not created_files:
        print("❌ No concatenated files were created")
        return False

    # Launch Audacity with the created files
    return audacity_launcher.launch_audacity_with_multiple_files(created_files)

def concatenate_audio_files_for_group(folder_path, audio_files, group_value):
    """Concatenate audio files for a specific group"""
    
    if not audio_files:
        return None
    
    all_audio_data = []
    target_sample_rate = None
    total_duration = 0
    
    for i, file_path in enumerate(audio_files):
        audio_data, sample_rate = load_audio_file(file_path)
        
        if audio_data is None:
            print(f"⚠️ Skipping {os.path.basename(file_path)} - could not load")
            continue
        
        # Set target sample rate from first successful file
        if target_sample_rate is None:
            target_sample_rate = sample_rate
        
        # Resample if necessary
        if sample_rate != target_sample_rate:
            num_samples = int(len(audio_data) * target_sample_rate / sample_rate)
            audio_data = signal.resample(audio_data, num_samples).astype(np.int16)
        
        # Handle stereo/mono conversion
        if len(audio_data.shape) > 1:
            audio_data = np.mean(audio_data, axis=1).astype(np.int16)
        
        duration = len(audio_data) / target_sample_rate
        total_duration += duration
        
        all_audio_data.append(audio_data)
        
        # Add a small gap between files (0.1 seconds of silence)
        if i < len(audio_files) - 1:
            gap_samples = int(0.1 * target_sample_rate)
            silence = np.zeros(gap_samples, dtype=np.int16)
            all_audio_data.append(silence)
            total_duration += 0.1
    
    if not all_audio_data:
        print(f"❌ No audio files could be processed for group {group_value}!")
        return None
    
    # Concatenate all audio data
    combined_audio = np.concatenate(all_audio_data)
    
    print(f"✅ Group {group_value}: {total_duration:.1f}s ({total_duration/60:.1f} min)")
    
    # Create temp folder if it doesn't exist
    temp_folder = os.path.join(folder_path, "temp")
    os.makedirs(temp_folder, exist_ok=True)
    
    # Save the combined audio to temp folder with group-specific name
    if group_value == 'no_decode':
        output_filename = "concatenated_no_decode.wav"
    elif group_value == 'invalid_detection':
        output_filename = "concatenated_invalid_detection.wav"
    elif group_value == 'no_session_id':
        output_filename = "concatenated_no_session_id.wav"
    elif isinstance(group_value, tuple):
        # group_value is a tuple of numbers (legacy support)
        numbers_str = "_".join(map(str, group_value))
        output_filename = f"concatenated_numbers_{numbers_str}.wav"
    else:
        # group_value is a session ID
        output_filename = f"concatenated_session_{group_value}.wav"
    output_path = os.path.join(temp_folder, output_filename)
    
    try:
        wavfile.write(output_path, target_sample_rate, combined_audio)
        print(f"💾 Saved: {output_filename}")
        return output_path
    except Exception as e:
        print(f"❌ Failed to save combined audio for group {group_value}: {e}")
        return None

def main():
    """Main function - concatenates all audio files into one continuous file"""
    default_folder = r"audio_files"

    if len(sys.argv) > 1:
        folder_path = sys.argv[1]
    else:
        folder_path = default_folder

    success = open_in_audacity(folder_path)

    if success:
        print(f"\n✅ Done!")
    else:
        print(f"\n❌ Operation failed.")

if __name__ == "__main__":
    main()
