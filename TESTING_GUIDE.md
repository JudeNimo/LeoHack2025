# Robot Testing Guide

This guide provides step-by-step instructions for testing both manual and autonomous robot control.

## Prerequisites

1. **Hardware Setup:**
   - Arduino Nano IoT connected and powered
   - ESP32-CAM connected to Arduino via Serial1 (TX pin of ESP32-CAM to RX pin of Arduino)
   - Motors connected to motor control pins
   - Robot on a safe, clear testing surface

2. **Software Setup:**
   - Arduino IDE with code uploaded to Arduino Nano IoT
   - ESP32-CAM code uploaded to ESP32-CAM module
   - Python 3 installed with required packages:
     ```bash
     pip install keyboard
     ```

3. **Network Setup:**
   - Laptop/computer connected to Arduino's WiFi AP
   - Default network: `Nano_[YOUR_NAME]_AP` (password: `nano1pwd`)
   - Arduino IP: `192.168.4.1`

---

## Part 1: Testing Manual Movement

### Step 1: Initial Setup

1. **Upload Arduino Code:**
   - Open `onboard_receiver/onboard_receiver.ino` in Arduino IDE
   - Update WiFi credentials if needed (lines 9-10)
   - Upload to Arduino Nano IoT
   - Open Serial Monitor (9600 baud) to see status messages

2. **Upload ESP32-CAM Code:**
   - Open `QR_demo/QR_demo.ino` in Arduino IDE
   - Upload to ESP32-CAM module
   - Open Serial Monitor (115200 baud) to verify QR detection

3. **Connect to WiFi:**
   - Connect your laptop to the Arduino's WiFi network
   - Verify connection by checking network settings

### Step 2: Test Basic Movement

**Start with low power settings to avoid damage:**

1. **Run Python Control Script:**
   ```bash
   python remote_control.py
   ```

2. **Test Forward Movement:**
   - Press and hold `W` key
   - **Observe:** Robot should move forward
   - **Check:** Does it move straight? Any drift?
   - Release key - robot should stop

3. **Test Backward Movement:**
   - Press and hold `S` key
   - **Observe:** Robot should move backward
   - **Check:** Same direction as forward? Any issues?

4. **Test Rotation:**
   - Press and hold `Q` (left rotation)
   - **Observe:** Robot should rotate counter-clockwise
   - **Check:** Smooth rotation? Consistent speed?
   - Test `E` (right rotation) similarly

5. **Test Translation:**
   - Press and hold `A` (translate left)
   - **Observe:** Robot should move sideways left
   - **Check:** Perpendicular to forward direction?
   - Test `D` (translate right) similarly

### Step 3: Things to Pay Attention To

#### Motor Behavior:
- ✅ **Smooth Operation:** Motors should start/stop smoothly without jerking
- ✅ **Consistent Speed:** Same power level should produce consistent speed
- ✅ **No Overheating:** Motors should not get excessively hot during testing
- ✅ **No Strange Noises:** Listen for grinding, clicking, or unusual sounds

#### Movement Accuracy:
- ✅ **Straight Forward/Backward:** Robot should move in a straight line
- ✅ **Rotation Center:** Robot should rotate around its center, not drift
- ✅ **Translation Perpendicular:** Lateral movement should be perpendicular to forward
- ✅ **Stop Response:** Robot should stop promptly when key is released

#### Communication:
- ✅ **Command Response:** Commands should execute immediately
- ✅ **No Delays:** No noticeable lag between key press and movement
- ✅ **Serial Monitor:** Check Arduino Serial Monitor for error messages
- ✅ **Connection Stability:** WiFi connection should remain stable

#### Power Levels:
- ✅ **Test Different Powers:** Try power levels 1-9 to find optimal settings
- ✅ **Low Power Test:** Start with power 2-3 for initial testing
- ✅ **High Power Test:** Gradually increase to test maximum speed
- ✅ **Battery Level:** Monitor battery voltage if applicable

### Step 4: Troubleshooting Manual Movement

**Problem: Robot doesn't move**
- Check motor connections
- Verify power supply
- Check Serial Monitor for error messages
- Verify WiFi connection

**Problem: Robot moves in wrong direction**
- Check motor wiring (may need to swap H-bridge connections)
- Verify motor pin assignments in `motor_control.cpp`

