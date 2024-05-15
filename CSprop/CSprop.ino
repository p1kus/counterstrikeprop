#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4; // Define the number of rows on the keypad
const byte COLS = 3; // Define the number of columns on the keypad
char keys[ROWS][COLS] = { // Matrix defining character to return for each key
  {'1','2','3'},
  {'7','8','9'},
  {'4','5','6'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pins (R0-R3) of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pins (C0-C2) of the keypad
//initialize an instance of class
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16 column and 2 rows


const int LED_PIN = 9;
const int BUZZER_PIN = 10;

String setCode = "7355608";
String inputCode = "";
int cursorColumn = 0;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  initMode();
}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.getKey();


  if (key) {
    inputCode += key;
    Serial.println(inputCode);
    lcd.setCursor(cursorColumn, 0); // move cursor to   (cursorColumn, 0)
    lcd.print(key);                 // print key at (cursorColumn, 0)

    cursorColumn++;                 // move cursor to next position
    if(cursorColumn == 11) {        // At the last input, change message
    lcd.setCursor(1, 1); // move cursor to   (cursorColumn, 0)
    lcd.print("Zatwierdz -> #"); // Zatwierdzenie działa niezaleznie co zostanie wcisniete, linijka nizej. 
    }
    if(cursorColumn == 12) {        // if reaching limit, clear LCD
      lcd.clear();
      cursorColumn = 0;
      armedMode(inputCode);
    }
  }
  if (key == '*') {     
    reset();           
  }
  if (key == '#') {
    armedMode(inputCode);             
    
  }

}


void initMode() {
  cursorColumn = 4;
  lcd.clear();
  lcd.setCursor(cursorColumn, 0);
  lcd.print("*******");
}

void inputMode() {

}

void reset() {
    lcd.clear();
    lcd.setCursor(4, 1); // move cursor to   (cursorColumn, 0)
    lcd.print("Reset"); 
    delay(1000);
    initMode();

}

void validateInput(String input) {
  if(input == inputCode) {
    defuse();
  }
}

void armedMode(String input) {
    input = inputCode;
    inputCode.remove(7);
    lcd.clear();
    lcd.setCursor(1, 1); // move cursor to   (cursorColumn, 0)
    lcd.print("Code:" + input); 

}

void defuse() {

}
