#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

const char* ssid = "mndsystem_2.4G";  // Wi-Fi 이름 (확인완료)
const char* password = "01099574832"; // Wi-Fi 비밀번호 (확인완료)

// 여기서 본인의 GitHub 사용자이름, 리포지토리 이름을 입력해주세요.
const char* firmwareUrl = "https://github.com/Jollibeegit/testest/releases/latest/download/firmware.bin";

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");

  otaUpdate();  // 부팅 시 한번 실행
}

void loop() {
  // 주기적 업데이트 확인이 필요하면 여기에 타이머를 추가하면 좋아요!
}

void otaUpdate() {
  Serial.println("Checking Firmware Update...");

  HTTPClient http;
  http.begin(firmwareUrl);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    int len = http.getSize();
    bool canUpdate = Update.begin(len);

    if (canUpdate) {
      size_t written = Update.writeStream(http.getStream());

      if (written == len) {
        Serial.println("OTA written successfully");
      } else {
        Serial.printf("OTA written partially (%d/%d)\n", written, len);
      }

      if (Update.end()) {
        if (Update.isFinished()) {
          Serial.println("Update successful. Rebooting.");
          ESP.restart();
        } else {
          Serial.println("Update not finished");
        }
      } else {
        Serial.printf("Update failed. Error: %d\n", Update.getError());
      }
    } else {
      Serial.println
