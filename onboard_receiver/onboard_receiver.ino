#include <SPI.h>
#include <WiFiNINA.h>
#include "motor_control.h"

char ssid[] = "Nano_[YOUR_NAME]_AP";
char pass[] = "nano1pwd";

int status = WL_IDLE_STATUS;
WiFiServer server(8080);  // TCP server on port 8080


void setup() { 
  Serial.begin(9600); //initialising serial connection
  //while (!Serial);

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

  motor_init();
}

void loop() {
  WiFiClient client = server.available(); // Listen for incoming clients

  if (client) {
    Serial.println("New client connected.");
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);         // Print to serial monitor

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

        client.write(c);         // Echo back to client
      }
    }
    client.stop();
    Serial.println("Client disconnected.");
  }
}