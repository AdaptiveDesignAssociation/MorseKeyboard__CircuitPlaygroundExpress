/****
Original logic and keyboard concept provided by Ken Finlayson of NottLimited
https://github.com/NottLimited/MorseCodeGboardSwitch

Class structure assembled by Jason Beck @ Adaptive Design Association
http://www.adaptivedesign.org/
*****/

#include<Keyboard.h> // library for keyboard inputs
#include "morseKeyboard.h" // load class header file

int buzzerPin = A0; // buzzer to provide auditory feedback to button presses
int debounceTime = 400; // eliminate noise on the buttons
int ditPin = A2; // press button on this pin to activate shorts (period)
int dahPin = A3; // press button on this pin to activate longs (hyphon)
int ditSound = 880; // frequency of the short beeps
int dahSound = 500; // frequency of the long beeps
int ditSoundDuration = 500; // length of shorts
int dahSoundDuration = 550; // length of longs
int speedTyper = 400; // time for key repeats, can be set with potentiometer
int speedSense = 3; // used when attaching a potentiometer
bool activeButtonValue = LOW; // whether you want the buttons to be triggered on low or high signal, also changes the internal resistors

char ditKey = 46; // short value (period)
char dahKey = 45; // long value (hyphon)
char altDitKey = 204; //alt key for shorts, that can be tied to controlling row scanning
char altDahKey = 205; // alt key for longs, that can be tied to controlling row scanning

// Arudino class that prepares the short key with variables above (see morseKeyboard.h)
KeyboardKey shortKey(ditPin, ditKey, altDitKey, debounceTime, buzzerPin, ditSound, ditSoundDuration, activeButtonValue);

// Arudino class that prepares the long key with variables above (see morseKeyboard.h)
KeyboardKey longKey(dahPin, dahKey, altDahKey, debounceTime, buzzerPin, dahSound, dahSoundDuration, activeButtonValue);

// Optional third button to toggle on and off accessiblity settings like row scanning (tested on Samsung)
//AccessKey accessButton(accessPin, debounceTime, activeButtonValue);

void setup() {
  // put your setup code here, to run once:
  // turn on Serial monitor for debugging
  Serial.begin(9600);

  // start the keyboard
  Keyboard.begin();
}

void loop() {

  //bool shiftValue = bleAccessButton.Check(); // used if using third key
    bool shiftValue = false; // set as a default to false, meaning don't use alternate values
    
  shortKey.Press(shiftValue, speedTyper);
  longKey.Press(shiftValue, speedTyper);
}
