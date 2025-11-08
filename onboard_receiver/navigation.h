#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "config.h"

// Navigation State Machine States
enum NavigationState {
  STATE_SEARCHING,      // Rotating to find QR code
  STATE_TARGET_FOUND,   // QR code detected, checking if correct side
  STATE_APPROACHING,    // Moving toward target
  STATE_ALIGNING,       // Fine-tuning position and orientation
  STATE_DOCKING,        // Final approach for docking
  STATE_DOCKED,         // Successfully docked
  STATE_LOST            // Lost QR code, return to search
};

// QR Code Data Structure
struct QRData {
  char id[32];          // QR code payload/ID
  int center_x;         // Center X coordinate in frame
  int center_y;         // Center Y coordinate in frame
  int width;            // QR code width in pixels
  int height;           // QR code height in pixels
  bool valid;           // Whether data is valid
  unsigned long timestamp; // Last update timestamp
};

// Navigation Functions
void navigation_init();
void navigation_update();
void process_qr_data(const char* data);
bool is_correct_docking_side(const char* qr_id);
NavigationState get_navigation_state();
QRData get_current_qr_data();

#endif



