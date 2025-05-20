// === BLE Client ===

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEClient.h>

BLEAdvertisedDevice* myDevice = nullptr;
bool doConnect = false;

int txPower = -59;  // 기준 Tx power (dBm)
float n = 2.0;      // 환경 감쇠 계수

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == "BLE_Distance_Server") {
      Serial.println("📡 BLE_Distance_Server 발견!");

      BLEDevice::getScan()->stop();  // 스캔 중단
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
    }
  }
};

void connectToServer() {
  BLEClient* pClient = BLEDevice::createClient();
  Serial.println("🔗 서버에 연결 시도 중...");

  if (pClient->connect(myDevice)) {
    Serial.println("✅ 서버에 성공적으로 연결됨!");

    // RSSI 받아오기
    int rssi = pClient->getRssi();
    float distance = pow(10.0, ((txPower - rssi) / (10.0 * n)));

    Serial.println("----- 연결된 BLE 장치 정보 -----");
    Serial.print("🔋 RSSI: "); Serial.println(rssi);
    Serial.print("📏 거리 추정: "); Serial.print(distance); Serial.println(" m");
    Serial.println("--------------------------------");


  } else {
    Serial.println("❌ 연결 실패!");
  }

  delete myDevice;
  myDevice = nullptr;
}

void setup() {
  Serial.begin(115200);
  Serial.println("🔍 BLE 스캐너 시작...");

  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);  // 5초간 스캔
}

void loop() {
  if (doConnect && myDevice != nullptr) {
    connectToServer();
    doConnect = false;

    // 일정 주기 후 재스캔을 원할 경우
    delay(5000);
    BLEDevice::getScan()->start(5, false);
  }

  delay(1000);
}
