#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RtcDS1302.h>
#include <DHT.h> // Include the DHT library

LiquidCrystal_I2C lcd(0x27, 16, 2);
ThreeWire myWire(5, 6, 7);
RtcDS1302<ThreeWire> Rtc(myWire);

// Initialize the DHT sensor (change pin number as needed)
#define DHTPIN 2
#define DHTTYPE DHT11 // Use DHT11 or DHT22 based on your sensor

DHT dht(DHTPIN, DHTTYPE);

const int buttonPin = 3; // Change this to the actual button pin
int displayMode = 0; // 0: time/date, 1: temperature/humidity, 2: PC mode

void setup()
{
    Serial.begin(9600);
    Rtc.Begin();
    lcd.init();
    lcd.backlight();
    dht.begin();

    pinMode(buttonPin, INPUT_PULLUP); // Set button pin as input with internal pull-up resistor

    // Other setup code...
}

void loop()
{
   RtcDateTime now = Rtc.GetDateTime();

   if (digitalRead(buttonPin) == LOW) {
        // Button is pressed; cycle through display modes
        displayMode = (displayMode + 1) % 3;
        lcd.clear();
        delay(200); // Debounce delay
    }

    switch (displayMode) {
        case 0:
            printDateTime(now);
            break;
        case 1:
            printTempHumidity();
            break;
        case 2:
            pcmode();
            break;
        default:
            // Handle unexpected mode (optional)
            break;
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

void pcmode()
{
    // Your custom logic for the third display mode (pcmode) goes here
    // For example, display some custom text or additional information
    lcd.setCursor(0, 0);
    lcd.print("PC Mode Active");
    // Add any other relevant content for this mode
}
// Rest of the code remains the same...
