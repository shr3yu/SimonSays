#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int pin1, pin2; //Used to map LEDS through Pin connections
int maxlevel = 6;
int maxseconds = 4000; 
int secondperlabel = 4000/6;

/*
Basic outline of game:

- There are 7 levels, it goes faster each time, and adds one more light it shows
- User has unlimited time to press after the sequence has been shown

*/

// Initialize the LCD with I2C address 0x27 (common for many LCDs), and set the SDA and SCL pins
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change the address to match your LCD if needed

// NOTE: path corresponds to the GPIO pins attached to the LED
int pins[3] = {17,16,4}; // 17: PATH 0, 16: PATH 1, 4: PATH 2

// 'Start' and 'end' pin attachments to LED
int start[6]= {0,0,2,1,1,2};
int end[6]={1,2,0,2,0,1};


void reset() {
  for (int i = 0; i < 3; i++) {
    pinMode(pins[i], OUTPUT);  // Set all pins as OUTPUT initially
    digitalWrite(pins[i], LOW);  // Ensure all LEDs are off initially
  }
}

// Flashes LED 
void flash (int startpath, int endpath){
  for (int i = 0; i< 3; i++){
    if (i == startpath){
      digitalWrite(pins[startpath], HIGH);
    }
    if (i == endpath){
      digitalWrite(pins[endpath], LOW);
    }
    if (i != startpath && i!= endpath){
      pinMode(pins[i], INPUT);
    }
  }
}

// Generates two random numbers (that correspond to GPIO pins, that is then mapped to LED)
void generaterandompins(){
  //Generate two random numbers (they can not be the same)
  pin1 = random(0,3);
  pin2 = pin1;
  while (pin2== pin1){
    pin2= random(0,3);
  }

}

void printpins(){
  // Print both pin values
  Serial.print("Pin 1: ");
  Serial.print(pin1);   // Print value of pin1
  Serial.print(", Pin 2: ");
  Serial.println(pin2); // Print value of pin2 with a newline at the end
}

// Uses Analog pin to determine corresponding button pressed 
int waitforcorrectbutton(int expectedPin){
  expectedPin++;
  //you can take however long you want, as long as the buttons are pressed in the correct order

  while (true){
    int rawValue = analogRead(32); // Read the analog pin
    float voltage = rawValue * (3.3 / 4095.0);// Convert to voltage

    // Map voltage ranges to specific buttons
    if (voltage > 0.02 && voltage < 0.1) {
      Serial.print("Voltage: ");
      Serial.println(voltage,3); // Print the voltage
      if (expectedPin == 1) {
        return true; // Correct button pressed
      } else {
        return false; // Wrong button pressed
      }
    } else if (voltage > 0.1 && voltage < 0.3) {
      Serial.print("Voltage: ");
      Serial.println(voltage,3); // Print the voltage
      if (expectedPin == 2) {
        return true;
      } else {
        return false;
      }
    } else if (voltage > 0.3 && voltage < 0.8) {
      Serial.print("Voltage: ");
      Serial.println(voltage,3); // Print the voltage
      if (expectedPin == 5) {
        return true;
      } else {
        return false;
      }
    } else if (voltage > 0.8 && voltage < 1.2) {
      Serial.print("Voltage: ");
      Serial.println(voltage,3); // Print the voltage
      if (expectedPin == 3) {
        return true;
      } else {
        return false;
      }
    } else if (voltage > 1.2 && voltage < 1.8) {
      Serial.print("Voltage: ");
      Serial.println(voltage,3); // Print the voltage
      if (expectedPin == 6) {
        return true;
      } else {
        return false;
      }
    } else if (voltage > 1.8) {
      Serial.print("Voltage: ");
      Serial.println(voltage,3); // Print the voltage
      if (expectedPin == 4) {
        return true;
      } else {
        return false;
      }
    }
    delay(10);
    
  }
}

// Based on the GPIO pins, determine the LED flashed
int determineLEDflashed(int pin1, int pin2){
  for (int i = 0; i < 6; i++){
    if (start[i] == pin1 && end[i]== pin2){
      return i;      
    }
  }
}

void printerror(){
  lcd.clear();  // Clear the LCD 
  lcd.print("YOU LOST!");  // Display level on LCD for the user
}

void printwinmessage(){
  lcd.clear();
  lcd.print("YOU WON!");
}

void setup() {
  // Start I2C communication
  Wire.begin(21, 22);  // SDA -> GPIO21, SCL -> GPIO22
  
  // Initialize the LCD
  lcd.backlight();
  lcd.begin(16, 2);    // 16 columns and 2 rows
  lcd.print("Let's play!");  // Display message on LCD
  delay(3000);

  Serial.begin(9600);
  reset();
}

void loop() {
  int level =1 ; //keeps track of the level,starts at level one
  int sequenceLength = 3; // number of consecutive lights per level, increases by 1 each time
  

  while (level < maxlevel){
    lcd.clear();  // Clear the LCD for each level
    lcd.print("Level ");  // Display level on LCD for the user
    lcd.print(level);

    Serial.print("Level");
    Serial.println(level);

    delay(4000); // Delay before starting the game
    Serial.print(level);
    int leveldelay = maxseconds - (level*secondperlabel);
    int ledsFlashed[10];

    for (int i = 0; i < sequenceLength; i++){
      lcd.clear();  // Clear the LCD for each level
      lcd.print("Simon says... "); 

      //generate a new number
      generaterandompins();
      printpins();
      flash(pin1, pin2);
      //based on the pins, map to LED, and add to list of LED flashed
      ledsFlashed[i] = determineLEDflashed(pin1,pin2);

      Serial.print("LED flashed ");
      Serial.println(ledsFlashed[i]);
      
      delay(leveldelay); // falsh the light for the generated time

      reset(); //reset IO pins
      //delay between each light is 1 second
      delay(250);
    }

    //we poll for the pressed button
    for (int i = 0; i < sequenceLength; i++){
      lcd.clear();  // Clear the LCD for each level
      lcd.print("Copy Simon! "); 
      bool correctbutton;
      Serial.print("Checking if this LED is pressed:  ");
      Serial.println(ledsFlashed[i]);
      correctbutton = waitforcorrectbutton(ledsFlashed[i]);
      Serial.print("Status: ");
      Serial.println(correctbutton);
      
      
      if (!correctbutton){
        //print error and restart
        printerror();
        delay(7000);
        return;
      }
      delay(250);
    }

    sequenceLength++;
    level++;
  }

  // At this point user made it through all levels
  printwinmessage();
  delay(7000);
}

