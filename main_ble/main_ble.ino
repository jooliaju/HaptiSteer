#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <ArduinoJson.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;

// UUIDs for the BLE service and characteristic
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a3"

#define MOTOR_PIN_L_1 13  
#define MOTOR_PIN_L_2 12  
#define MOTOR_PIN_L_3 14  
#define MOTOR_PIN_L_4 27  
#define MOTOR_PIN_L_5 26  
#define MOTOR_PIN_L_6 25  

#define MOTOR_PIN_R_1 15
#define MOTOR_PIN_R_2 2  
#define MOTOR_PIN_R_3 5  
#define MOTOR_PIN_R_4 18  
#define MOTOR_PIN_R_5 19
#define MOTOR_PIN_R_6 21


const int leftPins[] = {MOTOR_PIN_L_1, MOTOR_PIN_L_2, MOTOR_PIN_L_3, MOTOR_PIN_L_4, MOTOR_PIN_L_5, MOTOR_PIN_L_6};
const int rightPins[] = {MOTOR_PIN_R_1, MOTOR_PIN_R_2, MOTOR_PIN_R_3, MOTOR_PIN_R_4, MOTOR_PIN_R_5, MOTOR_PIN_R_6};


class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Client connected");
  }

  void onDisconnect(BLEServer* pServer) {
    Serial.println("Client disconnected, restarting advertising...");
    pServer->getAdvertising()->start();  // Keep device always connectable
  }
};

void parseAndHandleJsonData(const String &message) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  const char* maneuver = doc["maneuver"];
  double distance = doc["distance"];

  Serial.print("Maneuver: ");
  Serial.println(maneuver);
  Serial.print("Distance: ");
  Serial.println(distance);

  String maneuverStr = maneuver;

  if (maneuverStr == "turn-left") {
    Serial.println("Vibrating left motors...");
    // Turn all left pins on
    for (int pin : leftPins) {
      digitalWrite(pin, HIGH);
    }
  
    delay(500);
  
    // Turn all left pins off
    for (int pin : leftPins) {
      digitalWrite(pin, LOW);
    }
  } else if (maneuverStr == "turn-right") {
    Serial.println("Vibrating right motors...");
    // Turn all left pins on
    for (int pin : rightPins) {
      digitalWrite(pin, HIGH);
    }
  
    delay(500);
  
    // Turn all left pins off
    for (int pin : rightPins) {
      digitalWrite(pin, LOW);
    }
  } else if (maneuverStr == "complete"){

    Serial.println("Buzzing both motors 3 times quickly...");

      const int buzzCount = 3;
      const int buzzDuration = 100;      // Duration to keep motors on (in ms)
      const int buzzInterval = 100;      // Interval between buzzes (in ms)

      for (int i = 0; i < buzzCount; i++) {
          // Turn both left and right pins on
          for (int pin : leftPins) {
              digitalWrite(pin, HIGH);
          }
          for (int pin : rightPins) {
              digitalWrite(pin, HIGH);
          }

          delay(buzzDuration);  // Keep motors on for buzzDuration

          // Turn both left and right pins off
          for (int pin : leftPins) {
              digitalWrite(pin, LOW);
          }
          for (int pin : rightPins) {
              digitalWrite(pin, LOW);
          }

          delay(buzzInterval);  // Wait before the next buzz
      }

      Serial.println("Completed buzzing both motors.");

  }
    else {
    Serial.println("Unknown maneuver. Doing nothing.");
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize the BLE device
  BLEDevice::init("ESP32-BLE");
  Serial.println("ESP32 initialized");

  // Create a BLE server and set callbacks
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create a BLE service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->addDescriptor(new BLE2902());
  
  // Start the service
  pService->start();

  // Start advertising so that the device is always connectable
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
  Serial.println("BLE advertising started");

  // Initialize all left and right motor pins as outputs and set them LOW
  for (int pin : leftPins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  for (int pin : rightPins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
}

void loop() {

  // Check if there's any data written to the characteristic
  if (pCharacteristic->getValue().length() > 0) {
    String receivedMessage = pCharacteristic->getValue().c_str();
    Serial.print("Message from Swift: ");
    Serial.println(receivedMessage);

    // Clear the characteristic value after reading it
    pCharacteristic->setValue("");

    parseAndHandleJsonData(receivedMessage);

  } else {
    Serial.println("Waiting for message...");
  }

  delay(2000);
}
