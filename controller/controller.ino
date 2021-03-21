#include <krpc.h>
#include <krpc/services/krpc.h>
#include <krpc/services/space_center.h>

//Variable creation
HardwareSerial * conn;

unsigned long currentMillis;

bool reset = false;
bool abortActive = false;
bool stagingActive = false;

float throttleVal;
float pitchVal;
float yawVal;
float rollVal;
float upVal;
float rightVal;
float forwardVal;
const float maxAnalogVal = 1023;

unsigned long previousAbortLightMillis = 0;
const long abortLightInterval = 1000;

//Light pin settings
int gearLightState = LOW;
int lightsLightState = LOW;
int brakesLightState = LOW;
int abortLightState = LOW;
int nextStageLightState = LOW;
int SASLightState = LOW;
int SASModeLightState = LOW; //Probably not going to output via on/off light, this is just for testing.
int cargoLightState = LOW;
int radiatorLightState = LOW;
int solarLightState = LOW;
int antLightState = LOW;
int RCSLightState = LOW;
int reactionLightState = LOW;
int actionGroupGroupLightState = LOW; //Probably not going to output via on/off light, this is just for testing.
int actionGroupModeLightState = LOW;


//Digital pin assignment
const int gearInPin = 22;
const int gearOutPin = 23;

const int lightsInPin = 24;
const int lightsOutPin = 25;

const int brakesInPin = 26;
const int brakesOutPin = 27;

const int abortInPin = 28;
const int abortOutPin = 29;

const int activateNextStageInPin = 30;
const int activateNextStageOutPin = 31;

const int SASInPin = 32;
const int SASOutPin = 33;

const int SASModeInPin = 34;
const int SASModeOutPin = 35;

const int cargoBayInPin = 36;
const int cargoBayOutPin = 37;

const int radiatorsInPin = 38;
const int radiatorsOutPin = 39;

const int solarInPin = 40;
const int solarOutPin = 41;

const int antInPin = 42;
const int antOutPin = 43;

const int RCSInPin = 44;
const int RCSOutPin = 45;

const int reactionInPin = 46;
const int reactionOutPin = 47;

const int actionGroupGroupInPin = 48;
const int actionGroupGroupOutPin = 49;

const int actionGroupModeInPin = 50;
const int actionGroupModeOutPin = 51;

//Analog pin assignment
const int throttleInputPin = A0;
const int pitchInputPin = A1;
const int yawInputPin = A2;
const int rollInputPin = A3;
const int upInputPin = A4;
const int rightInputPin = A5;
const int forwardInputPin = A6;

krpc_SpaceCenter_Flight_t flight;
krpc_SpaceCenter_Control_t control;
krpc_SpaceCenter_Vessel_t vessel;

