#include <SPI.h>
#include <MFRC522.h>

const int VRX_PIN = A0; // Joystick X-axis
const int VRY_PIN = A1; // Joystick Y-axis
const int LEFT_THRESHOLD = 200;
const int RIGHT_THRESHOLD = 850;
const int UP_THRESHOLD = 850;
const int DOWN_THRESHOLD = 200;

int xValue = 0;
int yValue = 0;


#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud
  pinMode(VRX_PIN, INPUT);
  pinMode(VRY_PIN, INPUT);
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
}

void loop() {
  sendJoystickCommand();
  checkRFID();
  delay(100);
}

void sendJoystickCommand() {

  int xValue = analogRead(A0);
  int yValue = analogRead(A1);
  String xcommand = "";
  String ycommand = "";

  // Serial.println(xValue);
  // Serial.println(yValue);

  if (xValue > UP_THRESHOLD) {
    xcommand = "x1";  // Volume Up
    Serial.println(xcommand);
    delay(50);
    
  } else if (xValue < DOWN_THRESHOLD) {
    xcommand = "x2";  // Volume Down
    Serial.println(xcommand);
     delay(50);
   
  }
  else{
    xcommand = "" ;
  }

  if (yValue > RIGHT_THRESHOLD) {
    ycommand = "y1";  // Volume Up
    Serial.println(ycommand);
    delay(50);
    
  } else if (yValue < LEFT_THRESHOLD) {
    ycommand = "y2";  // Volume Down
    Serial.println(ycommand);
    delay(50);
  }
  else{
  ycommand = "" ;
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
    delay(1000);  // Delay to avoid multiple reads
  }
}
