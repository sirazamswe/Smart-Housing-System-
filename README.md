# 🏠 Smart Housing System

An ESP32-based smart home automation system that integrates multiple sensors and actuators to provide security, safety, and convenience features — all protected by a keypad password system.

---

## 👩‍💻 Authors

| Role | Name |
|------|------|
| **Author** | Jeb-Un-Nesa Tithy |
| **Author** | Mst. Sirazam Monira |
| **Co-Author** | Nishat Pranto |
| **Co-Author** | Shahriar Kabir |

---

## 📋 Features

- 🔐 **Password-Protected Access** — 4×4 keypad with configurable PIN (default: `1234`)
- 🔥 **Flame Detection** — Triggers buzzer and LED alert on fire detection
- 💧 **Water Level Monitoring** — Ultrasonic sensor monitors tank level (LOW / MEDIUM / FULL)
- 🗑️ **Smart Dustbin** — Servo-controlled lid opens automatically when trash is detected nearby
- 🚶 **Motion Detection (PIR)** — Automatically turns on the LED when motion is detected
- 📟 **LCD Display** — Real-time status shown on a 16×2 I2C LCD screen
- 🔊 **Buzzer Alerts** — Audible warnings for flame or low water level

---

## 🛠️ Hardware Requirements

| Component | Quantity |
|-----------|----------|
| ESP32 Development Board | 1 |
| 16×2 I2C LCD Display (0x27) | 1 |
| 4×4 Matrix Keypad | 1 |
| Flame Sensor (Digital) | 1 |
| PIR Motion Sensor | 1 |
| HC-SR04 Ultrasonic Sensor (Water Tank) | 1 |
| HC-SR04 Ultrasonic Sensor (Dustbin) | 1 |
| Servo Motor (SG90 or similar) | 1 |
| Buzzer | 1 |
| LED | 1 |
| Resistors / Jumper Wires / Breadboard | As needed |

---

## 📌 Pin Configuration

| Component | Pin |
|-----------|-----|
| Flame Sensor | GPIO 35 |
| LED | GPIO 2 |
| Buzzer | GPIO 15 |
| PIR Sensor | GPIO 19 |
| Water Ultrasonic Trig | GPIO 4 |
| Water Ultrasonic Echo | GPIO 16 |
| Dustbin Ultrasonic Trig | GPIO 17 |
| Dustbin Ultrasonic Echo | GPIO 5 |
| Servo Motor | GPIO 18 |
| Keypad Rows | GPIO 14, 27, 26, 25 |
| Keypad Cols | GPIO 32, 33, 23, 13 |
| LCD SDA/SCL | Default I2C (GPIO 21/22) |

---

## 📦 Dependencies / Libraries

Install the following libraries via the **Arduino Library Manager** or **PlatformIO**:

- [`Wire`](https://www.arduino.cc/en/reference/wire) — I2C communication (built-in)
- [`LiquidCrystal_I2C`](https://github.com/johnrickman/LiquidCrystal_I2C) — LCD display
- [`Keypad`](https://github.com/Chris--A/Keypad) — Matrix keypad input
- [`ESP32Servo`](https://github.com/madhephaestus/ESP32Servo) — Servo motor control for ESP32

---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/your-username/smart-housing-system.git
cd smart-housing-system
```

### 2. Open in Arduino IDE

- Open `smart_housing_system.ccp` in Arduino IDE
- Select **Board:** `ESP32 Dev Module`
- Select the correct **COM Port**

### 3. Install Libraries

Go to **Sketch → Include Library → Manage Libraries** and install all listed dependencies.

### 4. Upload the Code

Click **Upload** and open the **Serial Monitor** at `9600 baud` for debug output.

---

## 🔑 Default Password

```
1234
```

- Press `#` to **confirm** the password
- Press `*` to **clear** the input

> ⚠️ You can change the password by modifying the `password` variable in the source code:
> ```cpp
> String password = "1234";
> ```

---

## ⚙️ System Logic Overview

```
System Start
    │
    ▼
Enter Password (Keypad)
    │
    ├─ Wrong → Show error, retry
    │
    └─ Correct → Unlock System
                    │
                    ▼
            ┌───────────────────┐
            │  Read All Sensors  │
            └────────┬──────────┘
                     │
          ┌──────────┼───────────────┐
          ▼          ▼               ▼
     Flame?       Water Low?     Bin Full?
     Buzzer ON    Buzzer ON     Servo Opens
     LED ON       LED ON        Buzzer ON
          │
          └─── No alerts? Check PIR Motion
                              │
                         Motion? → LED ON (2s)
```

---

## 📊 Water Level Thresholds

| Raw Distance (cm) | Status |
|-------------------|--------|
| ≥ 4 | LOW ⚠️ (Buzzer ON) |
| 2 – 3 | MEDIUM |
| ≤ 1 | FULL ✅ |

---

## 📁 Project Structure

```
smart-housing-system/
│
├── smart_housing_system.ccp   # Main source code
└── README.md                  # Project documentation
```

---


> Built with ❤️ using ESP32 and Arduino framework.