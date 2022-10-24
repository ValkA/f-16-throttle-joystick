#include <Joystick.h>

// Init hat buttons 
int hat_button_pins[4] =      {13, 12, 11,  10}; // Button pins, according to the angles in hat_button_angles_1d - 0 is up, 90 is right.
int hat_button_angles_1d[4] = {0,  90, 180, 270};
int hat_button_angles_2d[4][4] = {
  {0, 45, -1, 315},
  {0, 90, 135, -1},
  {-1, 135, 180, 225},
  {315, -1, 225, 270},
};

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
  10, 1,           // Button Count, Hat Switch Count
  true, true, false,       // X, Y, Z Axis
  true, true, true,       // Rx, Ry, Rz
  false, true,             // rudder, throttle
  false, false, false);    // accelerator, brake, steering

void setup() {
  // Initialize Button Pins
  for (int i=0; i<14; ++i){
    pinMode(i, INPUT_PULLUP);
  }
  // Initialize Analog Pins
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
  Joystick.setThrottle(map(analogRead(A4) , 0, 1023, -512, 512) * -1);
  Joystick.setXAxis(map(analogRead(A3) , 0, 1023, -512, 512));
  Joystick.setYAxis(map(analogRead(A2) , 0, 1023, -512, 512));
  Joystick.setRxAxis(map(analogRead(A1) , 0, 1023, -512, 512));
  Joystick.setRyAxis(map(analogRead(A5) , 0, 1023, -512, 512));
  Joystick.setRzAxis(map(analogRead(A0) , 0, 1023, -512, 512));
  
  for (int i=0; i<10; ++i){
    Joystick.setButton(i, !digitalRead(i));
  }

  // hat buttons logic
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
  
  
  Joystick.sendState();
  delay(10);
}



