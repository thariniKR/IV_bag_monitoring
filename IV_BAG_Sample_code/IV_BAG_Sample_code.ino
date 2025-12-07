// ------------------------------
// Blynk IoT Configuration
// ------------------------------
#define BLYNK_TEMPLATE_ID "TMPL3a8108C8H"
#define BLYNK_TEMPLATE_NAME "IOT bsed IV Bag monitor"
#define BLYNK_AUTH_TOKEN "L17-***************************"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"

// ------------------------------
// LCD Setup (16x2)
// ------------------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);  // define LCD only once

// ------------------------------
// HX711 Pins for ESP32-S3
// ------------------------------
#define DOUT 4   // HX711 DT
#define CLK 5    // HX711 SCK
HX711 scale;   // Create HX711 object without pins

// ------------------------------
// Buzzer Pin
// ------------------------------
#define BUZZER 10

// ------------------------------
// WiFi Credentials
// ------------------------------
char ssid[] = "groot+";     
char pass[] = "12345678";   

// ------------------------------
// Variables
// ------------------------------
int liter;
int val;
float weight;
float calibration_factor = 102500; 
int last_val = -1; // track last percentage for comparison

// ------------------------------
// Setup
// ------------------------------
void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  // ------------------------------
  // WiFi Connection
  // ------------------------------
  lcd.clear();
  lcd.print("Connecting WiFi...");
  WiFi.begin(ssid, pass);
  int wifi_attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifi_attempts++;
    if (wifi_attempts > 40) {
      Serial.println("\nWiFi Failed! Restarting...");
      lcd.clear();
      lcd.print("WiFi Failed!");
      delay(2000);
      ESP.restart();
    }
  }

  Serial.println();
  Serial.println("WiFi Connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP().toString());
  delay(2000);

  // ------------------------------
  // HX711 Initialization
  // ------------------------------
  Serial.println("Remove all weight from scale");
  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare();
  long zero_factor = scale.read_average();
  Serial.print("Zero factor: ");
  Serial.println(zero_factor);

  // ------------------------------
  // Blynk Connection
  // ------------------------------
  Blynk.config(BLYNK_AUTH_TOKEN);
  if (Blynk.connect()) {
    Serial.println("Blynk Connected!");
    lcd.clear();
    lcd.print("Blynk Connected");
  } else {
    Serial.println("Blynk Connection Failed!");
    lcd.clear();
    lcd.print("Blynk Failed!");
  }
  delay(1500);
}

// ------------------------------
// Main Loop
// ------------------------------
void loop() {
  Blynk.run();
  measureweight();
}

// ------------------------------
// Measure Weight Function
// ------------------------------
void measureweight() {
  scale.set_scale(calibration_factor);
  weight = scale.get_units(5);

  if (weight < 0) weight = 0.0;

  liter = weight * 1000;  
  val = map(liter, 0, 505, 0, 100);

  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IV:");
  lcd.print(liter);
  lcd.print("mL");
  lcd.setCursor(0, 1);
  lcd.print("Bag:");
  lcd.print(val);
  lcd.print("%");

  // Serial Output
  Serial.print("Weight: ");
  Serial.print(weight, 3);
  Serial.println(" Kg");
  Serial.print("IV Bottle: ");
  Serial.print(liter);
  Serial.println(" mL");
  Serial.print("IV Bag Percent: ");
  Serial.print(val);
  Serial.println("%");
  Serial.println();

  // ------------------------------
  // Alerts and Buzzer Logic
  // ------------------------------
  if (val != last_val) { // Only act if value changes
    if (val <= 50 && val >= 40) {
      Blynk.logEvent("iv_alert", "IV Bottle is 50%");
      digitalWrite(BUZZER, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);
      delay(100);
    } 
    else if (val <= 20) {
      Blynk.logEvent("iv_alert", "IV Bottle is too LOW");
      digitalWrite(BUZZER, HIGH);
    } 
    else {
      digitalWrite(BUZZER, LOW);
    }
    last_val = val; // update last value
  }

  // ------------------------------
  // Send data to Blynk
  // ------------------------------
  Blynk.virtualWrite(V0, liter);
  Blynk.virtualWrite(V1, val);

  delay(500);
}
