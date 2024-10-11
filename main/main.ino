#define MOTOR_PIN 32  // Pin D32 

// a flag to control motor on and off
bool motorRun = true;

void setup() {
  // Initialize the motor pin as an output
  pinMode(MOTOR_PIN, OUTPUT);
  
  // Start with the motor turned off
  digitalWrite(MOTOR_PIN, LOW);
}

void loop() {

  if (motorRun) {
    digitalWrite(MOTOR_PIN, HIGH); 
    delay(2000); 
    digitalWrite(MOTOR_PIN, LOW);
    delay(2000);  

    motorRun = false;  // Set the flag to stop the loop
  }
}