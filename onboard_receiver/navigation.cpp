#include <Arduino.h>
#include <string.h>
#include "navigation.h"
#include "motor_control.h"
#include "config.h"

// Global state
static NavigationState current_state = STATE_SEARCHING;
static QRData current_qr = {0};
static unsigned long last_qr_update = 0;
static unsigned long state_start_time = 0;

// Forward declarations for internal functions
void execute_search();
void execute_target_found();
void execute_approaching();
void execute_aligning();
void execute_docking();
void calculate_movement(int& forward_speed, int& rotation_speed, int& translate_speed);

void navigation_init() {
  current_state = STATE_SEARCHING;
  memset(&current_qr, 0, sizeof(QRData));
  current_qr.valid = false;
  last_qr_update = 0;
  state_start_time = millis();
}

void navigation_update() {
  unsigned long current_time = millis();
  
  // Check if QR data is stale (timeout)
  if (current_qr.valid && (current_time - last_qr_update > QR_TIMEOUT_MS)) {
    current_qr.valid = false;
    if (current_state != STATE_SEARCHING && current_state != STATE_DOCKED) {
      current_state = STATE_LOST;
      state_start_time = current_time;
    }
  }
  
  // State machine execution
  switch (current_state) {
    case STATE_SEARCHING:
      execute_search();
      break;
    case STATE_TARGET_FOUND:
      execute_target_found();
      break;
    case STATE_APPROACHING:
      execute_approaching();
      break;
    case STATE_ALIGNING:
      execute_aligning();
      break;
    case STATE_DOCKING:
      execute_docking();
      break;
    case STATE_DOCKED:
      stop();
      break;
    case STATE_LOST:
      // Lost QR code, return to search
      current_state = STATE_SEARCHING;
      state_start_time = current_time;
      stop();
      break;
  }
}

void process_qr_data(const char* data) {
  // Parse format: QR:<id>,<cx>,<cy>,<width>,<height>\n
  if (strncmp(data, "QR:", 3) != 0) {
    return; // Not a QR data line
  }
  
  // Manual parsing (sscanf may not be available on all Arduino platforms)
  const char* ptr = data + 3; // Skip "QR:"
  char id[32] = {0};
  int id_len = 0;
  int cx = 0, cy = 0, width = 0, height = 0;
  
  // Extract ID (until first comma)
  while (*ptr != ',' && *ptr != '\0' && *ptr != '\n' && id_len < 31) {
    id[id_len++] = *ptr++;
  }
  id[id_len] = '\0';
  
  if (*ptr != ',') return; // Invalid format
  ptr++; // Skip comma
  
  // Parse cx
  cx = 0;
  bool negative = false;
  if (*ptr == '-') {
    negative = true;
    ptr++;
  }
  while (*ptr >= '0' && *ptr <= '9') {
    cx = cx * 10 + (*ptr - '0');
    ptr++;
  }
  if (negative) cx = -cx;
  
  if (*ptr != ',') return;
  ptr++;
  
  // Parse cy
  cy = 0;
  negative = false;
  if (*ptr == '-') {
    negative = true;
    ptr++;
  }
  while (*ptr >= '0' && *ptr <= '9') {
    cy = cy * 10 + (*ptr - '0');
    ptr++;
  }
  if (negative) cy = -cy;
  
  if (*ptr != ',') return;
  ptr++;
  
  // Parse width
  width = 0;
  while (*ptr >= '0' && *ptr <= '9') {
    width = width * 10 + (*ptr - '0');
    ptr++;
  }
  
  if (*ptr != ',') return;
  ptr++;
  
  // Parse height
  height = 0;
  while (*ptr >= '0' && *ptr <= '9' && *ptr != '\0' && *ptr != '\n') {
    height = height * 10 + (*ptr - '0');
    ptr++;
  }
  
  // Validate QR code dimensions
  if (width >= QR_TARGET_WIDTH_MIN && width <= QR_TARGET_WIDTH_MAX && id_len > 0) {
    strncpy(current_qr.id, id, sizeof(current_qr.id) - 1);
    current_qr.id[sizeof(current_qr.id) - 1] = '\0';
    current_qr.center_x = cx;
    current_qr.center_y = cy;
    current_qr.width = width;
    current_qr.height = height;
    current_qr.valid = true;
    current_qr.timestamp = millis();
    last_qr_update = millis();
  }
}

bool is_correct_docking_side(const char* qr_id) {
  // Check if QR code ID matches the docking side
  // Modify this logic based on your actual QR code payloads
  if (strcmp(qr_id, QR_SIDE_FRONT) == 0) {
    return true;
  }
  // You can add more conditions here if needed
  // For now, we'll dock to FRONT side
  return false;
}

NavigationState get_navigation_state() {
  return current_state;
}

QRData get_current_qr_data() {
  return current_qr;
}

// State execution functions

void execute_search() {
  // Rotate to find QR code
  if (current_qr.valid) {
    // QR code detected, check if it's the correct side
    current_state = STATE_TARGET_FOUND;
    state_start_time = millis();
    stop();
  } else {
    // Continue rotating
    turn_left(SEARCH_ROTATION_SPEED, 1);
  }
}