**Problem: Robot drifts when moving forward**
- Motors may have different speeds - adjust PWM values
- Check for mechanical issues (wheels, alignment)
- Calibrate motor speeds in code

**Problem: Robot doesn't stop when key released**
- Check Python script is sending stop command ('s')
- Verify Arduino is receiving commands
- Check Serial Monitor for command echo

**Problem: Jerky or uneven movement**
- Reduce power level
- Check for loose connections
- Verify PWM signals are stable

---

## Part 2: Testing Autonomous Mode

### Step 1: Prepare Test Environment

1. **Setup Passive Robot:**
   - Place passive robot in clear area
   - Ensure QR code is visible and properly lit
   - QR code should be on the docking side (e.g., "FRONT")
   - Distance: Start with 1-2 meters between robots

2. **Position Active Robot:**
   - Place active robot at semi-random position
   - Ensure ESP32-CAM has clear view
   - Robot should be able to see QR code when rotating

3. **Verify QR Detection:**
   - Check ESP32-CAM Serial Monitor (115200 baud)
   - Should see: `QR:FRONT,<cx>,<cy>,<width>,<height>`
   - Verify QR code is being detected consistently

### Step 2: Test Autonomous Mode Toggle

1. **Start Manual Control:**
   ```bash
   python remote_control.py
   ```

2. **Toggle Autonomous Mode:**
   - Press `M` key once
   - **Check Serial Monitor:** Should see "Autonomous mode: ON"
   - **Observe:** Robot should stop manual movement
   - Robot should start rotating (searching for QR code)

3. **Toggle Back to Manual:**
   - Press `M` key again
   - **Check Serial Monitor:** Should see "Autonomous mode: OFF"
   - **Observe:** Robot should stop
   - Manual commands should work again

### Step 3: Test Autonomous Docking Sequence

1. **Enable Autonomous Mode:**
   - Press `M` to enable autonomous mode
   - Robot should enter `STATE_SEARCHING`

2. **Observe Search Phase:**
   - Robot should rotate slowly
   - **Check:** Is rotation smooth? Appropriate speed?
   - **Wait:** Robot should detect QR code within reasonable time

3. **Observe Target Found Phase:**
   - When QR detected, robot should stop rotating
   - **Check Serial Monitor:** Should see state transitions
   - **Verify:** Correct QR code side detected? (Should be "FRONT")

4. **Observe Approach Phase:**
   - Robot should start moving toward target
   - **Check:** Is it moving in correct direction?
   - **Check:** Is it centering the QR code in frame?
   - **Observe:** Robot should slow down as it gets closer

5. **Observe Alignment Phase:**
   - Robot should fine-tune position
   - **Check:** QR code should be centered in camera frame
   - **Check:** Robot should adjust distance based on QR size
   - **Observe:** Small, precise movements

6. **Observe Docking Phase:**
   - Final approach to passive robot
   - **Check:** Smooth, controlled movement
   - **Check:** QR code size should increase (getting closer)
   - **Verify:** Robot stops when docked

7. **Verify Docking Complete:**
   - Robot should enter `STATE_DOCKED`
   - **Check Serial Monitor:** Should show "DOCKED" state
   - **Verify:** Robot is positioned correctly next to passive robot
   - **Check:** Physical alignment is correct

### Step 4: Things to Pay Attention To (Autonomous Mode)

#### QR Code Detection:
- ✅ **Consistent Detection:** QR code should be detected reliably
- ✅ **Correct ID:** QR code payload should match expected value
- ✅ **Frame Position:** QR code position in frame should be accurate
- ✅ **Size Calculation:** QR code width should correlate with distance

#### State Transitions:
- ✅ **Smooth Transitions:** State changes should be logical and timely
- ✅ **No Stuck States:** Robot shouldn't get stuck in one state
- ✅ **Error Recovery:** Robot should recover if QR code is lost
- ✅ **State Logging:** Check Serial Monitor for state information

#### Movement Behavior:
- ✅ **Proportional Control:** Movement should be smooth, not jerky
- ✅ **Centering Accuracy:** Robot should center QR code in frame
- ✅ **Distance Control:** Robot should maintain appropriate distance
- ✅ **Final Approach:** Smooth docking without overshooting

