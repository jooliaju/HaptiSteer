#define MOTOR_PIN_1 32  // Pin D32
#define MOTOR_PIN_2 34  // Pin D32

// a flag to control motor on and off
bool motorRun = true;

void setup() {
  // Initialize the motor pin as an output
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);

  // Start with the motor turned off
  digitalWrite(MOTOR_PIN_1, LOW);
  digitalWrite(MOTOR_PIN_2, LOW);
}

void loop() {

  if (motorRun) {
    digitalWrite(MOTOR_PIN_1, HIGH);
    digitalWrite(MOTOR_PIN_2, LOW);
    delay(2000);
    digitalWrite(MOTOR_PIN_1, LOW);
    digitalWrite(MOTOR_PIN_2, HIGH);
    delay(2000);
    motorRun = false;  // Set the flag to stop the loop
  }
}