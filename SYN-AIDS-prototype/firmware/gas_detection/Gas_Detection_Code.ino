#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// ==========================================
// LCD
// ==========================================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ==========================================
// MQ-2
// ==========================================
#define MQ2 A0

// ==========================================
// LEDs
// ==========================================
#define GREEN_LED 2
#define ORANGE_LED 3
#define RED_LED 4

// ==========================================
// BUZZER
// ==========================================
#define BUZZER 5

// ==========================================
// SIM800L
// SIM800L TX -> Nano D6
// SIM800L RX -> Nano D7
// ==========================================
SoftwareSerial sim800(6, 7);

// ==========================================
// PHONE NUMBER
// ==========================================
const char PHONE_NUMBER[] = "YOUR_PHONE_NUMBER";

// ==========================================
// GAS THRESHOLDS
// ==========================================
#define WARNING_LEVEL 300
#define DANGER_LEVEL 600

// ==========================================
// SMS FLAGS
// ==========================================
bool warningSent = false;
bool dangerSent = false;


// ==========================================
// SETUP
// ==========================================
void setup()
{
  Serial.begin(9600);
  sim800.begin(9600);

  // LED pins
  pinMode(GREEN_LED, OUTPUT);
  pinMode(ORANGE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Buzzer
  pinMode(BUZZER, OUTPUT);

  // Turn everything OFF
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(ORANGE_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);

  // ========================================
  // LCD START
  // ========================================
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("TOXIC GAS");
  lcd.setCursor(0, 1);
  lcd.print("MONITOR SYSTEM");

  delay(2000);

  // ========================================
  // MQ-2 WARMING
  // ========================================
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ-2 WARMING");
  lcd.setCursor(0, 1);
  lcd.print("Please wait");

  delay(10000);

  // ========================================
  // CHECK SIM800L
  // ========================================
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Checking SIM...");

  if (checkSIM())
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SIM CONNECTED");
    lcd.setCursor(0, 1);
    lcd.print("SYSTEM READY");
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SIM ERROR");
    lcd.setCursor(0, 1);
    lcd.print("Check Wiring");
  }

  delay(2000);
  lcd.clear();
}


// ==========================================
// MAIN LOOP
// ==========================================
void loop()
{
  // Read REAL MQ-2 value
  int gasValue = analogRead(MQ2);

  // Serial Monitor
  Serial.print("MQ-2 Value: ");
  Serial.println(gasValue);


  // ========================================
  // SAFE
  // ========================================
  if (gasValue < WARNING_LEVEL)
  {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(ORANGE_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("GAS: SAFE");

    lcd.setCursor(0, 1);
    lcd.print("Value: ");
    lcd.print(gasValue);

    // Reset SMS system
    warningSent = false;
    dangerSent = false;

    delay(1000);
  }


  // ========================================
  // WARNING
  // ========================================
  else if (gasValue >= WARNING_LEVEL &&
           gasValue < DANGER_LEVEL)
  {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(ORANGE_LED, HIGH);
    digitalWrite(RED_LED, LOW);

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("GAS: WARNING");

    lcd.setCursor(0, 1);
    lcd.print("Value: ");
    lcd.print(gasValue);

    // Buzzer beep
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);

    // ======================================
    // SEND WARNING SMS ONLY ONCE
    // ======================================
    if (!warningSent)
    {
      sendSMS(
        "WARNING! High gas/smoke level detected by MQ-2. Please check the area immediately."
      );

      warningSent = true;
    }

    delay(800);
  }


  // ========================================
  // DANGER
  // ========================================
  else
  {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(ORANGE_LED, LOW);
    digitalWrite(RED_LED, HIGH);

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("!!! DANGER !!!");

    lcd.setCursor(0, 1);
    lcd.print("Value: ");
    lcd.print(gasValue);

    // Danger buzzer
    digitalWrite(BUZZER, HIGH);
    delay(500);

    digitalWrite(BUZZER, LOW);
    delay(500);

    // ======================================
    // SEND DANGER SMS ONLY ONCE
    // ======================================
    if (!dangerSent)
    {
      sendSMS(
        "DANGER! Very high gas/smoke level detected by MQ-2. Evacuate the area immediately."
      );

      dangerSent = true;
    }
  }
}


// ==========================================
// CHECK SIM800L
// ==========================================
bool checkSIM()
{
  // Clear old data
  while (sim800.available())
  {
    sim800.read();
  }

  // Send AT command
  sim800.println("AT");

  unsigned long startTime = millis();

  String response = "";

  while (millis() - startTime < 3000)
  {
    if (sim800.available())
    {
      char c = sim800.read();

      response += c;

      Serial.write(c);
    }
  }

  Serial.println();

  // Check response
  if (response.indexOf("OK") >= 0)
  {
    return true;
  }

  return false;
}


// ==========================================
// SEND SMS
// ==========================================
void sendSMS(const char *message)
{
  Serial.println("Sending SMS...");

  // SMS text mode
  sim800.println("AT+CMGF=1");

  delay(1000);

  // Enter phone number
  sim800.print("AT+CMGS=\"");
  sim800.print(PHONE_NUMBER);
  sim800.println("\"");

  delay(1000);

  // Send message
  sim800.print(message);

  delay(500);

  // CTRL + Z
  sim800.write(26);

  delay(5000);

  Serial.println("SMS process completed.");
}
