///////////////////Includes//////////////////

#include <Arduino.h>

#include <sstream>
#include <string>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "ESP32_BLE.h"

// https://www.uuidgenerator.net/

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR1_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHAR2_UUID "e3223119-9445-4e96-a4a1-85358c4046a2"
#define CHAR3_UUID "f33ed397-47ca-426a-9fd7-17cfbf5a90b5"
#define CHAR4_UUID "ee5c91a8-2e8e-477e-88c9-d28e7806e46c"
#define CHAR5_UUID "bf02b4c6-b886-4a13-beff-aaa9d5e119e2"
#define CHAR6_UUID "90abe9e7-91f2-4782-9ba9-b248804cb290"
#define CHAR7_UUID "f9d0f50d-f558-4472-9151-e8f053ae2f38"
#define CHAR8_UUID "f08aa913-8e8f-494a-ac3c-c586b6b719f0"
#define CHAR9_UUID "79b5e4af-5d56-4311-8e16-96708c8649e1"
#define CHAR10_UUID "71d5574a-b20c-4904-a0f2-135d2d39504f"
#define CHAR11_UUID "700f02fa-371f-4e3d-b034-c9c0cf5c8684"

BLEServer* pServer = NULL;

BLECharacteristic* pCharacteristic = NULL;
BLECharacteristic* pCharacteristic_2 = NULL;
BLECharacteristic* pCharacteristic_3 = NULL;
BLECharacteristic* pCharacteristic_4 = NULL;
BLECharacteristic* pCharacteristic_5 = NULL;
BLECharacteristic* pCharacteristic_6 = NULL;
BLECharacteristic* pCharacteristic_7 = NULL;
BLECharacteristic* pCharacteristic_8 = NULL;
BLECharacteristic* pCharacteristic_9 = NULL;
BLECharacteristic* pCharacteristic_10 = NULL;
BLECharacteristic* pCharacteristic_11 = NULL;

BLE2902* pBLE2902;

/*
  CHAR Value:

  -CHAR1 - WhiteLeftOut
  -CHAR2 - WhiteLeftInner
  -CHAR3 - WhiteCenter
  -CHAR4 - WhiteRightInner
  -CHAR5 - WhiteRightOut
  -CHAR6 - ReflectLeft
  -CHAR7 - ReflectRight
  -CHAR8 - RedLeftInner
  -CHAR9 - RedRightInner
  -CHAR10 - GreenLeftInner
  -CHAR11 - GreenRightInner
*/

/////////////////////////////////////////////

//////////////////Variables//////////////////

unsigned int BLEStat = 0;  //0 - OFF, 1 - ON, 2 - connected

bool deviceConnected = false;
bool oldDeviceConnected = false;
/////////////////////////////////////////////

////////////////////Code/////////////////////

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    BLEStat = 2;
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    BLEStat = 1;
  }
};

void BLELoop(int wlo, int wli, int wc, int wri, int wro, int rl, int rr, int redl, int redr, int gl, int gr) {

  if (BLEStat >= 1) {

    // notify changed value
    if (deviceConnected) {

      pCharacteristic->setValue(wlo);
      pCharacteristic->notify();

      pCharacteristic_2->setValue(wli);
      pCharacteristic_2->notify();

      pCharacteristic_3->setValue(wc);
      pCharacteristic_3->notify();

      pCharacteristic_4->setValue(wri);
      pCharacteristic_4->notify();

      pCharacteristic_5->setValue(wro);
      pCharacteristic_5->notify();

      pCharacteristic_6->setValue(rl);
      pCharacteristic_6->notify();

      pCharacteristic_7->setValue(rr);
      pCharacteristic_7->notify();

      pCharacteristic_8->setValue(redl);
      pCharacteristic_8->notify();

      pCharacteristic_9->setValue(redr);
      pCharacteristic_9->notify();

      pCharacteristic_10->setValue(gl);
      pCharacteristic_10->notify();

      pCharacteristic_11->setValue(gr);
      pCharacteristic_11->notify();
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
      delay(500);                   // give the bluetooth stack the chance to get things ready
      pServer->startAdvertising();  // restart advertising
      Serial.println("BLE - Start advertising");
      oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
      // do stuff here on connecting
      oldDeviceConnected = deviceConnected;
    }
  }
}

void StartBLE() {

  BLEDevice::init("Oktagon III");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService* pService = pServer->createService(BLEUUID(SERVICE_UUID), 30, 0);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
    CHAR1_UUID,
    BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristic_2 = pService->createCharacteristic(
    CHAR2_UUID,
    BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristic_3 = pService->createCharacteristic(
    CHAR3_UUID,
    BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristic_4 = pService->createCharacteristic(
    CHAR4_UUID,
    BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristic_5 = pService->createCharacteristic(
    CHAR5_UUID,
    BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristic_6 = pService->createCharacteristic(
    CHAR6_UUID,
    BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristic_7 = pService->createCharacteristic(
    CHAR7_UUID,
    BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristic_8 = pService->createCharacteristic(
    CHAR8_UUID,
    BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristic_9 = pService->createCharacteristic(
    CHAR9_UUID,
    BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristic_10 = pService->createCharacteristic(
    CHAR10_UUID,
    BLECharacteristic::PROPERTY_NOTIFY);
  
  pCharacteristic_11 = pService->createCharacteristic(
    CHAR11_UUID,
    BLECharacteristic::PROPERTY_NOTIFY);

  pBLE2902 = new BLE2902();
  pBLE2902->setNotifications(true);

  // Add all Descriptors here
  pCharacteristic->addDescriptor(pBLE2902);

  pCharacteristic_2->addDescriptor(pBLE2902);

  pCharacteristic_3->addDescriptor(pBLE2902);

  pCharacteristic_4->addDescriptor(pBLE2902);

  pCharacteristic_5->addDescriptor(pBLE2902);

  pCharacteristic_6->addDescriptor(pBLE2902);

  pCharacteristic_7->addDescriptor(pBLE2902);

  pCharacteristic_8->addDescriptor(pBLE2902);

  pCharacteristic_9->addDescriptor(pBLE2902);

  pCharacteristic_10->addDescriptor(pBLE2902);

  pCharacteristic_11->addDescriptor(pBLE2902);

  // Start the service
  pService->start();

  BLEStat = 1;

  // Start advertising
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("BLE - Waiting a client connection to notify...");
}

void StopBLE() {
  BLEStat = 0;
  BLEDevice::stopAdvertising();
  BLEDevice::deinit();
  BLEStat = 0;
}

/////////////////////////////////////////////
