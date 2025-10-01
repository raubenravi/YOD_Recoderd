#!/usr/bin/env python3
"""
Audacity Launcher
This module handles finding and launching Audacity with audio files.
"""

import os
import subprocess


def find_audacity_executable():
    """Find Audacity executable on the system"""
    possible_paths = [
        r"C:\Program Files\Audacity\Audacity.exe",
        r"C:\Program Files (x86)\Audacity\Audacity.exe",
        r"C:\Users\{}\AppData\Local\Programs\Audacity\Audacity.exe".format(os.getenv('USERNAME', '')),
        "audacity.exe",  # If it's in PATH
    ]
    
    for path in possible_paths:
        if os.path.exists(path):
            return path
    
    return None


def launch_audacity(file_path):
    """Launch Audacity with the specified audio file"""
    audacity_path = find_audacity_executable()
    if not audacity_path:
        print("❌ Could not find Audacity installation")
        return False

    try:
        print(f"\n🚀 Opening in Audacity...")
        cmd = [audacity_path, file_path]
        process = subprocess.Popen(cmd,
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE,
                                 creationflags=subprocess.CREATE_NO_WINDOW if os.name == 'nt' else 0)

        print("🎉 Success! Audacity started with the audio file!")
        return True

    except Exception as e:
        print(f"❌ Error starting Audacity: {e}")
        return False


def launch_audacity_with_multiple_files(file_paths):
    """Launch Audacity with multiple audio files (opens first one)"""
    if not file_paths:
        print("❌ No files to open")
        return False
        
    audacity_path = find_audacity_executable()
    if not audacity_path:
        print("❌ Could not find Audacity installation")
        return False

    try:
        print(f"\n🚀 Opening first file in Audacity...")
        print(f"📁 Created {len(file_paths)} concatenated files in temp folder")
        
        # Open the first file
        cmd = [audacity_path, file_paths[0]]
        process = subprocess.Popen(cmd,
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE,
                                 creationflags=subprocess.CREATE_NO_WINDOW if os.name == 'nt' else 0)

        print("🎉 Success! Audacity started with the first concatenated file!")
        print("💡 Check the temp folder for all grouped concatenated files.")
        return True

    except Exception as e:
        print(f"❌ Error starting Audacity: {e}")
        return False