#### Edge Cases:
- ✅ **Lost QR Code:** Robot should return to search if QR lost
- ✅ **Wrong Side:** Robot should reject wrong QR code sides
- ✅ **Obstacles:** Test behavior if obstacle appears (if applicable)
- ✅ **Multiple QR Codes:** Behavior with multiple QR codes visible

### Step 5: Calibration Adjustments

If autonomous mode doesn't work correctly, adjust parameters in `config.h`:

**QR Detection Issues:**
```cpp
#define QR_TARGET_WIDTH_MIN 30      // Increase if too sensitive
#define QR_TARGET_WIDTH_MAX 200     // Adjust based on camera distance
#define QR_TARGET_WIDTH_DOCK 80     // Target size for docking
```

**Movement Speed:**
```cpp
#define SEARCH_ROTATION_SPEED 3    // Slower = more precise search
#define APPROACH_SPEED 4            // Adjust approach speed
#define ALIGN_SPEED 2               // Fine-tuning speed
#define DOCK_SPEED 3                // Final approach speed
```

**Control Gains:**
```cpp
#define KP_X 0.3                    // Increase for faster centering
#define KP_DISTANCE 0.4             // Increase for faster distance adjustment
```

**Timing:**
```cpp
#define QR_TIMEOUT_MS 2000          // Time before giving up on QR
#define DOCK_COMPLETE_WIDTH 90      // Size threshold for docking complete
```

### Step 6: Troubleshooting Autonomous Mode

**Problem: Robot doesn't find QR code**
- Check ESP32-CAM is detecting QR codes (Serial Monitor)
- Verify QR code is visible and well-lit
- Check Serial1 connection between ESP32-CAM and Arduino
- Increase `QR_TIMEOUT_MS` if needed
- Verify QR code payload matches `QR_SIDE_FRONT` in config.h

**Problem: Robot finds wrong side**
- Verify QR code payloads match config.h definitions
- Check `is_correct_docking_side()` function in navigation.cpp
- Update QR code side definitions if needed

**Problem: Robot overshoots target**
- Reduce `APPROACH_SPEED` and `DOCK_SPEED`
- Increase `KP_DISTANCE` for faster response
- Adjust `QR_TARGET_WIDTH_DOCK` threshold

**Problem: Robot doesn't center properly**
- Increase `KP_X` gain
- Check QR code position calculation
- Verify frame dimensions match camera resolution

**Problem: Robot gets stuck in one state**
- Check Serial Monitor for state information
- Verify state transition conditions in navigation.cpp
- Check for timeout conditions

**Problem: Jerky movement in autonomous mode**
- Reduce movement speeds
- Adjust proportional gains
- Check motor control smooth functions

---

## Testing Checklist

### Manual Mode Testing:
- [ ] Forward movement works
- [ ] Backward movement works
- [ ] Left rotation works
- [ ] Right rotation works
- [ ] Left translation works
- [ ] Right translation works
- [ ] Stop command works
- [ ] Servo controls work
- [ ] Movement is smooth
- [ ] No drift or unwanted movement
- [ ] Commands respond immediately

### Autonomous Mode Testing:
- [ ] Autonomous mode toggle works (M key)
- [ ] Search phase works (rotation)
- [ ] QR code detection works
- [ ] Correct side identification works
- [ ] Approach phase works
- [ ] Alignment phase works
- [ ] Docking phase works
- [ ] Docking complete detection works
- [ ] Lost QR code recovery works
- [ ] Wrong side rejection works

### Integration Testing:
- [ ] Manual mode disabled when autonomous active
- [ ] Stop command works in autonomous mode
- [ ] Mode switching is smooth
- [ ] No conflicts between modes
- [ ] Serial communication stable
- [ ] WiFi connection stable

---

## Safety Notes

1. **Always test in a safe, clear area**
2. **Start with low power settings**
3. **Have emergency stop ready (S key or disconnect power)**
4. **Monitor robot behavior closely during first tests**
5. **Check for overheating motors**
6. **Verify robot stops when commands stop**
7. **Test autonomous mode with supervision**

---

## Next Steps After Testing

1. **Calibrate Parameters:** Adjust config.h based on test results
2. **Fine-tune Gains:** Optimize proportional control gains
3. **Test Edge Cases:** Test various scenarios and positions
4. **Document Issues:** Note any problems for future fixes
5. **Optimize Performance:** Improve speed and accuracy based on results

