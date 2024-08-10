#include "HID-Project.h"
#include <ezButton.h>
uint8_t buf[8] = { 0 };
#include <SPI.h>

#include <MFRC522.h>  // RFID library
#define SS_PIN 10     //RX slave select
#define RST_PIN 9
int gled = 7;  // optional
int rled = 4;  // optional

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
String card_ID = "";               //
const char* message = "2572002";
String rfid = "21581167122";       // UID (unique Id Code Of Your Rfid Tag)
void setup() {
  Serial.begin(9600);  // Initialize serial communications with the PC
  BootKeyboard.begin();
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card
  pinMode(gled, OUTPUT);
  pinMode(rled, OUTPUT);
}
void loop() {

  //look for new card
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;  //if read card serial(0) returns 1, the uid struct contians the ID of the read card.
  }
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    card_ID += mfrc522.uid.uidByte[i];
  }
  // Serial.println(card_ID);
  if (card_ID == rfid) {
    digitalWrite(gled, HIGH);
    BootKeyboard.write(KEY_RETURN);
    delay(1000);
    const char* message = "2572002";
        while (*message) {
            BootKeyboard.write(*message); // Send each character
            delay(50); // Delay between characters (adjust as needed)
            message++;
        }
        BootKeyboard.write(KEY_RETURN); // Press "Enter"
        delay(500); // Debounce delay
    
    digitalWrite(gled, LOW);
    delay(200);
    digitalWrite(gled, HIGH);
    delay(200);
    digitalWrite(gled, LOW);
  }
  if (card_ID != rfid) {
    digitalWrite(rled, HIGH);
    digitalWrite(rled, LOW);
    delay(200);
    digitalWrite(rled, HIGH);
    delay(200);
    digitalWrite(rled, LOW);
  } else {
    goto cont;
  }
  delay(1000);
cont:
  delay(1000);
  card_ID = "";
}

