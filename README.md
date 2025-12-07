# 🏥 IoT-Based IV Bag Monitoring System (ESP32 + Blynk + HX711)

This smart healthcare project monitors the IV bag fluid level in real time and alerts the medical staff before the bag runs dry.  
The system measures the weight of the IV bag using the **HX711 load cell amplifier**, displays the status on an **LCD**, and sends **critical alerts** to the **Blynk IoT Cloud** with buzzer indication.

---

## 🚀 Features

✔ Real-time measurement of IV fluid level  
✔ Percentage & milliliters shown on LCD display  
✔ Wireless monitoring through Blynk IoT App  
✔ Automatic alerts & buzzer when fluid gets low  
✔ Useful in hospitals to avoid accidental air injection  
✔ Smart automation with continuous monitoring

---

## 🛠️ Hardware Components

| Component | Quantity |
|----------|---------|
| ESP32 / ESP32-S3 Development Board | 1 |
| HX711 Load Cell Amplifier | 1 |
| Load Cell Sensor | 1 |
| I2C LCD Display (16x2) | 1 |
| Buzzer | 1 |
| Jumper Wires & Stand for IV Bag | As required |

---

## 🔌 Circuit Connections

| Module | Pin → ESP32 |
|--------|-------------|
| HX711 DT (DOUT) | GPIO 4 |
| HX711 SCK (CLK) | GPIO 5 |
| Buzzer | GPIO 10 |
| LCD SDA | GPIO 21 (default I2C) |
| LCD SCL | GPIO 22 (default I2C) |
| VCC (LCD, HX711) | 5V |
| GND | GND |

📎 Detailed wiring: `circuit_connection.txt`

---

## 📱 Blynk Virtual Pins & Alerts

| Data | Virtual Pin |
|------|-------------|
| IV Volume (mL) | V0 |
| Percentage Left (%) | V1 |

🔔 Events triggered on:
- **50%** → Warning
- **20%** → Critical Low Alert + Continuous Buzzer

---

## 📊 Working Concept

| Step | Action |
|------|-------|
| 1 | Weight of IV bag measured using HX711 |
| 2 | Converted into mL and % remaining |
| 3 | Displayed on LCD |
| 4 | Sent to Blynk Cloud |
| 5 | Alerts triggered when threshold drops |

Prevents air entering the bloodstream by notifying nurses early ⚠️

---

## 🔧 Software & Tools Used

- Arduino IDE
- ESP32 Board Package
- Blynk IoT App & Cloud
- HX711 Arduino Library
- LiquidCrystal_I2C Library

---

## 📡 How to Use

1️⃣ Install ESP32 board in Arduino IDE  
2️⃣ Install required libraries and select ESP32 board  
3️⃣ Update Wi-Fi + Blynk credentials in code  
4️⃣ Upload the program to ESP32  
5️⃣ Place IV bag on load cell  
6️⃣ Monitor on LCD & Blynk App

---

## 🔍 Calibration

Modify this line if measurements differ:

```cpp
float calibration_factor = 102500;
