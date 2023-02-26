#include <ESP8266WiFi.h>
#include <DHT.h>  // Including library for dht
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is connected to GPIO2 (D4) on the NodeMCU board
#define ONE_WIRE_BUS 2

//--------------------------------------------------------------------------------------
//WIFI DECLARAINON
//--------------------------------------------------------------------------------------
 
String apiKey = "IVS0M88ILN278QN4";     //  Enter your Write API key from ThingSpeak
 
const char *ssid =  "Palnati";     // replace with your wifi ssid and wpa2 key
const char *pass =  "Uk_palnati";
const char* server = "api.thingspeak.com";


//--------------------------------------------------------------------------------------
// Data wire is connected to GPIO2 (D4) on the NodeMCU board
//--------------------------------------------------------------------------------------
#define ONE_WIRE_BUS 2


//--------------------------------------------------------------------------------------
#define DHTPIN 0          //D3 pin where the dht11 is connected



//--------------------------------------------------------------------------------------
//RELAY MODULE
//--------------------------------------------------------------------------------------
const int RELAY1_PIN = D1;
const int RELAY2_PIN = D8;
const int RELAY3_PIN = D5;
const int RELAY4_PIN = D6;
// control pin 
const int COX_CONTROL = D2;


//--------------------------------------------------------------------------------------
//WATER LEVEL
//--------------------------------------------------------------------------------------
const int trig = D0;
const int echo = D7;


//--------------------------------------------------------------------------------------
//water temperatre
//--------------------------------------------------------------------------------------
// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);
// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

//--------------------------------------------------------------------------------------
//temparature and humidity
//--------------------------------------------------------------------------------------
DHT dht(DHTPIN, DHT11);


//--------------------------------------------------------------------------------------
//ph sensor declaration
//--------------------------------------------------------------------------------------
const int analogInPin = A0;
int sensorValue = 0;
unsigned long int avgValue;
float b;
int buf[10], temp;

//--------------------------------------------------------------------------------------
//water level ultra sonic snsor
//--------------------------------------------------------------------------------------



 
WiFiClient client;
 
void setup() 
{
  //--------------------------------------------------------------------------------------
  //WATER LEVEL
  //--------------------------------------------------------------------------------------
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT); 

  //---------------------------------------------------------------------------------------
  // set the relay and COX pins as outputs
  //--------------------------------------------------------------------------------------
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);
  pinMode(COX_CONTROL, OUTPUT);

  // initialize all relays to be off
  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, LOW);
  digitalWrite(RELAY3_PIN, LOW);
  digitalWrite(RELAY4_PIN, LOW);
  
  // initialize the COX control signal to be low
  digitalWrite(COX_CONTROL, LOW);

  
  //--------------------------------------------------------------------------------------
  //water temerature
  //--------------------------------------------------------------------------------------
       sensors.begin();
  
       Serial.begin(115200);
       pinMode(trig, OUTPUT);
       pinMode(echo, INPUT);
       //water temerature
       sensors.begin();
       delay(10);
       dht.begin();
       
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("Start Project PH");
 
}
 
void loop() 
{
  //--------------------------------------------------------------------------------------
  //water level
  //--------------------------------------------------------------------------------------
  long tr = 0, hr = 0, hp = 0;
  
  // Transmitting pulse
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  // Waiting for pulse
  tr = pulseIn(echo, HIGH);
  
  // Calculating distance 
  hr = tr / 58;
 
  hr = hr - 6 ;  // offset correction
  hr = 20 - hr;  // water height, 0 - 50 cm
  
  hp = 5 * hr;  // distance in %, 0-100 %
  
  // Sending to computer
  Serial.print(hp);
  // Serial.print(" cm\n");
  Serial.print("\n");
  
  delay(1000);


  
  //--------------------------------------------------------------------------------------
  //WATER TEMPARTURE
  //--------------------------------------------------------------------------------------
  sensors.requestTemperatures(); // Send the command to get temperatures
  float waterTempC = sensors.getTempCByIndex(0); // Read the water temperature in Celsius
  float waterTempF = sensors.getTempFByIndex(0); // Convert to Fahrenheit if desired

  Serial.print("Water temperature: ");
  Serial.print(waterTempC);
  Serial.print("°C / ");
  Serial.print(waterTempF);
  Serial.println("°F");

  delay(1000); // Wait for 1 second before taking another reading



  //water level ultra sonic snsor
  




  
//ph senosor 
for (int i = 0; i < 10; i++) {
  buf[i] = analogRead(analogInPin);
  delay(10);
  }
    for (int i = 0; i < 9; i++)
      {
        for (int j = i + 1; j < 10; j++)
          {
              if (buf[i] > buf[j])
          {
  temp = buf[i];
  buf[i] = buf[j];
  buf[j] = temp;

  }
  }
  }
 avgValue = 0;
 for (int i = 2; i < 8; i++)
 avgValue += buf[i];
 float pHVol = (float)avgValue * 5.0 / 1024 / 6;
 float phValue = -5.70 * pHVol + 21.34;
 Serial.print("sensor = ");
 Serial.println(phValue);
 //water pump condition
if (phValue > 8.0) {
  // pH value is too high
  // Activate relay 1 and 2
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, LOW);
  digitalWrite(RELAY3_PIN, LOW);
  digitalWrite(RELAY4_PIN, LOW);
} else if (phValue < 6.7) {
  // pH value is too low
  // Activate relay 3 and 4
digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, HIGH);
  digitalWrite(RELAY3_PIN, LOW);
  digitalWrite(RELAY4_PIN, LOW);
} else {
  // pH value is within the acceptable range
  // Deactivate all relays
  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, LOW);
  digitalWrite(RELAY3_PIN, LOW);
  digitalWrite(RELAY4_PIN, LOW);
}

// Control the relay module using the control pin
digitalWrite(COX_CONTROL, HIGH);
delay(1000); // Wait for 1 second
digitalWrite(COX_CONTROL, LOW);

  
  //temparature and humididt sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (h > 30)
 {
  digitalWrite(RELAY4_PIN, HIGH);
  delay(1000);
  digitalWrite(RELAY4_PIN, LOW);
  delay(1000);

  digitalWrite(COX_CONTROL, HIGH);
  delay(1000);
  digitalWrite(COX_CONTROL, LOW);
  delay(1000);
 }
  
      
    if (isnan(h) || isnan(t)) 
      {
        Serial.println("Failed to read from DHT sensor!");
        return;
        }
 
    if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
    {  
                            
      String postStr = apiKey;
      postStr +="&field1=";
      postStr += String(t);
      postStr +="&field2=";
      postStr += String(h);
      postStr +="&field3=";
      postStr += String(phValue);
      postStr +="&field4=";
      postStr += String(waterTempC);
      postStr +="&field5=";
      postStr += String(hp);
      postStr += "\r\n\r\n";
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n\n\n\n");
      client.print(postStr);
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.print(" degrees Celcius, Humidity: ");
      Serial.print(h);
      Serial.print(" ph value: ");
      Serial.print(phValue);
      Serial.print("Temperature: ");
      Serial.print(waterTempC);
      Serial.println("%. Send to Thingspeak.");
      Serial.print(hp);
      Serial.println("%. Send to Thingspeak.");
      }
   client.stop();
 
   Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
}
