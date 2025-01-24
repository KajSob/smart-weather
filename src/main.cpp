#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#include "ScioSense_ENS160.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "NimBLEDevice.h"

const char* ca_cert = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

ScioSense_ENS160 ens160(0x53);
Adafruit_BME280 bme;
TFT_eSPI tft = TFT_eSPI();

const char* mqtt_server = "p321361f.ala.dedicated.gcp.emqxcloud.com";
const int mqtt_port = 8883;
const char* mqtt_user = "admin"; 
const char* mqtt_password = "cdV8Sa5eMRGuV3m";   

const char* ssid = "SkipperWiFi";
const char* password = "kupsewlasny";
WiFiClientSecure espClient;
PubSubClient client(espClient);

const char* MQTT_ID = "4tmYZsl99D";

#define LED1_PIN 35
#define LED2_PIN 36
#define LED3_PIN 37
#define LED4_PIN 38
#define LED5_PIN 39

const int photoResistorPin = 15;
int sensorValue = 0;
int delayTime = 2000;
const char* delay_topic = "weather-station/config/interval";

unsigned long previousMillis = 0;

static BLEUUID serviceUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6");
static BLEUUID charUUID("ebe0ccc1-7a0a-4b0c-8a1a-6ff2997da3a6");

BLEClient *pClient;

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean BLEconnected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
std::string mac_address = "a4:c1:38:fe:a2:c5";

float temp[4];
float humi[4];
float voltage[4];
 
bool ch=0;
int vol=0;
int volE; 
unsigned long tt=0;
unsigned long tt2=0;
int period=1000;
int s=0;

float temp_ble = 0;
float humi_ble = 0;
float voltage_ble = 0;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  Serial.print("Dane z sensora:");
  temp[0] = (pData[0] | (pData[1] << 8)) * 0.01;
  humi[0] = pData[2];
  voltage[0] = (pData[3] | (pData[4] << 8)) * 0.001;
  temp_ble = temp[0];
  humi_ble = humi[0];
  voltage_ble = voltage[0];
  Serial.printf("temp = %.1f C ; humidity = %.1f %% ; voltage = %.3f V\n", temp[0], humi[0], voltage[0]);
  Serial.println();

}

bool connectToServer(BLEAddress pAddress) {
  Serial.print("Forming a connection to ");
  Serial.println(pAddress.toString().c_str());

  pClient  = BLEDevice::createClient();
  Serial.println(" - Created client");

  pClient->connect(pAddress);
  Serial.println(" - Connected to server");

  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    return false;
  }
  Serial.println(" - Found our service");

  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    return false;
  }
  Serial.println(" - Found our characteristic");

  std::string value = pRemoteCharacteristic->readValue();
  Serial.print("The characteristic value was: ");
  Serial.println(value.c_str());

  if(pClient->getPeerAddress().toString().c_str()==mac_address)
  pRemoteCharacteristic->subscribe(true, notifyCallback);
  BLEconnected = true;//added
  return true;//added
}

void turnOffAllLEDs()
{
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  digitalWrite(LED4_PIN, LOW);
  digitalWrite(LED5_PIN, LOW);
}

void controlLEDs(int aqi)
{
  // Wyłącz wszystkie diody na początku
  turnOffAllLEDs();

  // Logika sterowania LED
  if (aqi > 0)
    digitalWrite(LED1_PIN, HIGH); // TVOC > 0
  if (aqi > 1)
    digitalWrite(LED2_PIN, HIGH); // TVOC > 100
  if (aqi > 2)
    digitalWrite(LED3_PIN, HIGH); // TVOC > 200
  if (aqi > 3)
    digitalWrite(LED4_PIN, HIGH); // TVOC > 300
  if (aqi > 4)
    digitalWrite(LED5_PIN, HIGH); // TVOC > 400
}
void setup_wifi() {
    delay(10);
    Serial.println("Łączenie z WiFi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Połączono z WiFi!");
    Serial.print("Adres IP: ");
    Serial.println(WiFi.localIP());
}
void reconnect() {
    while (!client.connected()) {
        Serial.println("Łączenie z brokerem MQTT...");
        if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
            Serial.println("Połączono!");
            client.subscribe("weather-station/config/interval");

        } else {
            Serial.print("Nieudane połączenie. Kod błędu: ");
            Serial.print(client.state());
            Serial.println(" Próba ponownie za 5 sekund...");
            delay(5000);
        }
    }
}
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Odebrano wiadomość z tematu: ");
    Serial.println(topic);

    
    if (strcmp(topic, delay_topic) == 0) {
        String delayStr = "";
        for (int i = 0; i < length; i++) {
            delayStr += (char)payload[i];
        }
        
        unsigned long newDelay = delayStr.toInt();
        if (newDelay > 0) {
            delayTime = newDelay;
            Serial.print("Nowe opóźnienie: ");
            Serial.println(delayTime);
        }
    }
}

