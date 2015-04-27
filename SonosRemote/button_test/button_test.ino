// Hardware interface
const int pinCount = 9;
// set pin numbers:
// LEDs and Buttons line up in the arrays (e.g. buttonPins[0] works with ledPins[0])
int buttonPins[pinCount] = {A0, A1, A2, A3, A4, A5, 0, 1, 4};    // the number of the pushbutton pin
int ledPins[pinCount] = {5, 6, 9, 10, 11, 13, 8, 12, 7};      // the number of the LED pin
boolean outputIsPWM[pinCount] = {true, true, true, true, true, true, false, false, false}; // whether output is PWM or totally digital

// LED Model
// LED State and Mode (Mode determines how the LED will display)
int ledBrightness[pinCount] = {255, 255, 255, 255, 255, 255, 255, 255, 255};
char ledMode[pinCount] = {'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o'};

// LCD Model
const int lcdColumns = 16;
const int lcdRows = 2;
char lcdState[lcdColumns * lcdRows] = {'S','o','n','o','s',' ','R','e','m','o','t','e',' ','v','1','\r', 'B','o','o','t','i','n','g','.','.','.'};

//Button Model
int pressedButton = -1;
int lastPressedButton = -1;


void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPins[0], INPUT_PULLUP);
  pinMode(buttonPins[1], INPUT_PULLUP);
  pinMode(buttonPins[2], INPUT_PULLUP);
  pinMode(buttonPins[3], INPUT_PULLUP);
  pinMode(buttonPins[4], INPUT_PULLUP);
  pinMode(buttonPins[5], INPUT_PULLUP);
  pinMode(buttonPins[6], INPUT_PULLUP);
  pinMode(buttonPins[7], INPUT_PULLUP);
  pinMode(buttonPins[8], INPUT_PULLUP);
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(ledPins[3], OUTPUT);
  pinMode(ledPins[4], OUTPUT);
  pinMode(ledPins[5], OUTPUT);
  pinMode(ledPins[6], OUTPUT);
  pinMode(ledPins[7], OUTPUT);
  pinMode(ledPins[8], OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  pressedButton = scanButtons(buttonPins, pinCount); //find a newly pressed button
  Serial.println("pressed button " + String(pressedButton));
  delay(200);
}// end loop()


// scanButtons will seek through the buttonPins and see if any are pressed. 
// It will return only the first button it finds
int scanButtons (int *buttonPins, int buttonLength) {
  Serial.println("------");
  for (int i = 0; i < buttonLength; i++) {
    Serial.println("scanning button " + String(buttonPins[i]));
    if (digitalRead(buttonPins[i]) == LOW) {
      Serial.println("pressed button " + String(i));
      return i;
    }
  }//end for each buttonPins
  return -1; //none of the buttons were pressed
}//end scanButtons

