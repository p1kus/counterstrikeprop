#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <CountDown.h>

const byte ROWS = 4;       // Define the number of rows on the keypad
const byte COLS = 3;       // Define the number of columns on the keypad
char keys[ROWS][COLS] = {  // Matrix defining character to return for each key
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte rowPins[ROWS] = { 8, 6, 7, 5 };  //connect to the row pins (R0-R3) of the keypad
byte colPins[COLS] = { 4, 3, 2 };     //connect to the column pins (C0-C2) of the keypad
//initialize an instance of class
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16 column and 2 rows
CountDown timer;


const int LED_PIN = 9;
const int BUZZER_PIN = 10;

String setCode = "7355608";
String inputCode = "";
String defuseCode = "";
int cursorColumn = 0;


void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  initMode();
}

bool isArmedMode = false;

void loop() {
  char key = keypad.getKey();
  if (key) {
    digitalWrite(LED_PIN, HIGH);
    delay(10);
    digitalWrite(LED_PIN, LOW);
    inputCode += key;  // create a string out of entered inputs
    Serial.println(inputCode);
    lcd.setCursor(cursorColumn, 0);  // move cursor to   (cursorColumn, 0)
    lcd.print(key);                  // print key at (cursorColumn, 0)

    cursorColumn++;             // move cursor to next position
    if (cursorColumn == 11) {   // At the last input, change message
      lcd.setCursor(3, 1);      // move cursor to   (cursorColumn, 0)
      lcd.print("Enter -> #");  // At the last position (cursorColumn == 11) it enters no matter what button next you press, code is reduced to 7 characters regardless.
    }
    if (cursorColumn == 12 && !isArmedMode) {  // if reaching limit, clear LCD and enter code
      Serial.println("First time entered code");
      timer.start(31000);
      lcd.clear();
      cursorColumn = 4;
      armedMode();
      isArmedMode = true;
    }
    if (isArmedMode) {
      defuseCode += key;
      Serial.println("Defuse:" + defuseCode);
    }
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  armedTimer();
  if (key == '*') {
    reset();
  }
  // if (key == '#') {
  //   // lcd.setCursor(4, 0);
  //   // inputCode.remove(0);
  //   // defuseCode = inputCode;
  //   // isArmedMode = true;
  // }
  if (cursorColumn == 12 && isArmedMode) {
    defuseCode = defuseCode.substring(1);
    defuseCode.remove(7);
    lcd.clear();
    isArmedMode = false;
    validateInput(defuseCode);
  }
}


bool isTimerExpired = false;
bool defused = false;
bool buzzerActive = false;

void armedTimer() {
  unsigned long currentTime = millis();
  static unsigned long lastUpdateTime = currentTime;
  if (currentTime - lastUpdateTime >= 1000) {
    lastUpdateTime = currentTime;
    uint32_t remainingTime = timer.remaining() / 1000;
    if (remainingTime > 0 && !defused) {
         if (remainingTime <= 30) {
        buzzerTickFaster(remainingTime);
      } else {
      }
      uint32_t timeInSeconds = remainingTime;
      lcd.setCursor(7, 1);
      if (timeInSeconds < 10) {
        buzzerActive = false;
        lcd.print("0");  // Add leading zero if necessary
        isTimerExpired = true;
      }
      lcd.print(timeInSeconds);
    }
    // else if(remainingTime > 0 && defused) {
    //   defused = false;
    //   lcd.clear();
    //   initMode();
    // }
    if (remainingTime == 0 && isTimerExpired) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("*BOMB EXPLODED*");
      lcd.setCursor(0, 1);
      lcd.print("----*******----");
      delay(5000);
      isTimerExpired = false;
      initMode();
    }
  }
}


void buzzerTickFaster(int remainingTime) {
  if (remainingTime > 0) {
    // Adjust duration based on logarithm of remaining time
    int duration = map(log(remainingTime + 1), log(31), log(1), 100, 10);
    
    tone(BUZZER_PIN, 1500, duration); // Constant frequency of 1500 Hz
    delay(duration * 2); // Adjust delay based on duration
    noTone(BUZZER_PIN);
  }
}






void reset() {
  inputCode = "";
  lcd.clear();
  lcd.setCursor(4, 1);
  lcd.print("Reset");
  delay(1000);
  isArmedMode = false;
  initMode();
}

void validateInput(String input) {
  if (input == setCode) {
    Serial.println("Validation true");
    Serial.println(input);
    inputCode = "";
    timer.stop();
    lcd.setCursor(2, 1);
    defused = true;
    tone(BUZZER_PIN, 2798);
    delay(100);
    noTone(BUZZER_PIN);
    lcd.print("Bomb defused!");
  } else {
    Serial.println(input);
    Serial.println("Validation false");
    inputCode = "";
    timer.stop();
    defused = true;
    lcd.setCursor(0, 0);
    lcd.print("*BOMB EXPLODED*");
    lcd.setCursor(0, 1);
    lcd.print("----*******----");
  }
}

void armedMode() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("*******");
}


void initMode() {
  cursorColumn = 4;
  lcd.clear();
  lcd.setCursor(cursorColumn, 0);
  lcd.print("*******");
  lcd.setCursor(3, 1);
  lcd.print("Enter code");
}
