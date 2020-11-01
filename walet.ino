#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <BlynkSimpleEsp32.h>


//Pin yang dipakai
const int lightSensorPin = 15;
const int tempSensorPin = 4;
const int ledLuxPin = 5;
const int ledCelPin = 18;

//Koneksi ke blynk dan wifi
char auth[] = "4oVixRDapxxhFSCHBWaQFdMJ";
char ssid[] = "balungakere";
char password[] = "belokan99";

//Data
int sensorLightValue = 0;
int lux = 0;
float sensorTempValue = 0.0;
float celcius = 0;
int luxTH = 0;
int celciusTH = 0;
int state = 0;
DHT dht11(tempSensorPin, DHT11);


//Widget
WidgetLED ledLux(V6);
WidgetLED ledCelcius(V7);

//GetData from BlynkApp
BLYNK_WRITE(V4) {
  luxTH = param.asInt() * 0.009768 + 10;
}
BLYNK_WRITE(V5) {
  celciusTH = param.asInt();
}
BLYNK_WRITE(V8) {
  state = param.asInt();
}
void setup() {
  Serial.begin(9600);
  // pinMode
  pinMode(tempSensorPin, INPUT);
  pinMode(ledLuxPin, OUTPUT);
  pinMode(ledCelPin, OUTPUT);

  Blynk.begin(auth, ssid, password, IPAddress(192, 168, 8, 100), 8080);
  dht11.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  if (state) {
    sensorLightValue = analogRead(lightSensorPin);
    sensorTempValue = dht11.readTemperature();
    lux = 0.009768 * sensorLightValue + 10;
    if (!isnan (sensorTempValue) ) {
      celcius = sensorTempValue;
    }
    Blynk.virtualWrite(V0, lux);
    Blynk.virtualWrite (V1, celcius);
    Blynk.virtualWrite(V2, lux);
    Blynk.virtualWrite(V3, celcius);
    if (lux > luxTH) {
      ledLux.on();
      digitalWrite(ledLuxPin, HIGH);
    } else {
      ledLux.off();
      digitalWrite(ledLuxPin, LOW);
    }
    if (celcius > celciusTH) {
      ledCelcius.on();
      digitalWrite(ledCelPin, HIGH);
    } else {
      ledCelcius.off();
      digitalWrite(ledCelPin, LOW);
    }
    Serial.print("LUX: ");
    Serial.print(lux);
    Serial.print(" Temp: ");
    Serial.print(celcius);
    Serial.print(" luxTH: ");
    Serial.print(luxTH);
    Serial.print(" celciusTH: ");
    Serial.println(celciusTH);
  } else {
    digitalWrite(ledLuxPin, LOW);
    digitalWrite(ledCelPin, LOW);
  }
  delay(200);
}
