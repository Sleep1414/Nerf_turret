#include <Arduino.h>

#include "Turret.h"
#include <ServoEasing.hpp>
#include <Esp32Servo.h>

#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "BLE2902.h"

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

#define SERVICE_UUID "8ebaf998-8c59-46da-91de-3a3dae4c224f"
#define CHARACTERISTIC_UUID_RX "bc4b69c4-4a1d-4d20-b829-ad92cd2888b6"
#define CHARACTERISTIC_UUID_TX "98ba9b1f-5341-4264-b895-7ff33fe15fcc"

#define INTERRUPT_PIN 12 
Servo recoil;
ServoEasing pan;
ServoEasing tilt;

Turret myturret(recoil, pan, tilt);

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };
 
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};
 
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      int length = rxValue.size();
      byte const* c = reinterpret_cast<byte const*>(rxValue.c_str());

      if(length > 0){
        switch (c[0])
        {
        case 0:
            myturret.fire();
            break;
        case 1: 
        myturret.motorSpeed(true);
            break;    
        case 2:
        myturret.motorSpeed(false);
            break;
        case 3: 
        if (length>3){
        myturret.motorSpeed(c[3]);
        }
            break;    
        default:
            break;
        }
      }
      if (length> 2)
      {
       myturret.targets(c[1], c[2]);
      }
      
        
        
        
    
 
      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.println("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
          Serial.println(c[i]);
 
        Serial.println();
        Serial.println("*********");
      }

    }
};
void IRAM_ATTR ISR(){
  myturret.fire();
}

void setup()
{

    Serial.begin(115200);

    

 
  // Create the BLE Device
  BLEDevice::init("UART Service For ESP32");
 
  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
 
  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);
 
  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID_TX,
                                        BLECharacteristic::PROPERTY_NOTIFY
                                    );
                      
  pTxCharacteristic->addDescriptor(new BLE2902());
 
  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
                                             CHARACTERISTIC_UUID_RX,
                                            BLECharacteristic::PROPERTY_WRITE
                                        );
 
  pRxCharacteristic->setCallbacks(new MyCallbacks());
 
  // Start the service
  pService->start();
 
  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");

  myturret.motorSpeed(2, false);
  
  pinMode(INTERRUPT_PIN,INPUT_PULLUP);
  attachInterrupt(INTERRUPT_PIN,ISR,HIGH);
  
}

void loop()

{
       if (deviceConnected) {
        pTxCharacteristic->setValue(&txValue, 1);
        pTxCharacteristic->notify();
        txValue++;
        delay(10); // bluetooth stack will go into congestion, if too many packets are sent
    }
 
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
