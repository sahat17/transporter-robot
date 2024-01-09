#include "PS2X_lib.h"  //for v1.6

#define ENA 3      //ENA
#define MOTORA_1 4 //IN3
#define MOTORA_2 5 //IN4
#define MOTORB_1 8 //IN1
#define MOTORB_2 7 //IN2
#define ENB 6      //ENB

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you conect the controller,
//or call config_gamepad(pins) again after connecting the controller.
int error = 0;
byte type = 0;
byte vibrate = 0;

void setup() {
  // Configure output pins
  pinMode(ENA, OUTPUT);
  pinMode(MOTORA_1, OUTPUT);
  pinMode(MOTORA_2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(MOTORB_1, OUTPUT);
  pinMode(MOTORB_2, OUTPUT);

  // Disable both motors
  digitalWrite(ENA, 0);
  digitalWrite(ENB, 0);

  Serial.begin(57600);

  error = ps2x.config_gamepad(10, 12, 11, 13, true, true); //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  if (error == 0) {
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
  }

  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  type = ps2x.readType();

  switch (type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
    default:
      break;
  }
}

void loop() {
  /* You must Read Gamepad to get new values
    Read GamePad and set vibration values
    ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
    if you don't enable the rumble, use ps2x.read_gamepad(); with no values

    you should call this at least once a second
  */

  ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed

  if (ps2x.Button(PSB_START))                  //will be TRUE as long as button is pressed
    Serial.println("Start is being held");
  if (ps2x.Button(PSB_SELECT))
    Serial.println("Select is being held");

  if (ps2x.Button(PSB_PAD_UP)) {        //will be TRUE as long as button is pressed
    Serial.println("UP is being held");
    digitalWrite(MOTORA_1, LOW);
    digitalWrite(MOTORA_2, HIGH);
    digitalWrite(MOTORB_1, HIGH);
    digitalWrite(MOTORB_2, LOW);
    analogWrite(ENB, 1023);
    analogWrite(ENA, 1023);
  }
  if (ps2x.Button(PSB_PAD_RIGHT)) {
    Serial.println("Right is being held");
    digitalWrite(MOTORA_1, HIGH);
    digitalWrite(MOTORA_2, LOW);
    digitalWrite(MOTORB_1, HIGH);
    digitalWrite(MOTORB_2, LOW);
    analogWrite(ENB, 1023);
    analogWrite(ENA, 1023);
  }
  if (ps2x.Button(PSB_PAD_LEFT)) {
    Serial.println("LEFT is being held");
    digitalWrite(MOTORA_1, LOW);
    digitalWrite(MOTORA_2, HIGH);
    digitalWrite(MOTORB_1, LOW);
    digitalWrite(MOTORB_2, HIGH);
    analogWrite(ENB, 1023);
    analogWrite(ENA, 1023);
  }
  if (ps2x.Button(PSB_PAD_DOWN)) {
    Serial.println("DOWN is being held");
    digitalWrite(MOTORA_1, HIGH);
    digitalWrite(MOTORA_2, LOW);
    digitalWrite(MOTORB_1, LOW);
    digitalWrite(MOTORB_2, HIGH);
    analogWrite(ENB, 1023);
    analogWrite(ENA, 1023);
  }

  if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
  {
    if (ps2x.Button(PSB_L3))
      Serial.println("L3 pressed");
    if (ps2x.Button(PSB_R3))
      Serial.println("R3 pressed");
    if (ps2x.Button(PSB_L2))
      Serial.println("L2 pressed");
    if (ps2x.Button(PSB_R2))
      Serial.println("R2 pressed");
  }

  if (ps2x.ButtonPressed(PSB_RED))            //will be TRUE if button was JUST pressed
    Serial.println("Circle just pressed");

  if (ps2x.ButtonReleased(PSB_RED))            //will be TRUE if button was JUST released
    Serial.println("Circle just released");


  if (ps2x.ButtonPressed(PSB_PINK))            //will be TRUE if button was JUST pressed
    Serial.println("Square just pressed");

  if (ps2x.ButtonReleased(PSB_PINK))            //will be TRUE if button was JUST released
    Serial.println("Square just released");

  if (ps2x.ButtonPressed(PSB_GREEN))            //will be TRUE if button was JUST pressed
    Serial.println("Triangle just pressed");

  if (ps2x.ButtonReleased(PSB_GREEN))            //will be TRUE if button was JUST released
    Serial.println("Triangle just released");

  if (ps2x.ButtonPressed(PSB_BLUE))            //will be TRUE if button was JUST pressed
    Serial.println("X just pressed");

  if (ps2x.ButtonReleased(PSB_BLUE))            //will be TRUE if button was JUST released
    Serial.println("X just released");


  //if(ps2x.NewButtonState(PSB_BLUE))            //will be TRUE if button was JUST pressed OR released
  //     Serial.println("X just changed");

  if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE
  {
    Serial.print("Stick Values:");
    Serial.print(ps2x.Analog(PSS_LY), DEC); // LY
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_LX), DEC); // LX
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_RY), DEC); // RY
    Serial.print(",");
    Serial.println(ps2x.Analog(PSS_RX), DEC); // RX
  }

  delay(50);
}
