#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <ESP32Servo.h>

// -------------------- Pins --------------------
const int flamePin = 35;      
const int ledPin = 2;
const int buzzerPin = 15;
const int pirPin = 19;
const int trigWater = 4;
const int echoWater = 16;
const int trigDustbin = 17;
const int echoDustbin = 5;
const int servoPin = 18;

// -------------------- Thresholds --------------------
const int dustbinDistanceLimit = 7; // 7 cm
const int dustbinPriorityFlame = 7; // Flame has priority if bin < 7cm
const int waterEmptyBaseline = 4;   // baseline reading when tank is empty

// -------------------- Objects --------------------
Servo dustbinServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4; 
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {14, 27, 26, 25}; 
byte colPins[COLS] = {32, 33, 23, 13};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String password = "1234";
String inputPass = "";
bool systemActive = false;

void setup() {
 Serial.begin(9600);
  delay(1000); 
  Serial.println("--- System Initializing ---");

  pinMode(flamePin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(trigWater, OUTPUT);
  pinMode(echoWater, INPUT);
  pinMode(trigDustbin, OUTPUT);
  pinMode(echoDustbin, INPUT);

  dustbinServo.attach(servoPin);
  dustbinServo.write(0);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
  
  Serial.println("Setup Complete. Waiting for Password...");
}

void loop() {
  if (!systemActive) {
    handlePassword();
  } else {
    runSystemLogic();
  }
}

void handlePassword() {
  char key = keypad.getKey();
  if (key) {
    Serial.print("Key Pressed: "); Serial.println(key); 
    if (key == '#') {
      if (inputPass == password) {
        systemActive = true;
        lcd.clear();
        lcd.print("Access Granted");
        Serial.println("PASSWORD CORRECT: Access Granted");
        delay(2000);
        lcd.clear();
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Wrong Password");
        Serial.println("PASSWORD WRONG: Access Denied");
        delay(2000);
        lcd.setCursor(0, 1);
        lcd.print("                ");
      }
      inputPass = "";
    } else if (key == '*') {
      inputPass = "";
      lcd.setCursor(0, 1);
      lcd.print("                ");
    } else {
      inputPass += key;
      lcd.setCursor(inputPass.length() - 1, 1);
      lcd.print("*");
    }
  }
}

long getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 30000);
  if (duration == 0) return 999;
  return (duration * 0.034) / 2;
}

void runSystemLogic() {
  // --- Read Sensors ---
  long waterDistRaw = getDistance(trigWater, echoWater);
  long waterDist = waterEmptyBaseline - waterDistRaw;
  if (waterDist < 0) waterDist = 0;                   
  long binDist = getDistance(trigDustbin, echoDustbin);

  int flameVal = digitalRead(flamePin);
  int motionVal = digitalRead(pirPin);

  Serial.print("Water Level: "); Serial.print(waterDistRaw); Serial.print(" | ");
  Serial.print("Dustbin: "); Serial.print(binDist); Serial.print("cm | ");
  Serial.print("Flame: "); Serial.print(flameVal); Serial.print(" | ");
  Serial.print("PIR: "); Serial.println(motionVal);

  bool buzzerAlert = false;
  bool dustbinOpen = false;
  String statusMsg = "";

  // --- LCD Line 1: Water Level (UPDATED LOGIC) ---
  lcd.setCursor(0, 0);
  lcd.print("Water: ");
  if (waterDistRaw >= 4) {
    lcd.print("LOW       ");
    buzzerAlert = true;   // buzzer ON for low water
  } 
  else if (waterDistRaw >= 2) {
    lcd.print("MEDIUM    ");
  } 
  else if (waterDistRaw == 1) {
    lcd.print("FULL      ");
  } 
  else {
    lcd.print("FULL      ");
  }

  // --- Dustbin Logic ---
  if (binDist < dustbinDistanceLimit) {
    dustbinServo.write(180);
    dustbinOpen = true;
    statusMsg = "";
  } else {
    dustbinServo.write(0);
    statusMsg = "";
  }

  // --- Flame Sensor Logic ---
  if (flameVal == LOW) {
    buzzerAlert = true;
    statusMsg = "";
  }

  lcd.setCursor(0, 1);
  lcd.print(statusMsg);

  // --- Actuators ---
  if (buzzerAlert || dustbinOpen) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH); 
  } else {
    digitalWrite(buzzerPin, LOW);

    if (motionVal == HIGH) {
      digitalWrite(ledPin, HIGH);
      delay(2000);
      digitalWrite(ledPin, LOW);
    } else {
      digitalWrite(ledPin, LOW);
    }
  }

  delay(500); 
}
