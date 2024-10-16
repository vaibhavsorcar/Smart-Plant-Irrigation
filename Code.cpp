#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <DHT.h>
char auth[] = "";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";
//DHT11 for reading temperature and humidity value
#define Light D2
#define DHTPIN D1
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
float moisture;
void sendSensor()
{
float h = dht.readHumidity();
float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
if (isnan(h) || isnan(t))
{
Serial.println("Failed to read from DHT sensor!");
return;
}
Blynk.virtualWrite(V2, t);
Blynk.virtualWrite(V1, h);
}
void setup()
{
// Debug console
Serial.begin(9600);

pinMode(Light,OUTPUT);
Blynk.begin(auth, ssid, pass);
digitalWrite(Light,HIGH);
dht.begin();
// Setup a function to be called every second
timer.setInterval(1000L, sendSensor);
}
void loop()
{
Blynk.virtualWrite(V3,analogRead(A0));
moisture=analogRead(A0);
Serial.print("Moisture");
Serial.println(moisture);
if(analogRead(A0) <= 600)
{ 
digitalWrite(Light,HIGH);
Blynk.notify("Pump Stop, Water not Flowing");
Serial.println("Pump Stop, Water not Flowing");
delay(2000);
}
else if(analogRead(A0)>= 650)
{
Serial.println("Pump started, Water Flowing");
Blynk.notify("Pump Started, Water Flowing");
digitalWrite(Light,LOW);
delay(2000);
}
Blynk.run();
timer.run();
}
