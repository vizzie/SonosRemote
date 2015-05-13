 /*
 PlayFavoriteStations
 Ed Guild, ed@vizzie.com
 Control a Sonos unit via five favorite buttons, volume control, and pause/play button

 */

#include <Process.h>
#include <string.h>
#include <Wire.h>
#include <LiquidTWI.h>

// Connect via i2c, default address #0 (A0-A2 not jumpered)
LiquidTWI lcd(0);

// Hardware interface
const int pinCount = 9;
// set pin numbers:
// LEDs and Buttons line up in the arrays (e.g. buttonPins[0] works with ledPins[0])
int buttonPins[pinCount] = {A0, A1, A2, A3, A4, A5, 7, 8, 4};    // the number of the pushbutton pin
int ledPins[pinCount] = {5, 6, 9, 10, 11, 13, 12, 12, 12};      // the number of the LED pin
boolean outputIsPWM[pinCount] = {true, true, true, true, true, true, false, false, false}; // whether output is PWM or totally digital

// LED Model
// LED State and Mode (Mode determines how the LED will display)
int ledBrightness[pinCount] = {255, 255, 255, 255, 255, 255, 255, 255, 255};
char ledMode[pinCount] = {'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o'};

// LCD Model
const int lcdColumns = 16;
const int lcdRows = 2;
char lcdState[lcdColumns * lcdRows] = {"Sonos Remote v1\rBooting..."};

//Button Model
int pressedButton = -1;
int lastPressedButton = -1;
int debounceCount = 0;
int sleepCountdown = 2000;

//Process Model
Process activeProcess;

int volume = -1;

void setup() {
  // set up the LCD's number of rows and columns: 
  lcd.begin(lcdColumns, lcdRows);
  lcd.setBacklight(HIGH);
  lcdPrint(lcdState);
  
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
    
  // Initialize Bridge
  Bridge.begin();

// Initialize Serial
  //Serial.begin(9600);
//
//  // Wait until a Serial Monitor is connected.
//  while (!Serial);
  //Serial.println(lcdState);
  char lcdState[] = {"Sonos Remote v1\rReady...       "};
  //Serial.println(lcdState);
  lcdPrint(lcdState);
}//end of setup

void loop() {
  //Serial.println("--------loop start---------");
  if (volume == -1) {
    volume = getSonosVolume();
  }
  if (activeProcess.running() == false) {
    //Serial.println("process not running");
    //Serial.println("pressed button is: " + String(pressedButton));
    dimButtonLed(lastPressedButton); //dim any previously pressed button; this probably isn't the right spot for this
    lastPressedButton = -1;
    if (printReturnedText()) lcdPrint(lcdState);
  }// end if active process is not running; do I need to do anything while process is running?
  pressedButton = scanButtons(buttonPins, pinCount); //find a newly pressed button
  if (pressedButton != -1 && debounceCount == 0) {
    //Serial.println("button not -1: " + String(pressedButton));
    lastPressedButton = pressedButton;
    debounceCount = 25;
    sleepCountdown = 2000;
    parseButtonPress(pressedButton);
  }
  //Serial.println("just pressed " + String(pressedButton));
  debounceCount = max(0, (debounceCount - 1));
  sleepCountdown = max(0, (sleepCountdown - 1));
  if (sleepCountdown == 0) {
    lcd.setBacklight(LOW);
    turnOffLeds();
  } else {
    lcd.setBacklight(HIGH);
  }
  //Serial.println("debounceCount: " + String(debounceCount));
  updateLedBrightness();
  updateLedPins();
  //delay(5);
}// end Loop()

//sub functions
//lcdPrint
void lcdPrint(char* inputText) {
  int col = 0;
  int row = 0;
  lcd.clear();
  for (int lcdPosition = 0; lcdPosition < (lcdRows * lcdColumns); lcdPosition++) {
    lcd.setCursor(col, row);
    if (inputText[lcdPosition] != '\r' && col < lcdColumns) {
      lcd.print(filterAscii(inputText[lcdPosition]));
    } else {
      row ++;
      col = -1;
    }
    delay(20);
    col++;
  }// end for each row
}// end lcdPrint

char filterAscii(char inputChar) {
  if (inputChar > 0 && inputChar < 128) {
    return inputChar;
  } else {
    return ' ';
  }
}//end filterAscii

boolean printReturnedText() {
  if (activeProcess.available() > 0) {
    int remainingChars = lcdColumns * lcdRows;
    int c = 0;
    while (remainingChars > 0) {
      if (activeProcess.available() != 0) {
        lcdState[c] = char(activeProcess.read());
      } else {
        lcdState[c] = char(' ');
      }
      remainingChars --;
      c ++;
    }
    return true;
  } else {
    return false;
  }//end if process has returned data
} // end saveReturnedText

