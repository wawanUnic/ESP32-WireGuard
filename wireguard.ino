// https://github.com/ciniml/WireGuard-ESP32-Arduino
// https://github.com/rjjrbatarao/ESP_WIREGUARD

#include <WiFi.h>
#include "time.h"
#include <WireGuard-ESP32.h>
#include <ESPping.h>

char ssid[] = "...............";
char password[] = "..............";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * 1;
const int   daylightOffset_sec = 3600 * 0;

char private_key[] = "cMt/axrRi5KYbACD/DAhsu6....................";        // [Interface] PrivateKey
IPAddress local_ip(192,168,42,5);                                          // [Interface] Address
char public_key[] = "UN4yBsP5Q0Z0VihrPZz0QEwaYB.................";         // [Peer] PublicKey
char endpoint_address[] = "192.168.4.133";                                   // [Peer] Endpoint
int endpoint_port = 51820;                                                  // [Peer] Endpoint

static constexpr const uint32_t UPDATE_INTERVAL_MS = 5000;
static WireGuard wg;
//static HTTPClient httpClient;
byte tries = 40;
byte points = 80;
int success, wrong = 0;
IPAddress targetIP(192,168,42,1);  // Destination IP address for PING
struct tm timeinfo;

void printLocalTime() {
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time. Press RESET.");
    while (1) {}
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("");
  Serial.println("-------------------------------");
  Serial.println("Connecting to the AP...");
  WiFi.begin(ssid, password);

  while (--tries && WiFi.status() != WL_CONNECTED) {
    if (--points != 0) {
      Serial.print(".");
    } else {
      Serial.println(".");
      points = 80;
    }
    delay(1000);
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Non Connecting to WiFi. Press RESET.");
    while (1) {}
  } else {
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  delay(2000);

  // Init and get the time
  Serial.println("Adjusting system time..."); // We configure the NTP server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  delay(2000);

  Serial.println("Initializing WireGuard...");
  wg.begin(local_ip, private_key, endpoint_address, public_key, endpoint_port);
  points = 80;
  while( !wg.is_initialized()) {
    if (--points != 0) {
      Serial.print(".");
    } else {
      Serial.println(".");
      points = 80;
    }
    delay(1000);
  }
  Serial.println("WireGuard initialized!");
  delay(2000);
}

void loop()
{
  getLocalTime(&timeinfo);

  Serial.print("Success/Wrong pings: ");
  Serial.print(success);
  Serial.print("/");
  Serial.print(wrong);
  Serial.print(". Time begin ping: ");
  Serial.println(&timeinfo, "%H:%M:%S");
  bool pingResult = Ping.ping(targetIP);

  if (pingResult) {
    success += 1;
    Serial.print("Response from ");
    Serial.print(targetIP);
    Serial.print(" in ");
    Serial.print(Ping.averageTime());
    Serial.println(" ms");
  } else {
    wrong += 1;
    Serial.println("Ping failed");
  }
}
