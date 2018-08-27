#include<Keyboard.h>
#include "morseKeyboard.h"

int buzzerPin = A0;
int debounceTime = 400; // prepare for noisy buttons
int ditPin = A2;
int dahPin = A3;
int ditSound = 880;
int dahSound = 500;
int ditSoundDuration = 500;
int dahSoundDuration = 550;
int speedTyper = 400; // time for key repeats
int speedSense = 3;
bool activeButtonValue = LOW;

char ditKey = 46;
char dahKey = 45;
char altDitKey = 204;
char altDahKey = 205;

KeyboardKey shortKey(ditPin, ditKey, altDitKey, debounceTime, buzzerPin, ditSound, ditSoundDuration, activeButtonValue);
KeyboardKey longKey(dahPin, dahKey, altDahKey, debounceTime, buzzerPin, dahSound, dahSoundDuration, activeButtonValue);
//AccessKey accessButton(accessPin, debounceTime, activeButtonValue);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Keyboard.begin();
}

void loop() {

  //bool shiftValue = bleAccessButton.Check();
    bool shiftValue = false;
    
  shortKey.Press(shiftValue, speedTyper);
  longKey.Press(shiftValue, speedTyper);
}
