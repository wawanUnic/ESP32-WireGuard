// https://github.com/ciniml/WireGuard-ESP32-Arduino
// https://github.com/rjjrbatarao/ESP_WIREGUARD
// https://techoverflow.net/2021/12/31/esp32-wireguard-example-with-http-access-over-wireguard-platformio/

#include <WiFi.h>
#include "time.h"
#include <WireGuard-ESP32.h>
#include <HTTPClient.h>

char ssid[] = "...";
char password[] = "...";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * 1;
const int   daylightOffset_sec = 3600 * 0;

char private_key[] = "cMt/axrRi5KYbACD/DAhsu6AraRebzcr...";        // [Interface] PrivateKey
IPAddress local_ip(192,168,42,5);                                          // [Interface] Address
char public_key[] = "UN4yBsP5Q0Z0VihrPZz0QEwaYBfUH...";         // [Peer] PublicKey
char endpoint_address[] = "192.168.4.133";                                   // [Peer] Endpoint
int endpoint_port = 51820;                                                  // [Peer] Endpoint

static constexpr const uint32_t UPDATE_INTERVAL_MS = 5000;
static WireGuard wg;
static HTTPClient httpClient;
struct tm timeinfo;
byte tries = 40;
const byte MaxPoints = 100;
byte points = MaxPoints;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("");
  Serial.println("-------------------------------");
  delay(2000);

  Serial.println("Connecting to the AP...");
  WiFi.begin(ssid, password);
  while (--tries && WiFi.status() != WL_CONNECTED) {
    if (--points != 0) {
      Serial.print(".");
    } else {
      Serial.println(".");
      points = MaxPoints;
    }
    delay(1000);
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Non connecting to WiFi. Press RESET.");
    while (1) {}
  } else {
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  delay(2000);

  Serial.println("Adjusting system time...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time. Press RESET.");
    while (1) {}
  } else {
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  delay(2000);
  }

  Serial.println("Initializing WireGuard...");
  wg.begin(local_ip, private_key, endpoint_address, public_key, endpoint_port);
  points = MaxPoints;
  while(--tries && !wg.is_initialized()) {
    if (--points != 0) {
      Serial.print(".");
    } else {
      Serial.println(".");
      points = MaxPoints;
    }
    delay(1000);
  }
  if (!wg.is_initialized()) {
    Serial.println("Non initialized WireGuard. Press RESET.");
    while (1) {}
  } else {
  Serial.println("WireGuard initialized!");
  delay(2000);
  }
}

void loop() {
    WiFiClient client;

    if( !client.connect("192.168.4.2", 80) ) {
        Serial.println("Failed to connect...");
        delay(2000);
        return;
    }
    
    //uint64_t uptime_msec = millis();
    unsigned uptime_msec = millis();
    Serial.printf("Sending uptime %u [ms]\r\n", uptime_msec);
    String json;
    json += "{\"uptime_msec\":";
    json.concat(static_cast<unsigned long>(uptime_msec));
    json += "}";
    Serial.printf("payload: %s\r\n", json.c_str());
    
    client.write("POST / HTTP/1.1\r\n");
    client.write("Host: harvest.soracom.io\r\n");
    client.write("Connection: Keep-Alive\r\n");
    client.write("Keep-Alive: timeout=5, max=2\r\n");
    client.write("Content-Type: application/json\r\n");
    client.write("Content-Length: ");
    client.write(String(json.length(), 10).c_str());
    client.write("\r\n\r\n");
    client.write(json.c_str());

    while(client.connected()) {
        auto line = client.readStringUntil('\n');
        Serial.write(line.c_str());
        Serial.write("\n");
        if( line == "\r" ) break;
    }
    if(client.connected()) {
        uint8_t buffer[256];
        size_t bytesToRead = 0;
        while((bytesToRead = client.available()) > 0) {
            bytesToRead = bytesToRead > sizeof(buffer) ? sizeof(buffer) : bytesToRead;
            auto bytesRead = client.readBytes(buffer, bytesToRead); 
            Serial.write(buffer, bytesRead);
        }
    }
    delay(UPDATE_INTERVAL_MS);
}