void setup() {
  //Pin I/O setup
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(gearInPin, INPUT_PULLUP);
  pinMode(gearOutPin, OUTPUT);
  pinMode(lightsInPin, INPUT_PULLUP);
  pinMode(lightsOutPin, OUTPUT);
  pinMode(brakesInPin, INPUT_PULLUP);
  pinMode(brakesOutPin, OUTPUT);
  pinMode(abortInPin, INPUT_PULLUP);
  pinMode(abortOutPin, OUTPUT);
  pinMode(activateNextStageInPin, INPUT_PULLUP);
  pinMode(activateNextStageOutPin, OUTPUT);
  pinMode(SASInPin, INPUT_PULLUP);
  pinMode(SASOutPin, OUTPUT);
  pinMode(SASModeInPin, INPUT_PULLUP);
  pinMode(SASModeOutPin, OUTPUT);
  pinMode(cargoBayInPin, INPUT_PULLUP);
  pinMode(cargoBayOutPin, OUTPUT);
  pinMode(radiatorsInPin, INPUT_PULLUP);
  pinMode(radiatorsOutPin, OUTPUT);
  pinMode(solarInPin, INPUT_PULLUP);
  pinMode(solarOutPin, OUTPUT);
  pinMode(antInPin, INPUT_PULLUP);
  pinMode(antOutPin, OUTPUT);
  pinMode(RCSInPin, INPUT_PULLUP);
  pinMode(RCSOutPin, OUTPUT);
  pinMode(reactionInPin, INPUT_PULLUP);
  pinMode(reactionOutPin, OUTPUT);
  pinMode(actionGroupGroupInPin, INPUT_PULLUP);
  pinMode(actionGroupGroupOutPin, OUTPUT);
  pinMode(actionGroupModeInPin, INPUT_PULLUP);
  pinMode(actionGroupModeOutPin, OUTPUT);

  conn = &Serial;
  krpc_open(&conn, NULL);
  krpc_connect(conn, "Arduino Repeater");
  krpc_SpaceCenter_ActiveVessel(conn, &vessel);
  krpc_SpaceCenter_Vessel_Control(conn, &control, vessel);
  krpc_SpaceCenter_Vessel_Flight(conn, &flight, vessel, KRPC_NULL);

  // Indicate succesful connection by lighting the on-board LED
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  currentMillis = millis();

  // Check if game has been reset
//  if ((krpc_SpaceCenterVessel_Situation(conn) == KRPC_SPACECENTER_VESSELSITUATION_PRELAUNCH) & (reset == false)) {
//    reset = true;
//  }

  //Digital Control Inputs//
  
  // Landing gear
  if (digitalRead(gearInPin) == HIGH) {
    krpc_SpaceCenter_Control_set_Gear(conn, control, true);
    digitalWrite(gearOutPin, HIGH);
  } else {
    krpc_SpaceCenter_Control_set_Gear(conn, control, false);
    digitalWrite(gearOutPin, LOW);
  }

  // Lights
  if (digitalRead(lightsInPin) == HIGH) {
    krpc_SpaceCenter_Control_set_Lights(conn, control, true);
    digitalWrite(lightsOutPin, HIGH);
  } else {
    krpc_SpaceCenter_Control_set_Lights(conn, control, false);
    digitalWrite(lightsOutPin, LOW);
  }

  // Brakes
  if (digitalRead(brakesInPin) == HIGH) {
    krpc_SpaceCenter_Control_set_Brakes(conn, control, true);
    digitalWrite(brakesOutPin, HIGH);
  } else {
    krpc_SpaceCenter_Control_set_Brakes(conn, control, false);
    digitalWrite(brakesOutPin, LOW);
  }

  // Abort
  if (digitalRead(abortInPin) == HIGH) {
  //krpc_SpaceCenter_Control_set_Abort(conn, control, true);
    abortActive = true;
  }

  // Activate next stage
  if ((digitalRead(activateNextStageInPin) == HIGH) & (stagingActive == false)) {
    krpc_list_object_t stageResults;
    krpc_SpaceCenter_Control_ActivateNextStage(conn, &stageResults, control);
    stagingActive = true;
  } else {
    stagingActive = digitalRead(activateNextStageInPin);
  }
  
  //SAS on/off
  if (digitalRead(SASInPin) == HIGH) {
    krpc_SpaceCenter_Control_set_SAS(conn, control, true);
    digitalWrite(SASOutPin, HIGH);
  } else {
    krpc_SpaceCenter_Control_set_SAS(conn, control, false);
    digitalWrite(SASOutPin, LOW);
  }

  //SAS mode settings - multi
  //Learn how to multi

  //Cargo bays open/close
  if (digitalRead(cargoBayInPin) == HIGH) {
    krpc_SpaceCenter_Control_set_CargoBays(conn, control, true);
    digitalWrite(cargoBayOutPin, HIGH);
  } else {
    krpc_SpaceCenter_Control_set_CargoBays(conn, control, false);
    digitalWrite(cargoBayOutPin, LOW);
  }

  //Radiators open/close
  if (digitalRead(radiatorsInPin) == HIGH) {
    krpc_SpaceCenter_Control_set_Radiators(conn, control, true);
    digitalWrite(radiatorsOutPin, HIGH);
  } else {
    krpc_SpaceCenter_Control_set_Radiators(conn, control, false);
    digitalWrite(radiatorsOutPin, LOW);
  }

  //Solar panels open/close
  if (digitalRead(solarInPin) == HIGH) {
    krpc_SpaceCenter_Control_set_SolarPanels(conn, control, true);
    digitalWrite(solarOutPin, HIGH);
  } else {
    krpc_SpaceCenter_Control_set_SolarPanels(conn, control, false);
    digitalWrite(solarOutPin, LOW);
  }

  //Antennas open/close
  if (digitalRead(antInPin) == HIGH) {
    krpc_SpaceCenter_Control_set_Antennas(conn, control, true);
    digitalWrite(antOutPin, HIGH);
  } else {
    krpc_SpaceCenter_Control_set_Antennas(conn, control, false);
    digitalWrite(antOutPin, LOW);
  }

  //RCS on/off
  if (digitalRead(RCSInPin) == HIGH) {
    krpc_SpaceCenter_Control_set_RCS(conn, control, true);
    digitalWrite(RCSOutPin, HIGH);
  } else {
    krpc_SpaceCenter_Control_set_RCS(conn, control, false);
    digitalWrite(RCSOutPin, LOW);
  }

  //Reaction Wheels on/off
  if (digitalRead(reactionInPin) == HIGH) {
    krpc_SpaceCenter_Control_set_ReactionWheels(conn, control, true);
    digitalWrite(reactionOutPin, HIGH);
  } else {
    krpc_SpaceCenter_Control_set_ReactionWheels(conn, control, false);
    digitalWrite(reactionOutPin, LOW);
  }

  //Action group group settings - multi
  //Learn how to multi
  
  //Action group mode on/off
  //Need to learn how to multi first as this needs the output of getActionGroup to run
  

  // Abort light control
  if ((currentMillis - previousAbortLightMillis >= abortLightInterval) & (abortActive)) {
    previousAbortLightMillis = currentMillis;
    
    if (abortLightState == LOW) {
      abortLightState = HIGH;
    } else {
      abortLightState = LOW;
    }
    digitalWrite(abortOutPin, abortLightState);
  }



//Analog Control Inputs//

//   Read throttle input
  throttleVal = (float) 1 - (maxAnalogVal - (float) analogRead(throttleInputPin)) / maxAnalogVal;
  krpc_SpaceCenter_Control_set_Throttle(conn, control, throttleVal);

//   Read pitch input
  pitchVal = (((float) 1 - (maxAnalogVal - (float) analogRead(pitchInputPin)) / maxAnalogVal) * (float) 2) - (float) 1;
  krpc_SpaceCenter_Control_set_Pitch(conn, control, pitchVal);

//   Read yaw input
  yawVal = (((float) 1 - (maxAnalogVal - (float) analogRead(yawInputPin)) / maxAnalogVal) * (float) 2) - (float) 1;
  krpc_SpaceCenter_Control_set_Yaw(conn, control, yawVal);

//   Read roll input
  rollVal = (((float) 1 - (maxAnalogVal - (float) analogRead(rollInputPin)) / maxAnalogVal) * (float) 2) - (float) 1;
  krpc_SpaceCenter_Control_set_Roll(conn, control, rollVal);

//   Read up input
  upVal = (((float) 1 - (maxAnalogVal - (float) analogRead(upInputPin)) / maxAnalogVal) * (float) 2) - (float) 1;
  krpc_SpaceCenter_Control_set_Up(conn, control, upVal);

//   Read right input
  rightVal = (((float) 1 - (maxAnalogVal - (float) analogRead(rightInputPin)) / maxAnalogVal) * (float) 2) - (float) 1;
  krpc_SpaceCenter_Control_set_Right(conn, control, rightVal);

//   Read forward input
  forwardVal = (((float) 1 - (maxAnalogVal - (float) analogRead(forwardInputPin)) / maxAnalogVal) * (float) 2) - (float) 1;
  krpc_SpaceCenter_Control_set_Forward(conn, control, forwardVal);
  
}
