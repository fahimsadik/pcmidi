#include <Wire.h> // Include the Wire library for I2C communication
#include <LiquidCrystal_I2C.h> // Include the LiquidCrystal_I2C library

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD object (address 0x27, 16 columns, 2 rows)

#include <RtcDS1302.h>

ThreeWire myWire(5, 6, 7); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

void setup()
{
    Serial.begin(9600);
    Rtc.Begin();

    // Initialize the I2C LCD with the appropriate dimensions (16 columns, 2 rows)
    lcd.init();
    lcd.backlight();

    // Comment this section later; first upload without comments (for RTC setup),
    // second time upload with comments for Arduino
    // rtc.setDOW(THURSDAY);      // Name of the day of the week (follow your current day)
    // rtc.setTime(10, 02, 00);  // Hour, Min, Sec (follow your current time)
    // rtc.setDate(06, 05, 2021); // Day, Month, Year (follow your current year)
}

void loop()
{
    RtcDateTime now = Rtc.GetDateTime();
    printDateTime(now);
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
