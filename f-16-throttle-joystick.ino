#include <Keypad.h>
#include <Joystick.h>


// Joystick interface with PC
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
  18, 1,                  // Button Count, Hat Switch Count
  true, true, false,      // X, Y, Z Axis
  true, true, true,       // Rx, Ry, Rz
  false, true,            // rudder, throttle
  false, false, false);   // accelerator, brake, steering

// Hat buttons
#define HAT_BUTTON_PIN_COUNT 4
int hat_button_pins[HAT_BUTTON_PIN_COUNT] =      {13, 12, 11,  10};
int hat_button_angles_1d[HAT_BUTTON_PIN_COUNT] = {0,  90, 180, 270};
int hat_button_angles_2d[HAT_BUTTON_PIN_COUNT][HAT_BUTTON_PIN_COUNT] = {
  {0, 45, -1, 315},
  {45, 90, 135, -1},
  {-1, 135, 180, 225},
  {315, -1, 225, 270},
};

// Single pin buttons
#define SINGLE_PIN_BUTTON_COUNT 2
int single_pin_button_pins[SINGLE_PIN_BUTTON_COUNT] =    {8, 9};
int single_pin_button_numbers[SINGLE_PIN_BUTTON_COUNT] = {16, 17};  // That's the button number the PC will see

// Buttons matrix
#define MATRIX_ROWS 4 //four rows
#define MATRIX_COLS 4 //three columns
char keys[MATRIX_ROWS][MATRIX_COLS]; // A dummy array, we're gonna use kcode which gives 0..15 codes to the keys.
byte rowPins[MATRIX_ROWS] = {3, 2, 1, 0}; // row pins of the keypad
byte colPins[MATRIX_COLS] = {4, 5, 6, 7}; // column pins of the keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, MATRIX_ROWS, MATRIX_COLS);

void setup() {
  // Initialize single pin uttons
  for (int i=0; i<SINGLE_PIN_BUTTON_COUNT; ++i){
    pinMode(single_pin_button_pins[i], INPUT_PULLUP);
  }
  for (int i=0; i<HAT_BUTTON_PIN_COUNT; ++i){
    pinMode(hat_button_pins[i], INPUT_PULLUP);
  }

  // Initialize analog inputs
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);

  // Initialize Joystick Library
  Joystick.begin(false); //false = dont send automatically. We will sendState() at the end of the loop
  Joystick.setThrottleRange(-512, 512);
  Joystick.setXAxisRange(-512, 512);
  Joystick.setYAxisRange(-512, 512);
  Joystick.setRxAxisRange(-512, 512);
  Joystick.setRyAxisRange(-512, 512);
  Joystick.setRzAxisRange(-512, 512);
}

void loop() {
  // Update analog inputs state
  Joystick.setThrottle(map(analogRead(A4) , 0, 1023, -512, 512));
  Joystick.setXAxis(map(analogRead(A3) , 0, 1023, -512, 512)*-1);
  Joystick.setYAxis(map(analogRead(A2) , 0, 1023, -512, 512)*-1);
  Joystick.setRxAxis(map(analogRead(A1) , 0, 1023, -512, 512));
  Joystick.setRyAxis(map(analogRead(A5) , 0, 1023, -512, 512));
  Joystick.setRzAxis(map(analogRead(A0) , 0, 1023, -512, 512));

  // Update matrix keypad state
  if (kpd.getKeys()){
    for (int i=0; i<LIST_MAX; i++){
      auto key = kpd.key[i];
      Joystick.setButton(key.kcode, key.kstate == HOLD || key.kstate == PRESSED);
    }
  }

  // Update single pin buttons state
  for (int i=0; i<SINGLE_PIN_BUTTON_COUNT; ++i){
    Joystick.setButton(single_pin_button_numbers[i], !digitalRead(single_pin_button_pins[i]));
  }

  // Update hat buttons state
  int hats[2] = {-1, -1};
  int count = 0;
  for (int i=0; i<4; ++i){
    if (!digitalRead(hat_button_pins[i])){
      hats[count++] = i;
    }
  }
  if (count == 1){
    Joystick.setHatSwitch(0, hat_button_angles_1d[hats[0]]);
  } else if (count == 2) {
    Joystick.setHatSwitch(0, hat_button_angles_2d[hats[0]][hats[1]]);
  } else {
    Joystick.setHatSwitch(0, -1);
  }
  
  // Send to PC
  Joystick.sendState();
  delay(10);
}
