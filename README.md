# counterstrikeprop
Counter Strike inspired bomb prop.

Use at your own risk!

#**List of parts used:**
- Arduino Nano
- Standard 2x16 I2C Display
- 4x3 Standard Keypad
- Buzzer
- Red LED Diode
- Development Board

#**Pin SETUP**

Make sure you connect all your parts to correct pins, or customize the configuration for your own setup.

for LED:

`const int LED_PIN = 9;`

for Buzzer:

`const int BUZZER_PIN = 10;`

for Keypad:

`const byte ROWS = 4; // Define the number of rows on the keypad
const byte COLS = 3; // Define the number of columns on the keypad`

`byte rowPins[ROWS] = {8, 6, 7, 5}; //connect to the row pins (R0-R3) of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pins (C0-C2) of the keypad`

