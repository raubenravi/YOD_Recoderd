# Reference Manual
This manual is for software developers who want to understand the structure of the code. To read it properly, use a reader that supports Markdown.
# Task Handling

After all objects are constructed, the `startTasks()` function from the Task handler is called.

The TaskHandler controls all tasks using the ESP-IDF FreeRTOS framework.

There are two tasks running on the ESP: one task is responsible for general control of the system and its peripherals (`observerUpdateTask`).


## Observer Update Task
The `observerUpdateTask()` has 4 observers and one listener.
When the update function runs, the observers are stored in a vector because it's easier for development, but should be transferred to an array when memory becomes an issue or when it goes to production. When a new implementation of the observer is made, a new ID should be added to `ObserverIds`.

The observers are checked every 200 ms.

# Audio Analyser Task
The second task that is running is a task to count the silence-to-speaking ratio.
Every 250 ms, the audio analyser measures whether the speaker is silent or speaking.



# Observer-Listener Pattern

An example of how to make a new Observer:

```
//found in Observer.hpp
enum class ObserverIds {

    Start,

    Stop,

    PatientPause,

    ResearchPause,

    CodeScanner,

    AudioDataAvailable

/* add new id here */

};

class MyObserver: public Observer {

  ObserverId id;

  void update(){

  if() // Here should be the logic to check if Observer measured an event
  {
    listener->notify(id);
 }
};

};

```
There are two listeners: the menu controller and display controller.


# Button Handling

The buttons in the YOD recorder are polled with interrupt pins and integrated with the listener pattern. When a button is pressed, a flag will be set by the interrupt. When the `update()` function is called, the flag will be read and if necessary, it will call the `menuController.notify()` to update its state. 

# Menu Controller
The `MenuController` class contains most of the business logic. It contains an enum with the state of the recorder. The state can be changed by calling `notify()`. After `notify()` is called, the `menuTask` is called to update the state machine.

The `startRecording()` function is responsible for ensuring that the audio modem has the right data and that the right `TascamBoundary` is called.



The Tascam recorder used in the project (Tascam DR-40X) can be controlled with UART (see hardware documentation for how to connect it).
The Tascam has 3 states: stop, idle, and recording.
The `TascamBoundary` class can control and set the recorder to any of the 3 states independently of the current state, because it doesn't keep track of the state. This means that if one transition goes wrong, it doesn't affect the next transition.
 

## Tascam Control
The `TascamBoundary` class is responsible for sending the right data via UART to the Tascam recorder.

The `uart_set_line_inverse` is called because on the ESP32 it's already inverted. The UART signal should be high on 1, low on 0.
| Tascam command | Uart value |
|--|--|
| Start | 107 |
| Stop | 104 |

## UART Settings:

| Setting | Value |
|--|--|
| Rx pin | 16 |
| Tx pin | 17  |
| Baud rate | 9600 |
| Data bits | 8 |
| Parity | Even |

# PWM Channel

The `PWMChannel` class provides a thread-safe interface for controlling PWM (Pulse Width Modulation) signals using the ESP32's LEDC (LED Control) peripheral. This base class is designed to be inherited by other components that need PWM functionality.

## Key Features:
- Thread-safe operations using FreeRTOS mutexes
- Configurable frequency, duty cycle, and resolution
- Uses ESP32's LEDC peripheral for precise PWM control
- Default configuration: 10kHz frequency, 5-bit resolution
- Shared mutex across all PWM instances for safe multi-threaded access

The PWM channel is automatically paused after initialization and must be explicitly resumed by derived classes when needed.

# Speaker

The `Speaker` class extends `PWMChannel` to provide audio feedback functionality. It generates beep sounds at specified frequencies through a connected speaker.

## Functionality:
- Inherits thread-safe PWM control from `PWMChannel`
- `beep(frequency)` method for generating audio tones
- Used for user interface feedback and system notifications
- Configurable frequency for different beep tones

The speaker is typically used to provide audio confirmation when buttons are pressed or when system events occur.

# Audio Modem

The `AudioModem` class extends `PWMChannel` to implement Frequency Shift Keying (FSK) modulation for data transmission over audio channels. This enables the YOD recorder to transmit digital data as audio signals.

## Key Features:
- FSK modulation using two frequencies (high and low)
- Transmits 8-bit data values bit by bit
- Each bit is transmitted for 20ms duration
- Thread-safe transmission using mutex protection
- Automatically pauses PWM after transmission

## Operation:
1. Takes a mutex lock to ensure exclusive access
2. Resumes PWM signal generation
3. For each bit in the 8-bit data:
   - Sets high frequency for bit '1'
   - Sets low frequency for bit '0'
   - Waits 20ms before next bit
4. Pauses PWM signal after transmission
5. Releases mutex lock

This component is essential for encoding patient and research data into audio signals that can be recorded by the Tascam recorder and later decoded for analysis.






