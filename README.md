# Morse Keyboard with the Circuit Playground Express

Utilize the native HID support of the Circuit Playground Express to create a custom interface for entering Morse code letters into the Google's GBoard Morse Code virtual keyboard.

Morse code has a rich history of use for communication.  Within the world of assistive technology, morse code can serve as an alternative input mechanism into computers and Alternative and Augmentative Communiction devices, reducing the need for fine motor movement across an array of keyboard keys.  With the ability to activate just one switch with an arm, a foot, a head, a tongue, etc. a person can have full access to a computer.

Tania Finlayson, a developer, details the opportunities having a morse system provided in her own life (http://www.tandemmaster.org/background.html).  With her understanding of morse, Tania has worked with her husband, Ken, and others to create the TandemMaster -- a morse code, USB input device.  Recently, in partnership with some friends at Google, Tania and Ken contributed to the development of the morse code GBoard.

Tania and Ken have also partnered with the Adaptive Design Assocation (http://www.adaptivedesign.org/), to explore other possibilities morse offers in terms of communication, literacy, and play for children (and adults).  The items contained within this repository include some of the initial work at encouraging others -- from developers to children -- to create custom inputs for themselves and others with morse code into tablets and phones through a hardware bridge.

This repository offers an Arduino class structure for working with some of the different features on both the Circuit Playground Express and the Morse GBoard like the ability to setup keyrepeats (so you can dwell on a switch for multiple inputs of a short or long) and use of the Circuit Playground's on board piezo for auditory feedback.  Connecting with external inputs, like our favorite micro switches, with alligator clips allows for quick prototyping of different accessible, custom built switches that build off a person's abilities.

The Arduino header file also includes a class and method for triggering the row scan and select features on some Android devices.  That is detailed here using an Arduino Leonardo: https://www.instructables.com/id/Gboard-Input-Switch-for-Morse-Code/.  The code is left here to provide for that funcationality.

## Hardware Components
For this tutorial, you will need:
- 1 x Circuit Playground Express: https://www.adafruit.com/product/3333
- 1 x USB a-micro (for programming and plugging into a tablet or smartphone): https://www.adafruit.com/product/2185
- 1 x USB-OTG Cable (for plugging into a tablet of smartphone): https://www.adafruit.com/product/1099
- 1 x pack of alligator clips: https://www.adafruit.com/product/1008
- 2 x micro-switches: https://www.adafruit.com/product/819

The project also compliments Dave Astels project documented here on the Adafruit website: https://learn.adafruit.com/android-gboard-morse-code-at-with-circuitplayground-express/overview.

## Downloading Software
First, you will need to insall GBoard and download the English/Morse language: https://play.google.com/store/apps/details?id=com.google.android.inputmethod.latin&hl=en_US, and configure it: https://support.google.com/accessibility/android/answer/9011881?hl=en

Dave's post also goes into some more detail on how to configure the GBoard settings and attach it to your phone: https://learn.adafruit.com/android-gboard-morse-code-at-with-circuitplayground-express/installing-gboard

You will also need the Arduino IDE and set it up to be able to program the Circuit Playground Express: https://learn.adafruit.com/adafruit-circuit-playground-express/arduino

## Looking at the Code
Next, you can download or clone this repository by clicking on the green "Clone or download button".  Unzip the folder, rename the file MorseKeyboard_CircuitPlaygroundExpress and move it to your Arduino sketches folder.

Now let's open the sketch MorseKeyboard_CircuitPlaygroundExpress in the Arduino IDE.  The sketch should include the MorseKeyboard_CircuitPlaygroundExpress.ino and the morseKeyboard.h header file.  Let's look a bit more at the .ino file.

First, we'll include both the Arduino keyboard library followed by our header file.  This gives us access to keyboard emmulation and also the morse class structures.

```C++
#include<Keyboard.h> // library for keyboard inputs
#include "morseKeyboard.h" // load class header file
```

Next, we'll create a bunch of variables to adjust various features of our morse interface.

```C++
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
```

The variables hold different information about which pins we'll be using on the Circuit Playground Express, frequencies and durations for the different short (dit) and long (dah) sounds, speed for repeating an entry and an option for controlling that with a potentiometer (not done here).  One item of note is the `bool activeButtonValue` variable can be LOW or HIGH -- based off what signal the attached switch will listen for.  It also sets the appropriate internal resistors for either an external or onboard button.

We'll then create some variables to handle the keyboard values being sent to the tablet or phone.

```C++
char ditKey = 46; // short value (period)
char dahKey = 45; // long value (hyphon)
char altDitKey = 204; //alt key for shorts, that can be tied to controlling row scanning
char altDahKey = 205; // alt key for longs, that can be tied to controlling row scanning
```

The `char altDitKey` and `char altDahKey` are included but not utilized.  These are the keycodes the board would output if using the accessiblity key.  We instead use the keycodes for period and hypon that work with the shorts and longs of the Gboard.

We will now initialize our class with the following pieces of code.

```C++
// Arudino class that prepares the short key with variables above (see morseKeyboard.h)
KeyboardKey shortKey(ditPin, ditKey, altDitKey, debounceTime, buzzerPin, ditSound, ditSoundDuration, activeButtonValue);

// Arudino class that prepares the long key with variables above (see morseKeyboard.h)
KeyboardKey longKey(dahPin, dahKey, altDahKey, debounceTime, buzzerPin, dahSound, dahSoundDuration, activeButtonValue);

// Optional third button to toggle on and off accessiblity settings like row scanning (tested on Samsung)
//AccessKey accessButton(accessPin, debounceTime, activeButtonValue);
```
We'll create our short input key and long input key.  Since we are not using the other accessibility features on the tablet, will leave the "AccessKey" commented out.

We will then populate the setup() function in the code.

```C++
void setup() {
  // put your setup code here, to run once:
  // turn on Serial monitor for debugging
  Serial.begin(9600);

  // start the keyboard
  Keyboard.begin();
}
```
`Serial.being(9600)` is included for debugging purposes, and can be removed.  `Keyboard.begin();` starts the keyboard library.

Finally, in our loop() function we'll call methods from the class.

```C++
void loop() {

  //bool shiftValue = bleAccessButton.Check(); // used if using third key
    bool shiftValue = false; // set as a default to false, meaning don't use alternate values
    
  shortKey.Press(shiftValue, speedTyper);
  longKey.Press(shiftValue, speedTyper);

  //if you don't want key repeats (so you have to release and press to enter the character again, comment out the above and uncomment the below
  //shortKey.PressNoRepeat();
  //longKey.PressNoRepeat();
}
```
The `bool shiftValue` refers again to the optional third button to enter into the accessibility features of the Android device.  We are not using that so set the value to false.

`shortKey.Press(shiftValue, speedTyper);` and `longKey.Press(shiftValue, speedTyper);` make calls to the method contained in the header file to debounce the switch and send the appopriate value at appropriate intervals if being held down.

The wiring used in this setup is provided in the embedded Fritzing diagram.  The micro switches are connected through alligator clips to the board at the correct pin and to ground because the board is looking for a low signal.

![The circular Circuit Playground Express has 1 aligator clip connected to A3, 1 alligator clip connected A2 and the ends of each respective clip connected to different micro switch. Each switch also has a alligator clip going from to ground on the Circuit Playground](https://github.com/AdaptiveDesignAssociation/MorseKeyboard__CircuitPlaygroundExpress/blob/master/Morse_Interface_Diagram.png "Circuit Playground Express Morse Interface")

After loading up the sketch you can use the USB-OTG cablet to plug it into your phone and access the morse keyboard.
