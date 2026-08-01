#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <WireGuard-ESP32.h>

#include "config.h"   // WIFI_SSID, WIFI_PASSWORD, WG_*, OTA_* ada di sini

WireGuard wg;
IPAddress wg_local_ip(WG_LOCAL_IP);

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // --- Konek WiFi ---
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi tersambung, IP: ");
  Serial.println(WiFi.localIP());

  // --- Sinkronisasi waktu (dibutuhkan WireGuard untuk handshake) ---
  configTime(7 * 3600, 0, "pool.ntp.org", "time.google.com");
  time_t now = time(nullptr);
  while (now < 100000) {
    delay(500);
    now = time(nullptr);
  }

  // --- Konek WireGuard ---
  bool wgOk = wg.begin(wg_local_ip, WG_PRIVATE_KEY, WG_ENDPOINT_ADDRESS,
                        WG_ENDPOINT_PUBLIC_KEY, WG_ENDPOINT_PORT);
  Serial.println(wgOk ? "WireGuard tersambung" : "Gagal konek WireGuard!");

  // --- Routes ---
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "ready");
  });

  // --- OTA ---
  ElegantOTA.begin(&server, OTA_USERNAME, OTA_PASSWORD);
  server.begin();
}

void loop() {
  ElegantOTA.loop();
}