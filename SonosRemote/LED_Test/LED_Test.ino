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
char ledMode[pinCount] = {'b', 'd', 'b', 'b', 'd', 'b', 'b', 'b', 'b'};

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
  
  // set initial LED state
  updateLedPins();

// Initialize Serial
  Serial.begin(9600);
//
//  // Wait until a Serial Monitor is connected.
  while (!Serial);

}//end of setup

void loop() {
  // put your main code here, to run repeatedly:
  updateLedBrightness();
  updateLedPins();
delay(5);
}//end loop

//sub functions

// scanButtons will seek through the buttonPins and see if any are pressed. 
// It will return only the first button it finds
int scanButtons (int *buttonPins, int buttonLength) {
  for (int i = 0; i < buttonLength; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      return i;
    }
  }//end for each buttonPins
  return -1; //none of the buttons were pressed
}//end scanButtons

void turnOffLeds() {
  for (int i = 0; i < pinCount; i++) {
    ledMode[i] = 'o';
  } // end for each pin
}// end zeroLeds

void dimButtonLed(int button) {
  if (button != -1) {
    ledMode[button] = 'd';
  }//end if
}// end dimButtonLed


//apply ledBrightness to the actual led pin output
void updateLedPins() {
  for (int i = 0; i < pinCount; i++) {
    if (outputIsPWM[i]) {
      digitalWrite(ledPins[i], ledBrightness[i]);
    } else {
      digitalWrite(ledPins[i], brightToDigital(ledBrightness[i]));
    }
  }//end for pinCount
} //end updateLedPins

int brightToDigital(int brightLevel) {
  if (brightLevel < 128) {
    Serial.println(brightLevel);
    return LOW;
  } else {
    return HIGH;
  }
}// end brightToDigital

//Update Button LEDs
void updateLedBrightness() {
  for (int i = 0; i < pinCount; i++) {
    switch (ledMode[i]) {
      case 'o': //off
      ledBrightness[i] = max(0, ledBrightness[i] - 1);
      break;
      case 'd': //dim
      ledBrightness[i] = max(10, ledBrightness[i] - 1);
      break;
      case 'b': //blink up
      ledBrightness[i] = ledBrightness[i] + 1;
      if (ledBrightness[i] >= 255) {
        ledMode[i] = 'B'; //switch to down direction
      }
      break;
      case 'B': //blink down
      ledBrightness[i] = ledBrightness[i] - 2;
      if (ledBrightness[i] <= 0) {
        ledMode[i] = 'b'; //switch to up direction
      }
      break;
    }//end switch
  }//end for pinCount
} //end updateLeds

//int calcLedBlink(int brightness) {
//  float sinCalc = (((sin((blinkCounter/255) * 2 * PI)/2) + 0.5) * 255);
//  if (blinkCounter == 255) {
//    blinkCounter = 0;
//  }
//  blinkCounter += 5;
//  Serial.println(String(sinCalc) + "/" + String(blinkCounter));
//  return max(0, min(255, sinCalc));
//}//end calcLedBlink



