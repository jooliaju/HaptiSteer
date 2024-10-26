#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;

// UUIDs for the BLE service and characteristic
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define MOTOR_PIN_1 32  // Pin D32

void setup() {
  Serial.begin(115200);

  // Initialize the BLE device
  BLEDevice::init("ESP32-BLE");

  // Create a BLE server
  pServer = BLEDevice::createServer();

  // Create a BLE service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->addDescriptor(new BLE2902());
  
  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();


  // Initialize the motor pin as an output
  pinMode(MOTOR_PIN_1, OUTPUT);

  // Start with the motor turned off
  digitalWrite(MOTOR_PIN_1, LOW);
}

void loop() {
  // // Update characteristic with a new message every 2 seconds
  // pCharacteristic->setValue("Hello from ESP32");
  // pCharacteristic->notify();  // Notify connected clients of new data

    // Check if there's any data written to the characteristic
  if (pCharacteristic->getValue().length() > 0) {
    String receivedMessage = pCharacteristic->getValue().c_str();
    Serial.print("Message from Swift: ");
    Serial.println(receivedMessage);

    // Clear the characteristic value after reading it
    pCharacteristic->setValue("");

    // run motor when mesage is received
    digitalWrite(MOTOR_PIN_1, HIGH);
    delay(2000);
    digitalWrite(MOTOR_PIN_1, LOW);

  } else {
    Serial.println("waiting for message");
  }
  
  delay(2000);
}
