#include <WiFi.h>
#include <HTTPClient.h>
#include <PZEM004Tv30.h>
#include <Wire.h>

// Initialize PZEM objects with hardware serial and pins
HardwareSerial mySerial1(1);
HardwareSerial mySerial2(2);
HardwareSerial mySerial3(3);
HardwareSerial mySerial4(4);
HardwareSerial mySerial5(5);

PZEM004Tv30 pzem1(mySerial1, 16, 17);
PZEM004Tv30 pzem2(mySerial2, 18, 19);
PZEM004Tv30 pzem3(mySerial3, 21, 22);
PZEM004Tv30 pzem4(mySerial4, 23, 25);
PZEM004Tv30 pzem5(mySerial5, 26, 27);

#define WIFI_SSID "PGCJio"
#define WIFI_PASSWORD "987654321"
#define LED_BUILTIN 2

void setup() {
    Serial.begin(9600);

    // Initialize LED pin if available on your ESP32 module
    #ifdef LED_BUILTIN
    pinMode(LED_BUILTIN, OUTPUT);
    #endif

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    // Turn on LED if available
    #ifdef LED_BUILTIN
    digitalWrite(LED_BUILTIN, LOW);
    delay(15000);
    digitalWrite(LED_BUILTIN, HIGH);
    #endif
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        float voltage[5], current[5], power[5], energy[5], frequency[5], pf[5];

        voltage[0] = pzem1.voltage();   current[0] = pzem1.current();   power[0] = pzem1.power();   energy[0] = pzem1.energy();   frequency[0] = pzem1.frequency();   pf[0] = pzem1.pf();
        voltage[1] = pzem2.voltage();  current[1] = pzem2.current();  power[1] = pzem2.power();  energy[1] = pzem2.energy();  frequency[1] = pzem2.frequency();  pf[1] = pzem2.pf();
        voltage[2] = pzem3.voltage();  current[2] = pzem3.current();  power[2] = pzem3.power();  energy[2] = pzem3.energy();  frequency[2] = pzem3.frequency();  pf[2] = pzem3.pf();
        voltage[3] = pzem4.voltage();  current[3] = pzem4.current();  power[3] = pzem4.power();  energy[3] = pzem4.energy();  frequency[3] = pzem4.frequency();  pf[3] = pzem4.pf();
        voltage[4] = pzem5.voltage();  current[4] = pzem5.current();  power[4] = pzem5.power();  energy[4] = pzem5.energy();  frequency[4] = pzem5.frequency();  pf[4] = pzem5.pf();

        String postData = "";
        for (int i = 0; i < 5; i++) {
            postData += "Voltage" + String(i+1) + "=" + String(voltage[i]) + "&";
            postData += "Current" + String(i+1) + "=" + String(current[i]) + "&";
            postData += "Power" + String(i+1) + "=" + String(power[i]) + "&";
            postData += "Frequency" + String(i+1) + "=" + String(frequency[i]) + "&";
            postData += "PowerFactor" + String(i+1) + "=" + String(pf[i]) + "&";
            postData += "Energy" + String(i+1) + "=" + String(energy[i]) + "&";
        }
        postData.remove(postData.length() - 1); // Remove the last "&"

        Serial.println("Starting HTTP request...");

        http.begin("http://pgcresearch.co.in/PG/RengaIllam/InsertDBRegaIllam.php");
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        int httpCode = http.POST(postData);
        if (httpCode > 0) {
            String payload = http.getString();
            Serial.println(httpCode);
            Serial.println(payload);
            Serial.println(postData);
        } else {
            Serial.print("Error on HTTP request: ");
            Serial.println(httpCode);
            
        }

        http.end();
        delay(3900);
        digitalWrite(LED_BUILTIN, 1);
        delay(100);
        digitalWrite(LED_BUILTIN, 0);
        
    } else {
        Serial.println("WiFi not connected");
    }
}
