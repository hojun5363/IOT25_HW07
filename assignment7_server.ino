// === BLE Server ===

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// 연결 상태 확인용 콜백 클래스
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("BLE client connected.");
  }

  void onDisconnect(BLEServer* pServer) {
    Serial.println("BLE client disconnected.");
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Server...");

  BLEDevice::init("BLE_Distance_Server");  // 서버 이름

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());  // 연결 콜백 등록

  BLEService *pService = pServer->createService("12345678-1234-5678-1234-56789abcdef0");

  // 더미 특성 (Read 가능)
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
    "abcd",
    BLECharacteristic::PROPERTY_READ
  );
  pCharacteristic->setValue("Hello BLE Client");
  pService->start();

  // 광고 시작
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID("12345678-1234-5678-1234-56789abcdef0");
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("BLE Advertising started...");
}

void loop() {
  delay(1000);
}

