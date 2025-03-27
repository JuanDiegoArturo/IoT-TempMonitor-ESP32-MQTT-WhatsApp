#include <WiFi.h>
#include <TFT_eSPI.h>
#include <PubSubClient.h>
#include <SPI.h>
#include "DHT.h"

#define BUTTON_LEFT 0

// Update these with values suitable for your network.

const char *ssid = "xxxxxxxxx";
const char *password = "xxxxxxxxx";
const char *mqtt_server = "xxxxxxxxxx";

#define DHTPIN 27
#define DHTTYPE DHT11
#define LED1 32
#define LED2 33

TFT_eSPI tft = TFT_eSPI();
DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

bool toggle = true;

void isr()
{
  toggle = !toggle;
}

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Conectando a la red: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado a WiFi.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Llega mensaje del topic: ");
  Serial.print(topic);
  Serial.print(".. Mensaje ..");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    messageTemp += (char)message[i];
  }
  Serial.println();
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Intentando conexión MQTT...");
    if (client.connect("ESP32Client1"))
    {
      Serial.println("conectado");
    }
    else
    {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" intentando en 5 segundos");
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  dht.begin();

  tft.init();
  tft.fillScreen(TFT_BLACK);
  attachInterrupt(BUTTON_LEFT, isr, RISING);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 10000) // Ahora se ejecuta cada 10 segundos
  {
    lastMsg = now;

    float humedad = dht.readHumidity();
    float temperatura = dht.readTemperature();

    if (isnan(humedad) || isnan(temperatura))
    {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    Serial.print(F("Humedad: "));
    Serial.print(humedad);
    Serial.print(F("% Temperatura: "));
    Serial.print(temperatura);
    Serial.println(F("°C "));

    char payload[50];
    snprintf(payload, 50, "{ \"temperature\": %.2f, \"humidity\": %.2f }", temperatura, humedad);
    client.publish("sensors/dht11", payload);
  }
}
