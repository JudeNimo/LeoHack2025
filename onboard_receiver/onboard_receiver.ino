#include <SPI.h>
#include <WiFiNINA.h>
#include <Servo.h>
#include "motor_control.h"
#include "student_functions.h"
#include "navigation.h"
#include "config.h"

char ssid[] = "Nano_OrbitalCleaners_AP";
char pass[] = "orbital";

int status = WL_IDLE_STATUS;
WiFiServer server(8080);  // TCP server on port 8080

// Autonomous navigation mode
bool autonomous_mode = false;  // Set to true to enable autonomous docking
String serial_buffer = "";    // Buffer for reading Serial1 data

// Forward declaration
void read_serial1_data();

void setup() { 
  Serial.begin(9600); //initialising serial connection for debugging
  Serial1.begin(SERIAL1_BAUD); // Hardware Serial for ESP32-CAM communication

  Serial.println("Creating an access point...");

  status = WiFi.beginAP(ssid, pass); //setting up the AP
  if (status != WL_AP_LISTENING){
    Serial.println("Failed to start AP");
    //while(true); //stop if failed
  }

  delay(5000); //allow the AP to initialise

  IPAddress ip = WiFi.localIP();
  Serial.print("AP IP Address: ");
  Serial.println(ip);

  server.begin();
  servo_init();

  motor_init();
  navigation_init();
  
  Serial.println("System initialized. Press 'm' to toggle autonomous mode.");
  Serial.println("Autonomous mode: OFF");
}

void loop() {
  // Read data from ESP32-CAM via Serial1
  read_serial1_data();
  
  // Update navigation if in autonomous mode
  if (autonomous_mode) {
    navigation_update();
  }
  
  WiFiClient client = server.available(); // Listen for incoming clients

  if (client) {
    Serial.println("New client connected.");
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);         // Print to serial monitor

        // Toggle autonomous mode
        if (c == 'm') {
          autonomous_mode = !autonomous_mode;
          if (autonomous_mode) {
            Serial.println("Autonomous mode: ON");
            navigation_init();
          } else {
            Serial.println("Autonomous mode: OFF");
            stop();
          }
          client.write(c); // Echo back
          continue;
        }
        
        // If in autonomous mode, ignore manual commands (except 'm' and 's')
        if (autonomous_mode && c != 's') {
          client.write(c); // Echo back
          continue;
        }

        //checking if one of standard commands
        if (c == 's') {
          //STOP
          // call the stop function here
          stop();
          Serial.println("Motors Stopped");
        }
        if (c == 'f') {
          //MOVE FORWARD
          //expecting a format of '[power (0-9)] [time (0-9)]'
          char space1 = client.read(); //empty space read
          char power = client.read();
          char space2 = client.read(); //empty space read
          char time = client.read();

          String outputmessage = "Going forward at power and time: ";
          outputmessage += power;
          outputmessage += ' ';
          outputmessage += time;
          Serial.println(outputmessage);

          //converting the chars to ints
          int itime = time - '0';
          int ipower = power - '0';

          go_forward(ipower, itime);
        }

        if (c == 'b') {
          //MOVE BACKWARDS
          //expecting a format of '[power (0-9)] [time (0-9)]'
          char space1 = client.read(); //empty space read
          char power = client.read();
          char space2 = client.read(); //empty space read
          char time = client.read();

          String outputmessage = "Going backwards at power and time: ";
          outputmessage += power;
          outputmessage += ' ';
          outputmessage += time;
          Serial.println(outputmessage);

          //converting the chars to ints
          int itime = time - '0';
          int ipower = power - '0';

          go_backward(ipower, itime);
        }

        if (c == 'l') {
          //ROTATE LEFT
          //expecting a format of '[power (0-9)] [time (0-9)]'
          char space1 = client.read(); //empty space read
          char power = client.read();
          char space2 = client.read(); //empty space read
          char time = client.read();

          String outputmessage = "Rotating left at power and time: ";
          outputmessage += power;
          outputmessage += ' ';
          outputmessage += time;
          Serial.println(outputmessage);

          //converting the chars to ints
          int itime = time - '0';
          int ipower = power - '0';

          turn_left(ipower, itime);
        }

        if (c == 'r') {
          //ROTATE RIGHT
          //expecting a format of '[power (0-9)] [time (0-9)]'
          char space1 = client.read(); //empty space read
          char power = client.read();
          char space2 = client.read(); //empty space read
          char time = client.read();

          String outputmessage = "Rotating right at power and time: ";
          outputmessage += power;
          outputmessage += ' ';
          outputmessage += time;
          Serial.println(outputmessage);

          //converting the chars to ints
          int itime = time - '0';
          int ipower = power - '0';

          turn_right(ipower, itime);
        }

        if (c == 'a') {
          //TRANSLATE LEFT
          //expecting a format of '[power (0-9)] [time (0-9)]'
          char space1 = client.read(); //empty space read
          char power = client.read();
          char space2 = client.read(); //empty space read
          char time = client.read();

          String outputmessage = "Going left at power and time: ";
          outputmessage += power;
          outputmessage += ' ';
          outputmessage += time;
          Serial.println(outputmessage);

          //converting the chars to ints
          int itime = time - '0';
          int ipower = power - '0';

          translate_left(ipower, itime);
        }

        if (c == 'd') {
          //TRANSLATE RIGHT
          //expecting a format of '[power (0-9)] [time (0-9)]'
          char space1 = client.read(); //empty space read
          char power = client.read();
          char space2 = client.read(); //empty space read
          char time = client.read();

          String outputmessage = "Going right at power and time: ";
          outputmessage += power;
          outputmessage += ' ';
          outputmessage += time;
          Serial.println(outputmessage);

          //converting the chars to ints
          int itime = time - '0';
          int ipower = power - '0';

          translate_right(ipower, itime);
        }

        // === SAMPLE SERVO CONTROL ===
        if (c == 'o') {
          servo_open();
          Serial.println("O function triggered");
        }
        if (c == 'p') {
          servo_close();
          Serial.println("P function triggered");
        }

        client.write(c);         // Echo back to client
      }
    }
    client.stop();
    Serial.println("Client disconnected.");
  }
}

// Function to read and process Serial1 data from ESP32-CAM
void read_serial1_data() {
  while (Serial1.available()) {
    char c = Serial1.read();
    
    if (c == '\n') {
      // End of line, process the buffer
      if (serial_buffer.length() > 0) {
        process_qr_data(serial_buffer.c_str());
        serial_buffer = "";
      }
    } else if (c >= 32 && c <= 126) {
      // Printable character, add to buffer
      serial_buffer += c;
      
      // Prevent buffer overflow
      if (serial_buffer.length() > 128) {
        serial_buffer = "";
      }
    }
  }
}