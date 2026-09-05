// ===== BLYNK SETTINGS =====
#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_BLYNK_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>
#include <MFRC522.h>



char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "YOUR_WIFI_SSID"; // Replace with your Wi-Fi SSID
char pass[] = "YOUR_WIFI_PASSWORD"; // Replace with your Wi-Fi password
    
// ===== PIN DEFINITIONS =====
#define PIR_PIN 27
#define VIBRATION_PIN 26
#define BUZZER_PIN 25
#define BUZZER_CHANNEL 0
#define TRIG_PIN 33
#define ECHO_PIN 32

// ===== RFID PINS =====
#define SS_PIN 5
#define RST_PIN 22
MFRC522 rfid(SS_PIN, RST_PIN);

// ===== SYSTEM VARIABLES =====
bool authorizedAccess = false;
unsigned long accessStartTime = 0;
const unsigned long ACCESS_TIME = 10000;   // 10 seconds

// ===== ALARM VARIABLES =====
bool alarmActive = false;
unsigned long alarmStartTime = 0;
const unsigned long ALARM_DURATION = 5000; // 5 seconds

// ===== SENSOR SETTINGS =====
const int DISTANCE_THRESHOLD = 20;

// ===== SETUP =====
void setup() {
  Serial.begin(9600);

  pinMode(PIR_PIN, INPUT);
  pinMode(VIBRATION_PIN, INPUT_PULLDOWN);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  ledcSetup(BUZZER_CHANNEL, 2000, 8);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  ledcWrite(BUZZER_CHANNEL, 0);

  SPI.begin();
  rfid.PCD_Init();

  // Start Blynk
  Blynk.begin(auth, ssid, pass);

  Serial.println("Museum Security System Ready");
}

// ===== ULTRASONIC DISTANCE FUNCTION =====
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

// ===== ALARM FUNCTION (non-blocking) =====
void triggerAlarm() {
  if (!alarmActive) {
    alarmActive = true;
    alarmStartTime = millis();
    ledcWrite(BUZZER_CHANNEL, 180);           // Turn buzzer ON
    Blynk.virtualWrite(V4, "ALARM TRIGGERED"); // Dashboard
    Blynk.logEvent("⚠ ALARM TRIGGERED! Check artifact!");
    Serial.println("🚨 ALARM TRIGGERED 🚨");
  }
}

// ===== LOOP =====
void loop() {
  Blynk.run(); // Keep Blynk connected

  // ===== RFID CHECK =====
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String uid = "";
    Serial.print("Card UID: ");
    for (byte i = 0; i < rfid.uid.size; i++) {
      Serial.print(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      Serial.print(rfid.uid.uidByte[i], HEX);
      Serial.print(" ");
      uid += String(rfid.uid.uidByte[i], HEX);
    }
    Serial.println();

    // ===== AUTHORIZED CARD =====
    if (uid.equalsIgnoreCase("YOUR_AUTHORIZED_RFID_UID")) { // Only this card
      Serial.println("Authorized Access — Sensors Disabled");
      authorizedAccess = true;
      accessStartTime = millis();
      Blynk.virtualWrite(V3, "Authorized: " + uid); // RFID Label
    } else {
      Serial.println("Unauthorized Card");
      Blynk.virtualWrite(V3, "Unauthorized: " + uid);
      triggerAlarm();
    }

    rfid.PICC_HaltA();
  }

  // ===== CHECK IF ACCESS TIME ENDED =====
  if (authorizedAccess && millis() - accessStartTime > ACCESS_TIME) {
    authorizedAccess = false;
    Serial.println("Sensors Active Again");
  }

  // ===== READ SENSORS IF NOT AUTHORIZED =====
  if (!authorizedAccess) {
    int pirState = digitalRead(PIR_PIN);
    int vibState = digitalRead(VIBRATION_PIN);
    long distance = getDistance();

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Update Blynk dashboard real-time
    Blynk.virtualWrite(V0, pirState);       // PIR
    Blynk.virtualWrite(V1, distance);       // Ultrasonic
    Blynk.virtualWrite(V2, vibState);       // Vibration

    // PIR Detection
    if (pirState == HIGH) Serial.println("⚠ Motion detected near artifact");

    // Ultrasonic Detection
    if (distance > 0 && distance < DISTANCE_THRESHOLD) triggerAlarm();

    // Vibration Detection
    if (vibState == HIGH) triggerAlarm();
  }

  // ===== CHECK IF ALARM DURATION ENDED =====
  if (alarmActive && millis() - alarmStartTime >= ALARM_DURATION) {
    alarmActive = false;
    ledcWrite(BUZZER_CHANNEL, 0);            // Buzzer OFF
    Blynk.virtualWrite(V4, "ALARM OFF");      // Dashboard
    Serial.println("Alarm OFF");
  }

  delay(100);
}
