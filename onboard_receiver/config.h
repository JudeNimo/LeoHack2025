#ifndef CONFIG_H
#define CONFIG_H

// Navigation Configuration Parameters

// QR Code Detection
#define QR_TARGET_WIDTH_MIN 30      // Minimum QR code width (pixels) to consider valid
#define QR_TARGET_WIDTH_MAX 200     // Maximum QR code width (pixels) before too close
#define QR_TARGET_WIDTH_DOCK 80     // Target QR code width for docking (pixels)
#define QR_CENTER_TOLERANCE 20      // Tolerance for centering QR code (pixels)

// Proportional Control Gains
#define KP_X 0.3                    // Proportional gain for x-axis centering
#define KP_DISTANCE 0.4             // Proportional gain for distance control
#define KP_ROTATION 0.5             // Proportional gain for rotation

// Movement Speed Limits
#define SEARCH_ROTATION_SPEED 3    // Power level (0-9) for search rotation
#define APPROACH_SPEED 4            // Power level (0-9) for approaching target
#define ALIGN_SPEED 2               // Power level (0-9) for fine alignment
#define DOCK_SPEED 3                // Power level (0-9) for final docking

// Frame Dimensions (QVGA: 320x240)
#define FRAME_WIDTH 320
#define FRAME_HEIGHT 240
#define FRAME_CENTER_X (FRAME_WIDTH / 2)
#define FRAME_CENTER_Y (FRAME_HEIGHT / 2)

// State Machine Timing
#define QR_TIMEOUT_MS 2000          // Timeout if no QR code detected (ms)
#define DOCK_COMPLETE_WIDTH 90      // QR width threshold for docking complete

// QR Code Side Identification
// Define expected QR code payloads for each side
// These should match the actual QR codes on the passive robot
#define QR_SIDE_FRONT "FRONT"       // Front/docking side QR code
#define QR_SIDE_BACK "BACK"         // Back side QR code
#define QR_SIDE_LEFT "LEFT"         // Left side QR code
#define QR_SIDE_RIGHT "RIGHT"       // Right side QR code

// Serial Communication
#define SERIAL1_BAUD 115200         // Baud rate for ESP32-CAM communication

#endif



