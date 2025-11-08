# Quick Testing Reference

## Manual Movement Testing

### Setup:
1. Upload code to Arduino Nano IoT
2. Connect laptop to WiFi: `Nano_[YOUR_NAME]_AP` (password: `nano1pwd`)
3. Run: `python remote_control.py`

### Test Each Movement:
- **W** - Forward (start with power 2-3)
- **S** - Backward
- **Q** - Rotate Left
- **E** - Rotate Right
- **A** - Translate Left
- **D** - Translate Right
- **O** - Servo Open
- **P** - Servo Close
- **M** - Toggle Autonomous Mode
- **ESC** - Exit

### What to Check:
✅ Smooth movement (no jerking)
✅ Straight lines (no drift)
✅ Consistent speed
✅ Immediate stop on key release
✅ No overheating motors
✅ No strange noises

---

## Autonomous Mode Testing

### Setup:
1. Place passive robot with QR code visible
2. Place active robot 1-2 meters away
3. Ensure ESP32-CAM can see QR code
4. Check ESP32-CAM Serial Monitor (115200 baud) for QR detection

### Test Sequence:
1. Press **M** to enable autonomous mode
2. Robot should start rotating (searching)
3. When QR code found, robot should approach
4. Robot should center and align
5. Robot should dock smoothly
6. Press **M** again to disable

### What to Check:
✅ QR code detected (check ESP32-CAM Serial Monitor)
✅ Correct side identified (should be "FRONT")
✅ Smooth approach movement
✅ QR code centered in frame
✅ Smooth docking without overshoot
✅ State transitions visible in Arduino Serial Monitor

### Troubleshooting:
- **No QR detection:** Check ESP32-CAM Serial Monitor, verify lighting
- **Wrong side:** Update QR code payload or config.h definitions
- **Overshoot:** Reduce speeds in config.h
- **Stuck:** Check Serial Monitor for state information

---

## Serial Monitor Outputs

### Arduino Serial Monitor (9600 baud):
- System initialization messages
- WiFi AP status
- Command confirmations
- Autonomous mode status
- State transitions

### ESP32-CAM Serial Monitor (115200 baud):
- Camera initialization
- QR code detection: `QR:FRONT,<cx>,<cy>,<width>,<height>`
- Detection errors (if any)

---

## Emergency Stop
- Press **S** key (stop command)
- Or press **M** to disable autonomous mode
- Or disconnect power

