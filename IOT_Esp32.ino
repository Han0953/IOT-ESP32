\#include \<Servo.h>
\#include \<EEPROM.h>

Servo baseServo;     // Servo 1: Pemutar lengan
Servo gripperServo;  // Servo 2: Capit
Servo extServo;      // Servo 3: Pemanjang lengan
Servo liftServo;     // Servo 4: Pengangkat lengan

const int pinBase    = 10;
const int pinGripper = 9;
const int pinExt     = 5;
const int pinLift    = 7;

const int joyLeftX  = A0;
const int joyLeftY  = A1;
const int joyRightX = A2;
const int joyRightY = A3;

float angleBase, angleGripper, angleExt, angleLift;

unsigned long previousMillis = 0;
const int interval = 15;
unsigned long lastSaveTime = 0;

void setup() {
Serial.begin(9600);

// Baca memori lama biar gak nyentak balik ke 90 derajat
angleBase    = EEPROM.read(0);
angleGripper = EEPROM.read(1);
angleExt     = EEPROM.read(2);
angleLift    = EEPROM.read(3);

// Proteksi kalau data EEPROM corrupt
if (angleBase > 180 || angleBase < 0) angleBase = 90;
if (angleGripper > 180 || angleGripper < 0) angleGripper = 90;
if (angleExt > 180 || angleExt < 0) angleExt = 90;
if (angleLift > 180 || angleLift < 0) angleLift = 90;

// Set posisi sebelum di-attach
baseServo.write((int)angleBase);
gripperServo.write((int)angleGripper);
extServo.write((int)angleExt);
liftServo.write((int)angleLift);
}

void loop() {
unsigned long currentMillis = millis();

if (currentMillis - previousMillis >= interval) {
previousMillis = currentMillis;

```
int valLeftX  = analogRead(joyLeftX);
int valLeftY  = analogRead(joyLeftY);
int valRightX = analogRead(joyRightX);
int valRightY = analogRead(joyRightY);

// Deteksi pergerakan tuas (di luar deadzone 480-540)
bool isBaseMoving    = (valLeftX < 480 || valLeftX > 540);  // Kiri X
bool isLiftMoving    = (valLeftY < 480 || valLeftY > 540);  // Kiri Y (Naik-Turun)
bool isExtMoving     = (valRightX < 480 || valRightX > 540); // Kanan X (Maju-Mundur)
bool isGripperMoving = (valRightY < 480 || valRightY > 540); // Kanan Y (Capit)

// =======================================================
// BAGIAN KONTROL JOYSTICK (SUDAH DIUPDATE)
// =======================================================

// ---- KONTROL JOYSTICK KIRI ----
if (isBaseMoving) {
  aktifkanSatuServo(1); // Nyalakan Base aja
  updateServoAngle(valLeftX, angleBase, baseServo);
} 
else if (isLiftMoving) {
  aktifkanSatuServo(4); // Nyalakan Lift (Naik/Turun) aja
  updateServoAngle(valLeftY, angleLift, liftServo);
} 

// ---- KONTROL JOYSTICK KANAN ----
else if (isExtMoving) {
  aktifkanSatuServo(3); // Nyalakan Ext (Maju/Mundur Lengan) aja
  updateServoAngle(valRightX, angleExt, extServo);
} 
else if (isGripperMoving) {
  aktifkanSatuServo(2); // Nyalakan Capit aja
  updateServoAngle(valRightY, angleGripper, gripperServo);
} 

// ---- KONDISI DIAM (JOYSTICK DI TENGAH) ----
else {
  aktifkanSatuServo(0); // Matikan arus ke semua servo

  // Simpan memori posisi terakhir setelah diam 1 detik
  if (millis() - lastSaveTime > 1000) {
    EEPROM.update(0, (int)angleBase);
    EEPROM.update(1, (int)angleGripper);
    EEPROM.update(2, (int)angleExt);
    EEPROM.update(3, (int)angleLift);
    lastSaveTime = millis();
  }
}
```

}
}

// Fungsi hemat listrik (hanya 1 servo nyala)
void aktifkanSatuServo(int activeId) {
if (activeId == 1) { if (!baseServo.attached()) baseServo.attach(pinBase); }
else { if (baseServo.attached()) baseServo.detach(); }

if (activeId == 2) { if (!gripperServo.attached()) gripperServo.attach(pinGripper); }
else { if (gripperServo.attached()) gripperServo.detach(); }

if (activeId == 3) { if (!extServo.attached()) extServo.attach(pinExt); }
else { if (extServo.attached()) extServo.detach(); }

if (activeId == 4) { if (!liftServo.attached()) liftServo.attach(pinLift); }
else { if (liftServo.attached()) liftServo.detach(); }
}

// Fungsi penggerak halus (smoothing)
void updateServoAngle(int joyVal, float &currentAngle, Servo &targetServo) {
float stepSpeed = 0.0;
if (joyVal < 480) {
stepSpeed = map(joyVal, 479, 0, 1, 10) / 10.0;
currentAngle -= stepSpeed;
}
else if (joyVal > 540) {
stepSpeed = map(joyVal, 541, 1023, 1, 10) / 10.0;
currentAngle += stepSpeed;
}
currentAngle = constrain(currentAngle, 0, 180);
targetServo.write((int)currentAngle);
}



Dari codingan ini kamu buatkan readmenya, ini arm robot
