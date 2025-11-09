# WiFi Communication Setup Guide

## Overview

The system has been updated to use **WiFi communication** between ESP32-CAM and Arduino Nano IoT instead of Hardware Serial (Serial1). This eliminates the need for a physical serial connection between the two devices.

## Architecture

### Communication Flow:
```
ESP32-CAM (WiFi Client) ──WiFi──> Arduino Nano IoT (WiFi AP)
                                      ├── Port 8080: Laptop Control
                                      └── Port 8081: ESP32-CAM QR Data
```

### Network Configuration:
- **Arduino Nano IoT**: Creates WiFi Access Point (AP)
  - SSID: `Nano_[YOUR_NAME]_AP` (configurable in `onboard_receiver.ino`)
  - Password: `nano1pwd` (configurable)
  - IP Address: `192.168.4.1`
  - Port 8080: Laptop control commands
  - Port 8081: ESP32-CAM QR data

- **ESP32-CAM**: Connects to Arduino's WiFi AP
  - Connects as WiFi Station (STA)
  - Connects to Arduino at `192.168.4.1:8081`
  - Sends QR code data via TCP

- **Laptop**: Connects to Arduino's WiFi AP
  - Connects to Arduino at `192.168.4.1:8080`
  - Sends control commands via TCP

## Configuration

### ESP32-CAM Configuration (`QR_demo/QR_demo.ino`)

Update these constants to match your Arduino's WiFi AP:

```cpp
const char* ssid = "Nano_[YOUR_NAME]_AP";  // Match Arduino's AP name
const char* password = "nano1pwd";          // Match Arduino's AP password
const char* arduino_ip = "192.168.4.1";    // Arduino's IP address
const int arduino_port = 8081;              // Port for QR data
```

### Arduino Configuration (`onboard_receiver/onboard_receiver.ino`)

Update these constants to match your desired WiFi AP:

```cpp
char ssid[] = "Nano_[YOUR_NAME]_AP";
char pass[] = "nano1pwd";
```

**Important**: The ESP32-CAM's `ssid` and `password` must match the Arduino's `ssid` and `pass`!

## Setup Instructions

### 1. Upload Arduino Code
1. Open `onboard_receiver/onboard_receiver.ino` in Arduino IDE
2. Update WiFi credentials (lines 9-10) if needed
3. Upload to Arduino Nano IoT
4. Open Serial Monitor (9600 baud) to verify AP creation

### 2. Upload ESP32-CAM Code
1. Open `QR_demo/QR_demo.ino` in Arduino IDE
2. **IMPORTANT**: Update WiFi credentials (lines 58-59) to match Arduino's AP:
   ```cpp
   const char* ssid = "Nano_[YOUR_NAME]_AP";  // Must match Arduino!
   const char* password = "nano1pwd";          // Must match Arduino!
   ```
3. Upload to ESP32-CAM
4. Open Serial Monitor (115200 baud) to verify connection

### 3. Verify Connection

**Arduino Serial Monitor (9600 baud):**
- Should show: "Creating an access point..."
- Should show: "AP IP Address: 192.168.4.1"
- Should show: "WiFi servers started:"
- Should show: "Port 8080: Laptop control"
- Should show: "Port 8081: ESP32-CAM QR data"
- When ESP32-CAM connects: "ESP32-CAM connected!"

**ESP32-CAM Serial Monitor (115200 baud):**
- Should show: "Connecting to Arduino WiFi AP"
- Should show: "[OK] WiFi connected"
- Should show: "IP Address: 192.168.4.x"
- Should show: "Connecting to Arduino at 192.168.4.1:8081..."
- Should show: "[OK] Connected to Arduino"
- When QR codes detected: "QR:FRONT,<cx>,<cy>,<width>,<height>"

## Troubleshooting

### ESP32-CAM Won't Connect to WiFi

**Problem**: ESP32-CAM can't connect to Arduino's WiFi AP

**Solutions**:
1. Verify SSID and password match exactly in both files
2. Check Arduino is powered and AP is running (check Serial Monitor)
3. Check ESP32-CAM is within WiFi range
4. Verify Arduino's WiFi AP is actually running (check Serial Monitor)
5. Try restarting both devices

### ESP32-CAM Connects to WiFi but Not Arduino

**Problem**: WiFi connected but can't connect to Arduino on port 8081

**Solutions**:
1. Verify Arduino's server on port 8081 is started (check Serial Monitor)
2. Check Arduino's IP address is `192.168.4.1`
3. Verify port number is 8081 in ESP32-CAM code
4. Check Arduino Serial Monitor for "ESP32-CAM connected!" message
5. Try restarting Arduino

### No QR Data Received

**Problem**: ESP32-CAM detects QR codes but Arduino doesn't receive data

**Solutions**:
1. Check ESP32-CAM Serial Monitor for QR detection messages
2. Verify ESP32-CAM is connected to Arduino (check both Serial Monitors)
3. Check Arduino Serial Monitor for connection status
4. Verify data format matches expected format: `QR:<id>,<cx>,<cy>,<width>,<height>\n`
5. Check for buffer overflow (increase buffer size if needed)

### Connection Drops

**Problem**: Connection works initially but drops after some time

**Solutions**:
1. Check WiFi signal strength
2. Verify both devices stay powered
3. Check for interference from other WiFi networks
4. The system has auto-reconnect - wait a few seconds
5. Check Serial Monitors for error messages

## Advantages of WiFi Communication

1. **No Physical Connection**: No need for Serial1 wire between devices
2. **Flexible Mounting**: ESP32-CAM can be mounted anywhere on robot
3. **Easier Debugging**: Can monitor both devices via Serial Monitor
4. **Scalability**: Can add more devices to the network if needed
5. **Range**: Better range than serial connection

## Network Topology

```
                    Arduino Nano IoT (AP)
                    IP: 192.168.4.1
                    ├── Port 8080 (Laptop Control)
                    └── Port 8081 (ESP32-CAM)
                           │
        ┌──────────────────┼──────────────────┐
        │                  │                  │
   Laptop            ESP32-CAM          (Future devices)
   (Client)          (Client)
```

## Testing

1. **Test WiFi Connection**:
   - Upload both codes
   - Check Serial Monitors for connection messages
   - Verify "ESP32-CAM connected!" appears in Arduino Serial Monitor

2. **Test QR Data Transmission**:
   - Show QR code to ESP32-CAM
   - Check ESP32-CAM Serial Monitor for QR detection
   - Check Arduino Serial Monitor for received data
   - Verify navigation system processes the data

3. **Test Autonomous Mode**:
   - Enable autonomous mode (press 'm' via laptop)
   - Verify robot responds to QR code data
   - Check for smooth navigation behavior

## Notes

- Both ESP32-CAM and laptop connect to the same WiFi AP
- ESP32-CAM uses port 8081, laptop uses port 8080
- No physical serial connection needed between ESP32-CAM and Arduino
- WiFi credentials must match exactly in both files
- Connection is automatically maintained and reconnected if dropped

