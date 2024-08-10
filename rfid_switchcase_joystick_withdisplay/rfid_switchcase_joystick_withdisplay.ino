#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int VRX_PIN = A0;    // Joystick X-axis
const int VRY_PIN = A1;    // Joystick Y-axis
const int BUTTON_PIN = 8;  // Button pin
const int LEFT_THRESHOLD = 200;
const int RIGHT_THRESHOLD = 850;
const int UP_THRESHOLD = 850;
const int DOWN_THRESHOLD = 200;

int xValue = 0;
int yValue = 0;
int buttonState = 0;
int mode = 0;  // Variable to keep track of the mode

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display

// Define the custom characters for the arrows
byte upArrow[8] = {
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};

byte downArrow[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

byte leftArrow[8] = {
  0b00100,
  0b01100,
  0b11100,
  0b11111,
  0b11100,
  0b01100,
  0b00100,
  0b00000
};

byte rightArrow[8] = {
  0b00100,
  0b00110,
  0b00111,
  0b11111,
  0b00111,
  0b00110,
  0b00100,
  0b00000
};

bool hasRun = false;

void setup() {
  Serial.begin(9600);  // Start serial communication at 9600 baud
  pinMode(VRX_PIN, INPUT);
  pinMode(VRY_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Set button pin as input with pull-up resistor
  SPI.begin();                        // Init SPI bus
  mfrc522.PCD_Init();                 // Init MFRC522
  lcd.init();                         // Initialize the LCD
  lcd.backlight();                    // Turn on the backlight

  // Create the custom characters
  lcd.createChar(0, upArrow);
  lcd.createChar(1, downArrow);
  lcd.createChar(2, leftArrow);
  lcd.createChar(3, rightArrow);

  // Display the initial message
  lcd.setCursor(0, 0);
  lcd.print(" SCAN  RFID  TO ");
  lcd.setCursor(0, 1);
  lcd.print("   UNLOCK  PC   ");
  delay(5000);  // Display message for 5 seconds
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mode 0: Control");
  delay(2000);  // Display mode message for 2 seconds
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.print("Vol UP");
  lcd.setCursor(8, 0);
  lcd.write(byte(0));
  lcd.print("Vol Down");
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  lcd.print("Vol UP");
  lcd.setCursor(8, 1);
  lcd.write(byte(0));
  lcd.print("Vol Down");
}

void loop() {
  sendJoystickCommand();
  checkRFID();
  delay(100);
}

void sendJoystickCommand() {
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) {
    mode = (mode + 1) % 3;  // Cycle through 3 modes

    lcd.clear();
    lcd.print("Mode: ");
    lcd.print(mode);
    delay(300);  // Debounce delay
  }

  String command = "";

  switch (mode) {
    case 0:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
      lcd.print("Vol UP");
      lcd.setCursor(8, 0);
      lcd.write(byte(0));
      lcd.print("Vol Down");
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      lcd.print("Vol UP");
      lcd.setCursor(8, 1);
      lcd.write(byte(0));
      lcd.print("Vol Down");
      //holdScreen();
      if (xValue > UP_THRESHOLD) {
        command = "x1";  // Volume Up
      } else if (xValue < DOWN_THRESHOLD) {
        command = "x2";  // Volume Down
      } else if (yValue > RIGHT_THRESHOLD) {
        command = "y1";  // Volume Up
      } else if (yValue < LEFT_THRESHOLD) {
        command = "y2";  // Volume Down
      }
      delay(50);
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
      lcd.print("Vol UP");
      lcd.setCursor(8, 0);
      lcd.write(byte(0));
      lcd.print("Vol Down");
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      lcd.print("Vol UP");
      lcd.setCursor(8, 1);
      lcd.write(byte(0));
      lcd.print("Vol Down");
      if (xValue > UP_THRESHOLD) {
        command = "x3";  // Custom action 1
      } else if (xValue < DOWN_THRESHOLD) {
        command = "x4";  // Custom action 2
      } else if (yValue > RIGHT_THRESHOLD) {
        command = "y3";  // Custom action 3
      } else if (yValue < LEFT_THRESHOLD) {
        command = "y4";  // Custom action 4
      }
      delay(50);
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
      lcd.print("Vol UP");
      lcd.setCursor(8, 0);
      lcd.write(byte(0));
      lcd.print("Vol Down");
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      lcd.print("Vol UP");
      lcd.setCursor(8, 1);
      lcd.write(byte(0));
      lcd.print("Vol Down");
      if (xValue > UP_THRESHOLD) {
        command = "x5";  // Custom action 5
      } else if (xValue < DOWN_THRESHOLD) {
        command = "x6";  // Custom action 6
      } else if (yValue > RIGHT_THRESHOLD) {
        command = "y5";  // Custom action 7
      } else if (yValue < LEFT_THRESHOLD) {
        command = "y6";  // Custom action 8
      }
      delay(50);
      break;
  }

  if (command != "") {
    Serial.println(command);
    delay(50);
  }
}

void checkRFID() {
  // Look for new card
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String card_ID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      card_ID += String(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println(card_ID);  // Send the UID over serial
    delay(1000);              // Delay to avoid multiple reads
  }
}
