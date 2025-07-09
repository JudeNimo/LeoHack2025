// Motor 1 connections
int Mot1_pwm = 2;
int Mot1_hbridge_1 = 4;
int Mot1_hbridge_2 = 5;
// Motor 2 connections
int Mot2_pwm = 3;
int Mot2_hbridge_1 = 6;
int Mot2_hbridge_2 = 7;

void setup() {
	// Set all the motor control pins to outputs
	pinMode(Mot1_pwm, OUTPUT);
	pinMode(Mot2_pwm, OUTPUT);
	pinMode(Mot1_hbridge_1, OUTPUT);
	pinMode(Mot1_hbridge_2, OUTPUT);
	pinMode(Mot2_hbridge_1, OUTPUT);
	pinMode(Mot2_hbridge_2, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(Mot1_hbridge_1, LOW);
	digitalWrite(Mot1_hbridge_2, LOW);
	digitalWrite(Mot2_hbridge_1, LOW);
	digitalWrite(Mot2_hbridge_2, LOW);
}

void loop() {
	directionControl();
	delay(1000);
	speedControl();
	delay(1000);
}

// This function lets you control spinning direction of motors
void directionControl() {
	// Set motors to maximum speed
	// For PWM maximum possible values are 0 to 255
	analogWrite(Mot1_pwm, 100);
	analogWrite(Mot2_pwm, 100);

	// Turn on motor 1 & 2
	digitalWrite(Mot1_hbridge_1, HIGH);
	digitalWrite(Mot1_hbridge_2, LOW);
	digitalWrite(Mot2_hbridge_1, HIGH);
	digitalWrite(Mot2_hbridge_2, LOW);
	delay(2000);
	
	// Now change motor directions
	digitalWrite(Mot1_hbridge_1, LOW);
	digitalWrite(Mot1_hbridge_2, HIGH);
	digitalWrite(Mot2_hbridge_1, LOW);
	digitalWrite(Mot2_hbridge_2, HIGH);
	delay(2000);
	
	// Turn off motors
	digitalWrite(Mot1_hbridge_1, LOW);
	digitalWrite(Mot1_hbridge_2, LOW);
	digitalWrite(Mot2_hbridge_1, LOW);
	digitalWrite(Mot2_hbridge_2, LOW);
}

// This function lets you control speed of the motors
void speedControl() {
	// Turn on motors
	digitalWrite(Mot1_hbridge_1, LOW);
	digitalWrite(Mot1_hbridge_2, HIGH);
	digitalWrite(Mot2_hbridge_1, LOW);
	digitalWrite(Mot2_hbridge_2, HIGH);
	
	// Accelerate from zero to maximum speed
	for (int i = 0; i < 256; i++) {
		analogWrite(Mot1_pwm, i);
		analogWrite(Mot2_pwm, i);
		delay(20);
	}
	
	// Decelerate from maximum speed to zero
	for (int i = 255; i >= 0; --i) {
		analogWrite(Mot1_pwm, i);
		analogWrite(Mot2_pwm, i);
		delay(20);
	}
	
	// Now turn off motors
	digitalWrite(Mot1_hbridge_1, LOW);
	digitalWrite(Mot1_hbridge_2, LOW);
	digitalWrite(Mot2_hbridge_1, LOW);
	digitalWrite(Mot2_hbridge_2, LOW);
}