void setup() {
  Wire.begin(8,9);
  Serial.begin(115200);
  bme.begin(0x76);
  tft.init();
  tft.setRotation(1);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  Serial.print("ENS160...");
    bool ok = ens160.begin();
    Serial.println(ens160.available() ? "done." : "failed!");
  if (ens160.available())
  {
    Serial.print("\tRev: ");
    Serial.print(ens160.getMajorRev());
    Serial.print(".");
    Serial.print(ens160.getMinorRev());
    Serial.print(".");
    Serial.println(ens160.getBuild());

    Serial.print("\tCustom mode ");
    ens160.initCustomMode(3); 
    Serial.println(ens160.setMode(ENS160_OPMODE_STD) ? "done." : "failed!");
  }
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(LED5_PIN, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  espClient.setCACert(ca_cert);
  
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("tt");
  connectToServer(BLEAddress(mac_address));

  client.setCallback(callback);
}


void loop() {

  unsigned long currentMillis = millis();

  if (!client.connected()) {
        reconnect();
  }
  client.loop();

  if (currentMillis - previousMillis >= delayTime) {
    previousMillis = currentMillis;

  float temp = bme.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" *C");
  
  float press = bme.readPressure() / 100.0F;
  Serial.print("Pressure = ");
  Serial.print(press);
  Serial.println(" hPa");
  
  float hum = bme.readHumidity();
  Serial.print("Humidity = ");
  Serial.print(hum);
  Serial.println(" %");

  ens160.measure(true);
  Serial.print("AQI: ");
  float aqi = ens160.getAQI();
  Serial.print(aqi);
  Serial.println(" units\t");
  controlLEDs(ens160.getAQI());
  
  Serial.print("CO2: ");
  float co2 = ens160.geteCO2();
  Serial.print(co2);
  Serial.println(" ppm");

  Serial.print("TVOC: ");
  float tvoc = ens160.getTVOC();
  Serial.print(tvoc);
  Serial.println(" ppb");

  int rawValue = analogRead(photoResistorPin);
  float voltage = (4095-rawValue) * (3.3/4095);     
  float amps = voltage / 10000.0;
  float microamps = amps * 1000000;
  float lux = microamps * 2.0; 
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lux");

  Serial.println();

  if (client.connected()) {
    client.publish("weather-station/sensors/temperature", String(temp).c_str());
    client.publish("weather-station/sensors/humidity", String(hum).c_str());
    client.publish("weather-station/sensors/pressure", String(press).c_str());
    client.publish("weather-station/sensors/light", String(lux).c_str());
    client.publish("weather-station/sensors/tvoc", String(tvoc).c_str()); 
    client.publish("weather-station/sensors/aqui", String(aqi).c_str());
    client.publish("weather-station/sensors/c02", String(co2).c_str());  
  } else {
    Serial.println("MQTT nie jest połączony!");
  }

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 10); 
  tft.print("Temperature: ");
  tft.print(temp);
  tft.println(" *C");

  tft.setCursor(10, 40);
  tft.print("Pressure: ");
  tft.print(press);
  tft.println(" hPa");

  tft.setCursor(10, 70);
  tft.print("Humidity: ");
  tft.print(hum);
  tft.println(" %");

  tft.setCursor(10, 100);
  tft.print("AQI: ");
  tft.print(aqi);
  tft.println(" units");

  tft.setCursor(10, 130);
  tft.print("CO2: ");
  tft.print(co2);
  tft.println(" ppm");

  tft.setCursor(10, 160);
  tft.print("TVOC: ");
  tft.print(tvoc);
  tft.println(" ppb");

  tft.setCursor(10, 190);
  tft.print("Light: ");
  tft.print(lux);
  tft.println(" lux");

  if (BLEconnected) {
    tft.setCursor(10, 220);
    tft.print("BLE Device tmp: ");
    tft.print(temp_ble);
    tft.print(" *C");
  }
  else {
    tft.setCursor(10, 220);
    tft.println("BLE Device: not connected");
  }

}
}
