#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RtcDS1302.h>
#include <DHT.h>  // Include the DHT library

ThreeWire myWire(5, 6, 7);
RtcDS1302<ThreeWire> Rtc(myWire);

const int VRX_PIN = A0;    // Joystick X-axis
const int VRY_PIN = A1;    // Joystick Y-axis
const int BUTTON_PIN = 8;  // Button pin
const int LEFT_THRESHOLD = 200;
const int RIGHT_THRESHOLD = 850;
const int UP_THRESHOLD = 850;
const int DOWN_THRESHOLD = 200;

bool initialDisplayShown = false;
bool initialDisplayShown1 = false;
bool initialDisplayShown2 = false;


int xValue = 0;
int yValue = 0;
int buttonState = 0;
int mode = 0;  // Variable to keep track of the mode

#define DHTPIN 2
#define DHTTYPE DHT11  // Use DHT11 or DHT22 based on your sensor

DHT dht(DHTPIN, DHTTYPE);

const int buttonPin = 3;  // Change this to the actual button pin
int displayMode = 0;      // 0: time/date, 1: temperature/humidity, 2: PC mode

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
  Rtc.Begin();
  dht.begin();
  pinMode(buttonPin, INPUT_PULLUP);  // Set button pin as input with internal pull-up resistor

  // Create the custom characters
  lcd.createChar(0, upArrow);
  lcd.createChar(1, downArrow);
  lcd.createChar(2, leftArrow);
  lcd.createChar(3, rightArrow);
}

void loop() {

  sendJoystickCommand();
  checkRFID();

  RtcDateTime now = Rtc.GetDateTime();

  if (digitalRead(buttonPin) == LOW) {
    // Button is pressed; cycle through display modes
    displayMode = (displayMode + 1) % 3;
    lcd.clear();
    delay(200);  // Debounce delay
  }

  switch (displayMode) {
    case 0:
      printDateTime(now);
      break;
    case 1:
      printTempHumidity();
      break;
    case 2:
      updateDisplay();
      break;
    default:
      // Handle unexpected mode (optional)
      break;
  }
  // delay(100);
}

void sendJoystickCommand()
{
    int xValue = analogRead(VRX_PIN);
    int yValue = analogRead(VRY_PIN);
    int buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == LOW) {
        mode = (mode + 1) % 3; // Cycle through 3 modes
        delay(300); // Debounce delay
    }

    String command = "";

    switch (mode) {
        case 0:
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

void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);

  switch (mode) {
    case 0:
    initialDisplayShown2 = false;

    if (!initialDisplayShown) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("     MODE  1    ");
        lcd.setCursor(0, 1);
        lcd.print("  WINDOWS MODE  ");
        delay(2000);
        initialDisplayShown = true; // Mark as shown
    }

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
      lcd.print("Vol UP");
      lcd.setCursor(8, 0);
      lcd.write(byte(1));
      lcd.print("Vol Down");
      lcd.setCursor(0, 1);
      lcd.write(byte(2));
      lcd.print("Vol UP");
      lcd.setCursor(8, 1);
      lcd.write(byte(3));
      lcd.print("Vol Down");
      delay(100);
      break;

    case 1:
    initialDisplayShown = false;

    if (!initialDisplayShown1) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("     MODE  2    ");
        lcd.setCursor(0, 1);
        lcd.print("  BROWSER MODE  ");
        delay(2000);
        initialDisplayShown1 = true; // Mark as shown
    }
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
      delay(100);
      break;
    case 2:
  initialDisplayShown1 = false;

    if (!initialDisplayShown2) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("     MODE  2    ");
        lcd.setCursor(0, 1);
        lcd.print("   PLAYER MODE  ");
        delay(2000);
        initialDisplayShown2 = true; // Mark as shown
    }
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
      delay(100);
      break;
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
void printDateTime(const RtcDateTime& dt)
{
    char timeString[15]; // HH:MM:SS format
    char dateString[12]; // MM/DD/YYYY format
    char dayString[10]; // Day of the week

    // Format the time in 12-hour format with seconds and AM/PM
    snprintf_P(timeString,
               countof(timeString),
               PSTR("  %02u:%02u:%02u %s"),
               (dt.Hour() % 12 == 0) ? 12 : dt.Hour() % 12,
               dt.Minute(),
               dt.Second(),
               (dt.Hour() < 12) ? "AM" : "PM"); // Add AM/PM label here

    // Format the date in MM/DD/YYYY format
    snprintf_P(dateString,
               countof(dateString),
               PSTR(" %02u/%02u/%04u"),
               dt.Month(),
               dt.Day(),
               dt.Year());

    // Get the day of the week (0 = Sunday, 6 = Saturday)
    const char* daysOfWeek[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    snprintf_P(dayString,
               countof(dayString),
               PSTR("%s"),
               daysOfWeek[dt.DayOfWeek()]);

    // Update only the relevant parts of the display
    lcd.setCursor(0, 0);
    lcd.print(timeString);

    lcd.setCursor(0, 1);
    lcd.print(dateString);
    lcd.print(" ");
    lcd.print(dayString);
}

void printTempHumidity()
{
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature(); // In Celsius

    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature, 1); // Display one decimal place

    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity, 1); // Display one decimal place
}