void execute_target_found() {
  // Check if this is the correct docking side
  if (!current_qr.valid) {
    current_state = STATE_SEARCHING;
    state_start_time = millis();
    return;
  }
  
  if (is_correct_docking_side(current_qr.id)) {
    // Correct side found, start approaching
    current_state = STATE_APPROACHING;
    state_start_time = millis();
  } else {
    // Wrong side, continue searching
    current_state = STATE_SEARCHING;
    state_start_time = millis();
  }
}

void execute_approaching() {
  if (!current_qr.valid) {
    current_state = STATE_SEARCHING;
    state_start_time = millis();
    return;
  }
  
  // Check if close enough to start fine alignment
  if (current_qr.width >= QR_TARGET_WIDTH_DOCK - 20) {
    current_state = STATE_ALIGNING;
    state_start_time = millis();
    return;
  }
  
  // Calculate movement to approach target
  int forward_speed = 0;
  int rotation_speed = 0;
  int translate_speed = 0;
  
  calculate_movement(forward_speed, rotation_speed, translate_speed);
  
  // Apply movement
  if (abs(rotation_speed) > 1) {
    if (rotation_speed > 0) {
      turn_right(APPROACH_SPEED, 1);
    } else {
      turn_left(APPROACH_SPEED, 1);
    }
  } else if (abs(translate_speed) > 1) {
    if (translate_speed > 0) {
      translate_right(APPROACH_SPEED, 1);
    } else {
      translate_left(APPROACH_SPEED, 1);
    }
  } else {
    // Move forward
    go_forward(APPROACH_SPEED, 1);
  }
}

void execute_aligning() {
  if (!current_qr.valid) {
    current_state = STATE_SEARCHING;
    state_start_time = millis();
    return;
  }
  
  // Check if aligned and at correct distance
  int x_offset = current_qr.center_x - FRAME_CENTER_X;
  int width_diff = current_qr.width - QR_TARGET_WIDTH_DOCK;
  
  bool x_aligned = abs(x_offset) < QR_CENTER_TOLERANCE;
  bool distance_ok = abs(width_diff) < 10;
  
  if (x_aligned && distance_ok) {
    // Ready for final docking
    current_state = STATE_DOCKING;
    state_start_time = millis();
    return;
  }
  
  // Fine-tune position
  int forward_speed = 0;
  int rotation_speed = 0;
  int translate_speed = 0;
  
  calculate_movement(forward_speed, rotation_speed, translate_speed);
  
  // Apply fine movements
  if (abs(rotation_speed) > 0) {
    if (rotation_speed > 0) {
      turn_right(ALIGN_SPEED, 1);
    } else {
      turn_left(ALIGN_SPEED, 1);
    }
  } else if (abs(translate_speed) > 0) {
    if (translate_speed > 0) {
      translate_right(ALIGN_SPEED, 1);
    } else {
      translate_left(ALIGN_SPEED, 1);
    }
  } else if (abs(forward_speed) > 0) {
    if (forward_speed > 0) {
      go_forward(ALIGN_SPEED, 1);
    } else {
      go_backward(ALIGN_SPEED, 1);
    }
  } else {
    stop();
  }
}

void execute_docking() {
  if (!current_qr.valid) {
    current_state = STATE_SEARCHING;
    state_start_time = millis();
    return;
  }
  
  // Check if docked (QR code is large enough)
  if (current_qr.width >= DOCK_COMPLETE_WIDTH) {
    current_state = STATE_DOCKED;
    state_start_time = millis();
    stop();
    return;
  }
  
  // Final approach - move forward slowly
  int x_offset = current_qr.center_x - FRAME_CENTER_X;
  
  // Small corrections while moving forward
  if (abs(x_offset) > QR_CENTER_TOLERANCE) {
    if (x_offset > 0) {
      translate_right(ALIGN_SPEED, 1);
    } else {
      translate_left(ALIGN_SPEED, 1);
    }
  } else {
    go_forward(DOCK_SPEED, 1);
  }
}

void calculate_movement(int& forward_speed, int& rotation_speed, int& translate_speed) {
  if (!current_qr.valid) {
    forward_speed = 0;
    rotation_speed = 0;
    translate_speed = 0;
    return;
  }
  
  // Calculate x-axis offset (centering)
  int x_offset = current_qr.center_x - FRAME_CENTER_X;
  
  // Calculate distance error (based on QR width)
  int width_error = current_qr.width - QR_TARGET_WIDTH_DOCK;
  
  // Proportional control for translation (centering)
  translate_speed = (int)(x_offset * KP_X);
  
  // Proportional control for forward/backward (distance)
  forward_speed = (int)(width_error * KP_DISTANCE);
  
  // Limit speeds
  if (translate_speed > 9) translate_speed = 9;
  if (translate_speed < -9) translate_speed = -9;
  if (forward_speed > 9) forward_speed = 9;
  if (forward_speed < -9) forward_speed = -9;
  
  // Rotation speed (for future use, currently not used)
  rotation_speed = 0;
}