// scanButtons will seek through the buttonPins and see if any are pressed. 
// It will return only the first button it finds
int scanButtons (int *buttonPins, int buttonLength) {
  //Serial.println("------");
  for (int i = 0; i < buttonLength; i++) {
    //Serial.println("scanning button " + String(buttonPins[i]));
    if (digitalRead(buttonPins[i]) == LOW) {
      //Serial.println("pressed button " + String(i));
      return i;
    }
  }//end for each buttonPins
  return -1; //none of the buttons were pressed
}//end scanButtons

void parseButtonPress (int buttonNumber) {
  switch (buttonNumber) {
    case 0:
    runFavStation(buttonNumber);
    break;
    case 1:
    runFavStation(buttonNumber);
    break;
    case 2:
    runFavStation(buttonNumber);
    break;
    case 3:
    runFavStation(buttonNumber);
    break;
    case 4:
    runFavStation(buttonNumber);
    break;
    case 5:
    runFavStation(buttonNumber);
    break;
    case 6: //volume up
    //Serial.println("vol up");
    upVolume(buttonNumber);
    break;
    case 7: //volume down
    //Serial.println("vol down");
    downVolume(buttonNumber);
    break;
    case 8: //pause/play
    pausePlay(buttonNumber);
    break;   
  }//end switch buttonNumber
}// end parseButtonPress

void runFavStation(int favoriteStation) {
  int ledPin = favoriteStation;
  activeProcess.begin("python");	// Process that launch the "python" command
  activeProcess.addParameter("/mnt/sda1/PlayFavoriteStation.py"); // Add the local python file location
  activeProcess.addParameter(String(favoriteStation)); // which favorite station to play
  activeProcess.runAsynchronously();		// Run the process and go do other stuff
  turnOffLeds();
  ledMode[ledPin] = 'b';
}// end runFavStation

void pausePlay(int ledPin) {
  activeProcess.begin("python");	// Process that launch the "python" command
  activeProcess.addParameter("/mnt/sda1/PausePlayToggle.py"); // Add the local python file location
  activeProcess.runAsynchronously();		// Run the process and go do other stuff
  ledMode[ledPin] = 'B';
}//end runFavStation

void upVolume(int ledPin) {
  volume = min((volume + 3), 100);
  //Serial.println("setting volume to " + String(volume));
  activeProcess.begin("python");	// Process that launch the "python" command
  activeProcess.addParameter("/mnt/sda1/VolumeControl.py"); // Add the local python file location
  activeProcess.addParameter(String(volume)); // which favorite station to play
  activeProcess.runAsynchronously();		// Run the process and go do other stuff
  ledMode[ledPin] = 'b';
}//end upVolume

void downVolume(int ledPin) {
  volume = max((volume - 3), 0);
  //Serial.println("setting volume to " + String(volume));
  activeProcess.begin("python");	// Process that launch the "python" command
  activeProcess.addParameter("/mnt/sda1/VolumeControl.py"); // Add the local python file location
  activeProcess.addParameter(String(volume)); // which favorite station to play
  activeProcess.runAsynchronously();		// Run the process and go do other stuff
  ledMode[ledPin] = 'b';
}//end downVolume

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
      analogWrite(ledPins[i], ledBrightness[i]);
    } else {
      digitalWrite(ledPins[i], brightToDigital(ledBrightness[i]));
    }
  }//end for pinCount
} //end updateLedPins

int brightToDigital(int brightLevel) {
  if (brightLevel < 100) {
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
      ledBrightness[i] = min(255, ledBrightness[i] + 2);
      if (ledBrightness[i] >= 255) {
        ledMode[i] = 'B'; //switch to down direction
      }
      //Serial.println("blink up: " + String(ledBrightness[i]));
      break;
      case 'B': //blink down
      ledBrightness[i] = max(0, ledBrightness[i] - 2);
      if (ledBrightness[i] <= 0) {
        ledMode[i] = 'b'; //switch to up direction
      }
      //Serial.println("blink down: " + String(ledBrightness[i]));
      break;
    }//end switch
  }//end for pinCount
} //end updateLeds

/////// old sub functions

//currently gets second device
//this funciton needs to allow user to select at some point
String getDeviceIP() {
  String deviceString;
  // Launch "python" command
  Process p;		// Create a process and call it "p"
  p.begin("python");	// Process that launch the "python" command
  p.addParameter("/mnt/sda1/GetDevices.py"); // Add the local python file location
  p.run();
  while (p.available()>0) {
    char c = p.read();
    //Serial.print(c);
  }

}//end getDeviceIP

int getSonosVolume() {
  // Launch "python" command
  activeProcess.begin("python");	// Process that launch the "python" command
  activeProcess.addParameter("/mnt/sda1/GetVolume.py"); // Add the local python file location
  activeProcess.run();
  String c;
  while (activeProcess.available()>0) {
    c = c + char(activeProcess.read());
  }
  //Serial.println("Sonos volume: " + c);
  return c.toInt();
} //end getSonosVolume
