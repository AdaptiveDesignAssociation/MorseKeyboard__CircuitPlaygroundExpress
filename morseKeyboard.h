class KeyboardKey {
    int keyPin;      // the number of the button pin
    char modifierValue;
    char keyValue;
    char letterValue;
    char altLetterValue;
    int pressSpeed;
    int debounceDelay;
    int freq;
    int sndLength;
    int sndPin;
    bool buttonTriggerState;

    bool keyFirstPress = true;

    unsigned long lastDebounceTime = 0;

  public:
    KeyboardKey (int pin, char primaryKeyValue, char secondaryKeyValue, int debouncerTime, int speakerPin, int freqValue, int sndDuration, bool buttonValueState)
    {
      keyPin = pin;
      letterValue = primaryKeyValue;
      altLetterValue = secondaryKeyValue;
      keyValue = letterValue;
      debounceDelay = debouncerTime;
      freq = freqValue;
      sndLength = sndDuration;
      sndPin = speakerPin;
      buttonTriggerState = buttonValueState;
      int resistorType;
      
      if (buttonTriggerState == LOW){
        resistorType = INPUT_PULLUP;
      }
      else{
        resistorType = INPUT_PULLDOWN;
      }
      
      pinMode(keyPin, resistorType);
    }

    void Press(boolean isShiftPressed, int ditdahTimer)
    {
      int keyState = digitalRead(keyPin);
      bool shiftCheck = isShiftPressed;
      int playLength = sndLength - pressSpeed;
      pressSpeed = ditdahTimer;

      if (isShiftPressed) {
        keyValue = altLetterValue;
      }
      else {
        keyValue = letterValue;
      }

      if (keyState == buttonTriggerState) {
        if (millis() - lastDebounceTime > debounceDelay) {
          if (keyFirstPress == true) {
            tone(sndPin, freq, playLength / 3);
            Keyboard.press(keyValue);
            Keyboard.release(keyValue);
            keyFirstPress = false;
            lastDebounceTime = millis();
          }
          while (digitalRead(keyPin) == buttonTriggerState && millis() - lastDebounceTime > debounceDelay) {
            if (millis() - lastDebounceTime > pressSpeed) {
              tone(sndPin, freq, playLength / 3);
              Keyboard.press(keyValue);
              Keyboard.release(keyValue);
              lastDebounceTime = millis();
            }
          }
          lastDebounceTime = millis();
        }
      }
      else {
        keyFirstPress = true;
      }
    }
};

class AccessKey {
    int keyPin;      // the number of the button pin
    char keyValue;
    int debounceDelay;
    bool buttonTriggerState;
    
    unsigned long lastDebounceTime = 0;

    bool lastButtonState = HIGH;
    bool buttonState;

    bool modifierState = false;

  public:
    AccessKey (int pin, int debouncerTime, bool buttonStateValue)
    {
      keyPin = pin;
      debounceDelay = debouncerTime;
      buttonTriggerState = buttonStateValue;
      
      int resistorValue;
      
      if (buttonTriggerState == LOW){
        resistorValue = INPUT_PULLUP;
      }
      else{
        resistorValue = INPUT_PULLDOWN;
      }
      pinMode(keyPin, INPUT_PULLDOWN);
    }

    void Press()
    {
      int keyState = digitalRead(keyPin);

      if (keyState == buttonTriggerState) {
        if (millis() - lastDebounceTime > debounceDelay) {
          Keyboard.press(131);
          Keyboard.press(176);
          Keyboard.releaseAll();
          Keyboard.press(131);
          Keyboard.press(176);
          Keyboard.releaseAll();
          Keyboard.press(131);
          Keyboard.press(176);
          Keyboard.releaseAll();
          lastDebounceTime = millis();
        }
        lastDebounceTime = millis();
      }
    }

    boolean Check()
    {
      int keyState = digitalRead(keyPin);

      if (keyState != lastButtonState) {
        lastDebounceTime = millis();
      }

      if ((millis() - lastDebounceTime) > debounceDelay) {
        if (keyState != buttonState) {
          buttonState = keyState;

          if (buttonState == HIGH) {
            modifierState = !modifierState;
            Keyboard.press(131);
            Keyboard.press(176);
            Keyboard.releaseAll();
            Keyboard.press(131);
            Keyboard.press(176);
            Keyboard.releaseAll();
            Keyboard.press(131);
            Keyboard.press(176);
            Keyboard.releaseAll();
          } 
        }
      }
      lastButtonState = keyState;
      return modifierState;
    }
};
