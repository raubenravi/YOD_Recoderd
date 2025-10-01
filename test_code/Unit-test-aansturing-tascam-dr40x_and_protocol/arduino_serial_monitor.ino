/*
 * Arduino Uno Serial Data Reader
 * This sketch reads serial data and displays it on the Serial Monitor
 * Useful for monitoring UART communication from ESP32 or other devices
 * 
 * Connections:
 * - Connect RX pin (Pin 0) to TX of the device you want to monitor
 * - Connect TX pin (Pin 1) to RX of the device you want to monitor (if bidirectional)
 * - Connect GND to GND of the device
 * 
 * For ESP32 UART monitoring:
 * - ESP32 GPIO17 (TX) -> Arduino Pin 0 (RX)
 * - ESP32 GPIO16 (RX) -> Arduino Pin 1 (TX) (optional)
 * - ESP32 GND -> Arduino GND
 */

void setup() {
  // Initialize Serial communication at 9600 baud rate
  // This is for communication with the Serial Monitor
  Serial.begin(9600);
  
  // Wait for serial port to connect (needed for some boards)
  while (!Serial) {
    ; // wait for serial port to connect
  }
  
  Serial.println("Arduino Serial Monitor Started");
  Serial.println("Listening for incoming serial data...");
  Serial.println("====================================");
}

void loop() {
  // Check if data is available to read
  if (Serial.available() > 0) {
    // Read the incoming data
    String incomingData = Serial.readString();
    
    // Remove any trailing whitespace
    incomingData.trim();
    
    // Display timestamp and data
    Serial.print("[");
    Serial.print(millis());
    Serial.print(" ms] Received: ");
    Serial.println(incomingData);
    
    // Optional: Echo the data back (useful for bidirectional communication)
    // Uncomment the next line if you want to echo data back
    // Serial.println(incomingData);
  }
  
  // Small delay to prevent overwhelming the serial buffer
  delay(10);
}