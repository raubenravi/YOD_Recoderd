#!/usr/bin/env python3
"""
Audio GUI - Tkinter interface for viewing decoded values and launching concatenated audio files
"""

import tkinter as tk
from tkinter import ttk, messagebox, filedialog
import os
import glob
from pathlib import Path
import threading
import decoder_audiomodem as DecoderNew
import audacity_launcher
from audacity_concatenator import concatenate_audio_files_for_group


class AudioDecoderGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Audio Decoder & Concatenator GUI")
        self.root.geometry("800x600")
        
        # Data storage
        self.decoded_results = []
        self.groups = {}
        self.current_folder = ""
        
        self.setup_ui()
        
    def setup_ui(self):
        """Set up the user interface"""
        # Main frame
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # Configure grid weights
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)
        main_frame.columnconfigure(1, weight=1)
        main_frame.rowconfigure(2, weight=1)
        
        # Folder selection
        ttk.Label(main_frame, text="Audio Folder:").grid(row=0, column=0, sticky=tk.W, padx=(0, 5))
        self.folder_var = tk.StringVar(value="audio_files")
        folder_frame = ttk.Frame(main_frame)
        folder_frame.grid(row=0, column=1, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 10))
        folder_frame.columnconfigure(0, weight=1)
        
        self.folder_entry = ttk.Entry(folder_frame, textvariable=self.folder_var)
        self.folder_entry.grid(row=0, column=0, sticky=(tk.W, tk.E), padx=(0, 5))
        
        ttk.Button(folder_frame, text="Browse", command=self.browse_folder).grid(row=0, column=1)
        ttk.Button(folder_frame, text="Scan", command=self.scan_folder).grid(row=0, column=2, padx=(5, 0))
        
        # Progress bar
        self.progress_var = tk.StringVar(value="Ready")
        ttk.Label(main_frame, textvariable=self.progress_var).grid(row=1, column=0, columnspan=3, sticky=tk.W, pady=(0, 10))
        
        # Results area
        results_frame = ttk.LabelFrame(main_frame, text="Decoded Values & Files", padding="5")
        results_frame.grid(row=2, column=0, columnspan=3, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 10))
        results_frame.columnconfigure(0, weight=1)
        results_frame.rowconfigure(1, weight=1)
        
        # Treeview for results
        columns = ("Value", "Files", "Status")
        self.tree = ttk.Treeview(results_frame, columns=columns, show="tree headings", height=15)
        self.tree.grid(row=1, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # Configure columns
        self.tree.heading("#0", text="Group")
        self.tree.heading("Value", text="Decoded Value")
        self.tree.heading("Files", text="File Count")
        self.tree.heading("Status", text="Concat Status")
        
        self.tree.column("#0", width=100, minwidth=100)
        self.tree.column("Value", width=120, minwidth=100)
        self.tree.column("Files", width=80, minwidth=80)
        self.tree.column("Status", width=120, minwidth=100)
        
        # Scrollbar for treeview
        scrollbar = ttk.Scrollbar(results_frame, orient=tk.VERTICAL, command=self.tree.yview)
        scrollbar.grid(row=1, column=1, sticky=(tk.N, tk.S))
        self.tree.configure(yscrollcommand=scrollbar.set)
        
        # Double-click binding
        self.tree.bind("<Double-1>", self.on_double_click)
        
        # Action buttons
        button_frame = ttk.Frame(main_frame)
        button_frame.grid(row=3, column=0, columnspan=3, pady=(10, 0))
        
        ttk.Button(button_frame, text="Create Concatenated File", 
                  command=self.create_selected_concat).pack(side=tk.LEFT, padx=(0, 5))
        ttk.Button(button_frame, text="Launch in Audacity", 
                  command=self.launch_selected).pack(side=tk.LEFT, padx=(0, 5))
        ttk.Button(button_frame, text="Create All & Launch First", 
                  command=self.create_all_and_launch).pack(side=tk.LEFT, padx=(0, 5))
        ttk.Button(button_frame, text="Refresh", 
                  command=self.scan_folder).pack(side=tk.LEFT, padx=(0, 5))
        
        # Details text area
        details_frame = ttk.LabelFrame(main_frame, text="File Details", padding="5")
        details_frame.grid(row=4, column=0, columnspan=3, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(10, 0))
        details_frame.columnconfigure(0, weight=1)
        details_frame.rowconfigure(0, weight=1)
        
        self.details_text = tk.Text(details_frame, height=8, wrap=tk.WORD)
        self.details_text.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        details_scrollbar = ttk.Scrollbar(details_frame, orient=tk.VERTICAL, command=self.details_text.yview)
        details_scrollbar.grid(row=0, column=1, sticky=(tk.N, tk.S))
        self.details_text.configure(yscrollcommand=details_scrollbar.set)
        
        # Bind tree selection
        self.tree.bind("<<TreeviewSelect>>", self.on_tree_select)
        
        # Load default folder if it exists
        default_path = os.path.join(os.getcwd(), "audio_files")
        if os.path.exists(default_path):
            self.scan_folder()
    
    def browse_folder(self):
        """Browse for audio folder"""
        folder = filedialog.askdirectory(
            title="Select Audio Files Folder",
            initialdir=self.folder_var.get() if os.path.exists(self.folder_var.get()) else os.getcwd()
        )
        if folder:
            self.folder_var.set(folder)
            # Automatically scan the new folder
            self.scan_folder()
    
    def get_audio_files(self, folder_path):
        """Get all audio files in the specified folder"""
        audio_extensions = ['*.wav', '*.mp3', '*.flac', '*.ogg', '*.aiff', '*.au', '*.m4a']
        audio_files = set()
        
        for extension in audio_extensions:
            pattern = os.path.join(folder_path, extension)
            audio_files.update(glob.glob(pattern, recursive=False))
            # Also check for uppercase extensions
            pattern_upper = os.path.join(folder_path, extension.upper())
            audio_files.update(glob.glob(pattern_upper, recursive=False))
        
        return sorted(list(audio_files))
    
    def scan_folder(self):
        """Scan the selected folder for audio files and decode them"""
        folder_path = self.folder_var.get().strip()
        
        # Convert to absolute path if it's relative
        if not os.path.isabs(folder_path):
            folder_path = os.path.abspath(folder_path)
        
        if not os.path.exists(folder_path):
            messagebox.showerror("Error", f"Folder '{folder_path}' does not exist")
            return
        
        self.current_folder = folder_path
        
        # Clear previous results
        self.tree.delete(*self.tree.get_children())
        self.decoded_results = []
        self.groups = {}
        
        # Clear details text
        self.details_text.delete(1.0, tk.END)
        
        # Start scanning in a separate thread to avoid freezing the GUI
        threading.Thread(target=self._scan_worker, args=(folder_path,), daemon=True).start()
    
    def _scan_worker(self, folder_path):
        """Worker thread for scanning and decoding audio files"""
        try:
            # Update UI from main thread
            self.root.after(0, lambda: self.progress_var.set("Scanning for audio files..."))
            
            audio_files = self.get_audio_files(folder_path)
            
            if not audio_files:
                self.root.after(0, lambda: messagebox.showinfo("Info", f"No audio files found in '{folder_path}'"))
                self.root.after(0, lambda: self.progress_var.set("No audio files found"))
                return
            
            # Group files by base name (before S12/S34)
            file_pairs = {}
            single_files = []
            
            for file_path in audio_files:
                filename = os.path.basename(file_path)
                name_without_ext = os.path.splitext(filename)[0]
                
                if name_without_ext.endswith('S12'):
                    base_name = name_without_ext[:-3]  # Remove 'S12'
                    if base_name not in file_pairs:
                        file_pairs[base_name] = {}
                    file_pairs[base_name]['s12'] = file_path
                elif name_without_ext.endswith('S34'):
                    base_name = name_without_ext[:-3]  # Remove 'S34'
                    if base_name not in file_pairs:
                        file_pairs[base_name] = {}
                    file_pairs[base_name]['s34'] = file_path
                else:
                    single_files.append(file_path)
            
            # Create list of files to process
            files_to_process = []
            
            # Add complete pairs (both S12 and S34)
            for base_name, pair in file_pairs.items():
                if 's12' in pair and 's34' in pair:
                    # Process both files in the pair
                    files_to_process.append({
                        'file': pair['s12'],
                        'type': 's12',
                        'base_name': base_name,
                        'pair_file': pair['s34']
                    })
                    files_to_process.append({
                        'file': pair['s34'],
                        'type': 's34',
                        'base_name': base_name,
                        'pair_file': pair['s12']
                    })
                else:
                    # Incomplete pairs - treat as single files
                    if 's12' in pair:
                        single_files.append(pair['s12'])
                    if 's34' in pair:
                        single_files.append(pair['s34'])
            
            # Add single files
            for file_path in single_files:
                files_to_process.append({
                    'file': file_path,
                    'type': 'single',
                    'base_name': os.path.splitext(os.path.basename(file_path))[0],
                    'pair_file': None
                })
            
            self.root.after(0, lambda: self.progress_var.set(f"Found {len(audio_files)} files ({len(file_pairs)} pairs). Starting decoding..."))
            
            # Decode each file
            all_decoded_bits = []
            
            for i, file_info in enumerate(files_to_process):
                file_to_decode = file_info['file']
                # Update progress
                progress_msg = f"Decoding {i+1}/{len(files_to_process)}: {os.path.basename(file_to_decode)}"
                self.root.after(0, lambda msg=progress_msg: self.progress_var.set(msg))
                
                try:
                    decoded_bits = DecoderNew.analyze_audio(
                        filename=file_to_decode,
                        chunk_duration=0.004,
                        freq1=21_000, 
                        freq2=23_000,
                        use_normalization=True,      # Enable normalization
                        norm_type="max",             # Choose 'rms' or 'max'
                        normalization_target=5.0,    # Target value for normalization
                        use_agc=False,               # AGC is optional, off by default
                        use_bandpass=True,           # Enable bandpass filter
                        lowcut=20000,                # Filter everything below 20kHz
                        verbose=False                # Set to False for GUI to avoid console spam
                    )
                    
                    if decoded_bits:
                        # Decode multiple 8-bit numbers like in DecoderNew.py
                        numbers = []
                        for j in range(0, len(decoded_bits), 8):
                            if j + 8 <= len(decoded_bits):  # Only process complete 8-bit chunks
                                byte_bits = decoded_bits[j:j+8]
                                number = sum(bit * (2 ** idx) for idx, bit in enumerate(byte_bits))
                                numbers.append(number)
                        
                        # Validate first three numbers should be 1, 2, 3
                        is_valid = len(numbers) >= 3 and numbers[0] == 1 and numbers[1] == 2 and numbers[2] == 3
                        
                        # Extract session ID (4th number) if available
                        session_id = numbers[3] if len(numbers) >= 4 else None
                        
                        all_decoded_bits.append({
                            'file': file_to_decode,
                            'filename': os.path.basename(file_to_decode),
                            'bits': decoded_bits,
                            'number': numbers[0] if numbers else None,  # Keep for compatibility
                            'numbers': numbers,
                            'session_id': session_id,
                            'is_valid': is_valid,
                            'file_type': file_info['type'],
                            'base_name': file_info['base_name'],
                            'pair_file': file_info['pair_file']
                        })
                    else:
                        all_decoded_bits.append({
                            'file': file_to_decode,
                            'filename': os.path.basename(file_to_decode),
                            'bits': None,
                            'number': None,
                            'numbers': None,
                            'session_id': None,
                            'is_valid': False,
                            'file_type': file_info['type'],
                            'base_name': file_info['base_name'],
                            'pair_file': file_info['pair_file']
                        })
                        
                except Exception as e:
                    print(f"Error decoding {file_to_decode}: {e}")
                    all_decoded_bits.append({
                        'file': file_to_decode,
                        'filename': os.path.basename(file_to_decode),
                        'bits': None,
                        'number': None,
                        'numbers': None,
                        'session_id': None,
                        'is_valid': False,
                        'error': str(e),
                        'file_type': file_info['type'],
                        'base_name': file_info['base_name'],
                        'pair_file': file_info['pair_file']
                    })
            
            # Update progress
            self.root.after(0, lambda: self.progress_var.set("Grouping files by decoded values..."))
            
            # Group files by session ID (4th decoded number) and file pairs
            groups = {}
            valid_files = []
            invalid_files = []
            
            for result in all_decoded_bits:
                if result.get('numbers') is not None and result.get('is_valid', False):
                    # Valid files with proper 1,2,3 detection sequence
                    valid_files.append(result)
                    session_id = result.get('session_id')
                    if session_id is not None:
                        # Group by session ID
                        if session_id not in groups:
                            groups[session_id] = []
                        groups[session_id].append(result)
                    else:
                        # Files with valid detection but no session ID
                        if 'no_session_id' not in groups:
                            groups['no_session_id'] = []
                        groups['no_session_id'].append(result)
                elif result.get('numbers') is not None:
                    # Files with decoded numbers but invalid detection sequence
                    invalid_files.append(result)
                    if 'invalid_detection' not in groups:
                        groups['invalid_detection'] = []
                    groups['invalid_detection'].append(result)
                else:
                    # Files with no decoded value go to a special group
                    if 'no_decode' not in groups:
                        groups['no_decode'] = []
                    groups['no_decode'].append(result)
            
            # Update UI from main thread
            self.root.after(0, lambda: self._update_tree_results(groups, all_decoded_bits))
            self.root.after(0, lambda: self.progress_var.set(f"Scanned {len(audio_files)} files → {len(groups)} groups"))
            
        except Exception as e:
            error_msg = f"Error during scanning: {e}"
            self.root.after(0, lambda: messagebox.showerror("Error", error_msg))
            self.root.after(0, lambda: self.progress_var.set("Scan failed"))
    
    def _update_tree_results(self, groups, all_decoded_bits):
        """Update the tree view with results"""
        self.groups = groups
        self.decoded_results = all_decoded_bits
        
        # Clear tree
        self.tree.delete(*self.tree.get_children())
        
        if not groups:
            return
        
        # Add groups to tree
        # Sort groups with custom key to handle mixed types
        def sort_key(item):
            key = item[0]
            if key == 'no_decode':
                return (3, key)  # Put 'no_decode' at the end
            elif key == 'invalid_detection':
                return (2, key)  # Put 'invalid_detection' second to last
            elif key == 'no_session_id':
                return (1, key)  # Put 'no_session_id' third to last
            else:
                return (0, key)  # Put session IDs first, sorted by ID
        
        for group_value, results in sorted(groups.items(), key=sort_key):
            # Check if concatenated file exists
            concat_status = self._check_concat_file_exists(group_value)
            
            if group_value == 'no_decode':
                group_text = "No Decode"
                value_text = "-"
            elif group_value == 'invalid_detection':
                group_text = "Invalid Detection"
                value_text = "Bad 1,2,3"
            elif group_value == 'no_session_id':
                group_text = "No Session ID"
                value_text = "Missing 4th"
            else:
                # This is a session ID
                group_text = f"Session {group_value}"
                value_text = f"ID: {group_value}"
            
            # Insert group node
            group_id = self.tree.insert("", "end", text=group_text, 
                                      values=(value_text, len(results), concat_status))
            
            # Add individual files as children
            for result in results:
                file_text = result['filename']
                if result.get('error'):
                    file_text += f" (Error: {result['error']})"
                elif result.get('numbers'):
                    session_info = f"Session {result.get('session_id', '?')}" if result.get('session_id') is not None else "No Session"
                    file_text += f" → {session_info}"
                    if not result.get('is_valid', False):
                        file_text += " (Invalid detection sequence)"
                elif result.get('bits'):
                    file_text += f" → {result['bits']}"
                
                self.tree.insert(group_id, "end", text=file_text, 
                               values=("", "", ""))
    
    def _check_concat_file_exists(self, group_value):
        """Check if concatenated file exists for a group"""
        if not self.current_folder:
            return "Unknown"
            
        temp_folder = os.path.join(self.current_folder, "temp")
        
        if group_value == 'no_decode':
            concat_file = os.path.join(temp_folder, "concatenated_no_decode.wav")
        elif group_value == 'invalid_detection':
            concat_file = os.path.join(temp_folder, "concatenated_invalid_detection.wav")
        elif group_value == 'no_session_id':
            concat_file = os.path.join(temp_folder, "concatenated_no_session_id.wav")
        else:
            # group_value is a session ID
            concat_file = os.path.join(temp_folder, f"concatenated_session_{group_value}.wav")
        
        return "Exists" if os.path.exists(concat_file) else "Not Created"
    
    def on_tree_select(self, event):
        """Handle tree selection"""
        selection = self.tree.selection()
        if not selection:
            return
        
        item = selection[0]
        item_text = self.tree.item(item, "text")
        parent = self.tree.parent(item)
        
        if parent:  # This is a file (child item)
            # Find the file in our results
            for result in self.decoded_results:
                if result['filename'] in item_text:
                    self.show_file_details(result)
                    break
        else:  # This is a group (parent item)
            self.show_group_details(item_text)
    
    def show_file_details(self, result):
        """Show details for a single file"""
        self.details_text.delete(1.0, tk.END)
        
        details = f"File: {result['filename']}\n"
        details += f"Full Path: {result['file']}\n"
        
        # Add stereo pair information
        if result.get('file_type') in ['s12', 's34']:
            details += f"File Type: {result['file_type'].upper()} (Channels {result['file_type'][1]}&{result['file_type'][2]})\n"
            details += f"Base Name: {result['base_name']}\n"
            if result.get('pair_file'):
                pair_filename = os.path.basename(result['pair_file'])
                pair_exists = os.path.exists(result['pair_file'])
                details += f"Pair File: {pair_filename} ({'Exists' if pair_exists else 'Missing'})\n"
        elif result.get('file_type') == 'single':
            details += "File Type: Single file (no S12/S34 pair)\n"
        
        details += "\n"
        
        if result.get('error'):
            details += f"Error: {result['error']}\n"
        elif result.get('numbers') is not None:
            details += f"Decoded Numbers: {result['numbers']}\n"
            details += f"Detection Sequence Valid: {result.get('is_valid', False)}\n"
            if len(result['numbers']) >= 3:
                details += f"Detection Numbers: {result['numbers'][:3]}\n"
                if result['numbers'][:3] == [1, 2, 3]:
                    details += "✓ Correct detection sequence (1, 2, 3)\n"
                else:
                    details += "✗ Invalid detection sequence (should be 1, 2, 3)\n"
        
            session_id = result.get('session_id')
            if session_id is not None:
                details += f"Session ID: {session_id}\n"
            else:
                details += "Session ID: Not available (need at least 4 numbers)\n"
                
            details += f"Total numbers decoded: {len(result['numbers'])}\n"
            if result.get('bits'):
                details += f"Raw bits: {result['bits']}\n"
        elif result.get('bits') is not None:
            details += f"Decoded Bits: {result['bits']}\n"
            if result.get('number') is not None:
                details += f"Decoded Value: {result['number']}\n"
        else:
            details += "No signal detected or decoding failed\n"
        
        self.details_text.insert(1.0, details)
    
    def show_group_details(self, group_text):
        """Show details for a group"""
        self.details_text.delete(1.0, tk.END)
        
        # Find the group
        for group_value, results in self.groups.items():
            if group_value == 'no_decode':
                expected_text = "No Decode"
            elif group_value == 'invalid_detection':
                expected_text = "Invalid Detection"
            elif group_value == 'no_session_id':
                expected_text = "No Session ID"
            else:
                expected_text = f"Session {group_value}"
                
            if expected_text in group_text:
                details = f"Group: {expected_text}\n"
                if group_value == 'no_decode':
                    details += "Files that could not be decoded\n"
                elif group_value == 'invalid_detection':
                    details += "Files with invalid detection sequence (first 3 numbers should be 1, 2, 3)\n"
                elif group_value == 'no_session_id':
                    details += "Files with valid detection sequence but missing session ID (4th number)\n"
                else:
                    details += f"Session ID: {group_value}\n"
                    details += "Files with valid detection sequence (1, 2, 3) and this session ID\n"
                
                details += f"File Count: {len(results)}\n"
                
                # Count stereo pairs
                s12_files = [r for r in results if r.get('file_type') == 's12']
                s34_files = [r for r in results if r.get('file_type') == 's34']
                single_files = [r for r in results if r.get('file_type') == 'single']
                
                details += f"  • S12 files (channels 1&2): {len(s12_files)}\n"
                details += f"  • S34 files (channels 3&4): {len(s34_files)}\n"
                details += f"  • Single files: {len(single_files)}\n"
                
                # Check for complete pairs
                complete_pairs = 0
                s12_base_names = {r['base_name'] for r in s12_files}
                s34_base_names = {r['base_name'] for r in s34_files}
                complete_pair_names = s12_base_names.intersection(s34_base_names)
                complete_pairs = len(complete_pair_names)
                
                if complete_pairs > 0:
                    details += f"  • Complete pairs (S12+S34): {complete_pairs}\n"
                
                details += "\nFiles in this group:\n"
                
                # Group by base name for better display
                by_base_name = {}
                for result in results:
                    base_name = result.get('base_name', 'unknown')
                    if base_name not in by_base_name:
                        by_base_name[base_name] = []
                    by_base_name[base_name].append(result)
                
                for base_name, base_results in sorted(by_base_name.items()):
                    if len(base_results) > 1:
                        # Multiple files with same base name (stereo pair)
                        details += f"  📁 {base_name}:\n"
                        for result in sorted(base_results, key=lambda x: x.get('file_type', '')):
                            file_type = result.get('file_type', 'unknown')
                            session_info = f" [Session {result.get('session_id', '?')}]" if result.get('session_id') is not None else " [No Session]"
                            details += f"    • {result['filename']} ({file_type.upper()}){session_info}\n"
                    else:
                        # Single file
                        result = base_results[0]
                        file_type = result.get('file_type', 'unknown')
                        session_info = f" [Session {result.get('session_id', '?')}]" if result.get('session_id') is not None else " [No Session]"
                        details += f"  • {result['filename']} ({file_type.upper()}){session_info}\n"
                
                # Check concat file status
                concat_status = self._check_concat_file_exists(group_value)
                details += f"\nConcatenated File Status: {concat_status}"
                
                if concat_status == "Exists":
                    if group_value == 'no_decode':
                        concat_file = "concatenated_no_decode.wav"
                    elif group_value == 'invalid_detection':
                        concat_file = "concatenated_invalid_detection.wav"
                    elif group_value == 'no_session_id':
                        concat_file = "concatenated_no_session_id.wav"
                    else:
                        concat_file = f"concatenated_session_{group_value}.wav"
                    details += f"\nFile: {concat_file}"
                
                self.details_text.insert(1.0, details)
                break
    
    def on_double_click(self, event):
        """Handle double-click on tree items"""
        self.launch_selected()
    
    def get_selected_group_value(self):
        """Get the group value of the currently selected item"""
        selection = self.tree.selection()
        if not selection:
            return None
        
        item = selection[0]
        parent = self.tree.parent(item)
        
        # If it's a child item, get the parent
        if parent:
            item = parent
        
        item_text = self.tree.item(item, "text")
        
        # Find matching group
        for group_value in self.groups.keys():
            if group_value == 'no_decode':
                expected_text = "No Decode"
            elif group_value == 'invalid_detection':
                expected_text = "Invalid Detection"
            elif group_value == 'no_session_id':
                expected_text = "No Session ID"
            else:
                expected_text = f"Session {group_value}"
                
            if expected_text in item_text:
                return group_value
        
        return None
    
    def create_selected_concat(self):
        """Create concatenated file for the selected group"""
        group_value = self.get_selected_group_value()
        if group_value is None:
            messagebox.showwarning("Warning", "Please select a group first")
            return
        
        self.create_concat_file(group_value)
    
    def create_concat_file(self, group_value):
        """Create concatenated file for a specific group"""
        if group_value not in self.groups:
            messagebox.showerror("Error", f"Group {group_value} not found")
            return
        
        results = self.groups[group_value]
        file_paths = [result['file'] for result in results]
        
        self.progress_var.set(f"Creating concatenated file for group {group_value}...")
        
        try:
            concat_file = concatenate_audio_files_for_group(self.current_folder, file_paths, group_value)
            
            if concat_file:
                messagebox.showinfo("Success", f"Concatenated file created: {os.path.basename(concat_file)}")
                # Update the tree to reflect the new status
                self._update_tree_results(self.groups, self.decoded_results)
            else:
                messagebox.showerror("Error", f"Failed to create concatenated file for group {group_value}")
                
        except Exception as e:
            messagebox.showerror("Error", f"Error creating concatenated file: {e}")
        
        self.progress_var.set("Ready")
    
    def launch_selected(self):
        """Launch the selected group's concatenated file in Audacity"""
        group_value = self.get_selected_group_value()
        if group_value is None:
            messagebox.showwarning("Warning", "Please select a group first")
            return
        
        # Check if concatenated file exists
        temp_folder = os.path.join(self.current_folder, "temp")
        
        if group_value == 'no_decode':
            concat_file = os.path.join(temp_folder, "concatenated_no_decode.wav")
        elif group_value == 'invalid_detection':
            concat_file = os.path.join(temp_folder, "concatenated_invalid_detection.wav")
        elif group_value == 'no_session_id':
            concat_file = os.path.join(temp_folder, "concatenated_no_session_id.wav")
        else:
            concat_file = os.path.join(temp_folder, f"concatenated_session_{group_value}.wav")
        
        if not os.path.exists(concat_file):
            # Ask if user wants to create it
            result = messagebox.askyesno("File Not Found", 
                                       f"Concatenated file for group {group_value} doesn't exist. "
                                       "Do you want to create it first?")
            if result:
                self.create_concat_file(group_value)
                # Check again if file was created
                if not os.path.exists(concat_file):
                    return
            else:
                return
        
        # Launch in Audacity
        self.progress_var.set("Launching Audacity...")
        
        try:
            success = audacity_launcher.launch_audacity(concat_file)
            if success:
                messagebox.showinfo("Success", "Audacity launched successfully!")
            else:
                messagebox.showerror("Error", "Failed to launch Audacity")
        except Exception as e:
            messagebox.showerror("Error", f"Error launching Audacity: {e}")
        
        self.progress_var.set("Ready")
    
    def create_all_and_launch(self):
        """Create all concatenated files and launch the first one"""
        if not self.groups:
            messagebox.showwarning("Warning", "No groups found. Please scan a folder first.")
            return
        
        self.progress_var.set("Creating all concatenated files...")
        
        created_files = []
        
        for group_value in self.groups.keys():
            try:
                results = self.groups[group_value]
                file_paths = [result['file'] for result in results]
                
                concat_file = concatenate_audio_files_for_group(self.current_folder, file_paths, group_value)
                
                if concat_file:
                    created_files.append(concat_file)
                
            except Exception as e:
                print(f"Error creating file for group {group_value}: {e}")
        
        if created_files:
            # Update tree
            self._update_tree_results(self.groups, self.decoded_results)
            
            # Launch first file
            try:
                success = audacity_launcher.launch_audacity(created_files[0])
                if success:
                    messagebox.showinfo("Success", 
                                      f"Created {len(created_files)} concatenated files and "
                                      f"launched the first one in Audacity!")
                else:
                    messagebox.showwarning("Partial Success", 
                                         f"Created {len(created_files)} concatenated files but "
                                         f"failed to launch Audacity")
            except Exception as e:
                messagebox.showerror("Error", f"Created files but error launching Audacity: {e}")
        else:
            messagebox.showerror("Error", "Failed to create any concatenated files")
        
        self.progress_var.set("Ready")


def main():
    """Main function to run the GUI"""
    root = tk.Tk()
    app = AudioDecoderGUI(root)
    root.mainloop()


if __name__ == "__main__":
    main()
