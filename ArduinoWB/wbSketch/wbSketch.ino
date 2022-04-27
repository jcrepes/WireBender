/*
 * Title: MovePositionAbsolute
 *
 * Objective:
 *    This example demonstrates control of a ClearPath motor in Step and
 *    Direction mode.
 *
 * Description:
 *    This example enables a ClearPath then commands a series of repeating
 *    absolute position moves to the motor.
 *
 * Requirements:
 * 1. A ClearPath motor must be connected to Connector M-0.
 * 2. The connected ClearPath motor must be configured through the MSP software
 *    for Step and Direction mode (In MSP select Mode>>Step and Direction).
 * 3. The ClearPath motor must be set to use the HLFB mode "ASG-Position
 *    w/Measured Torque" with a PWM carrier frequency of 482 Hz through the MSP
 *    software (select Advanced>>High Level Feedback [Mode]... then choose
 *    "ASG-Position w/Measured Torque" from the dropdown, make sure that 482 Hz
 *    is selected in the "PWM Carrier Frequency" dropdown, and hit the OK
 *    button).
 * 4. Set the Input Format in MSP for "Step + Direction".
 *
 * ** Note: Homing is optional, and not required in this operational mode or in
 *    this example. This example makes positive absolute position moves,
 *    assuming any homing move occurs in the negative direction.
 *
 * ** Note: Set the Input Resolution in MSP the same as your motor's Positioning
 *    Resolution spec if you'd like the pulses sent by ClearCore to command a
 *    move of the same number of Encoder Counts, a 1:1 ratio.
 *
 * Links:
 * ** ClearCore Documentation: https://teknic-inc.github.io/ClearCore-library/
 * ** ClearCore Manual: https://www.teknic.com/files/downloads/clearcore_user_manual.pdf
 * ** ClearPath Manual (DC Power): https://www.teknic.com/files/downloads/clearpath_user_manual.pdf
 * ** ClearPath Manual (AC Power): https://www.teknic.com/files/downloads/ac_clearpath-mc-sd_manual.pdf
 *
 * 
 * Copyright (c) 2020 Teknic Inc. This work is free to use, copy and distribute under the terms of
 * the standard MIT permissive software license which can be found at https://opensource.org/licenses/MIT
 */

#include "ClearCore.h"
#include <SPI.h>
#include <SD.h>

//#define HBridgeConnector ConnectorIO4
// Specifies which motor to move.
// Options are: ConnectorM0, ConnectorM1, ConnectorM2, or ConnectorM3.
#define motorX ConnectorM0
#define motorY ConnectorM1
#define motorZ ConnectorM2

// Select the baud rate to match the target serial device
#define baudRate 9600

// Define the velocity and acceleration limits to be used for each move
int velocityLimit = 1000; // pulses per sec
int accelerationLimit = 1000; // pulses per sec^2
String rawData = "";
String material = "";
String currentAxis = "";
unsigned int lastN = 0;
float moveForward = 0.0;
float degreeBend = 0.0;
File myFile;

// Declares our user-defined helper function, which is used to command moves to
// the motor. The definition/implementation of this function is at the  bottom
// of the example
bool MoveAbsolutePosition(int32_t position);
void readInstructions(int count);

//bool outputState;
void setup() {
    // Put your setup code here, it will only run once:
    //HBridgeConnector.Mode(Connector::OUTPUT_H_BRIDGE);
    
    // Sets the input clocking rate. This normal rate is ideal for ClearPath
    // step and direction applications.
    MotorMgr.MotorInputClocking(MotorManager::CLOCK_RATE_NORMAL);

    // Sets all motor connectors into step and direction mode.
    MotorMgr.MotorModeSet(MotorManager::MOTOR_ALL,
                          Connector::CPM_MODE_STEP_AND_DIR);
    //pinMode(IO5, OUTPUT);
    // Set the motor's HLFB mode to bipolar PWM
    motorX.HlfbMode(MotorDriver::HLFB_MODE_HAS_BIPOLAR_PWM);
    motorY.HlfbMode(MotorDriver::HLFB_MODE_HAS_BIPOLAR_PWM);
    motorZ.HlfbMode(MotorDriver::HLFB_MODE_HAS_BIPOLAR_PWM);
    // Set the HFLB carrier frequency to 482 Hz
    motorX.HlfbCarrier(MotorDriver::HLFB_CARRIER_482_HZ);
    motorY.HlfbCarrier(MotorDriver::HLFB_CARRIER_482_HZ);
    motorZ.HlfbCarrier(MotorDriver::HLFB_CARRIER_482_HZ);

    // Sets the maximum velocity for each move
    motorX.VelMax(velocityLimit);
    motorY.VelMax(velocityLimit);
    motorZ.VelMax(velocityLimit);

    // Set the maximum acceleration for each move
    motorX.AccelMax(accelerationLimit);
    motorY.AccelMax(accelerationLimit);
    motorZ.AccelMax(accelerationLimit);
    //outputState = true;
    // Sets up serial communication and waits up to 5 seconds for a port to open.
    // Serial communication is not required for this example to run. 
    Serial.begin(baudRate);
    uint32_t timeout = 5000;
    uint32_t startTime = millis();
    while (!Serial && millis() - startTime < timeout) {
        continue;
    }
 
/*

  SD card test

  This example shows how use the utility libraries on which the'

  SD library is based in order to get info about your SD card.

  Very useful for testing a card when you're not sure whether its working or not.

  Pin numbers reflect the default SPI pins for Uno and Nano models

  The circuit:

    SD card attached to SPI bus as follows:

 ** SDO - pin 11 on Arduino Uno/Duemilanove/Diecimila

 ** SDI - pin 12 on Arduino Uno/Duemilanove/Diecimila

 ** CLK - pin 13 on Arduino Uno/Duemilanove/Diecimila

 ** CS - depends on your SD card shield or module.

        Pin 10 used here for consistency with other Arduino examples

  created  28 Mar 2011

  by Limor Fried

  modified 24 July 2020

  by Tom Igoe

*/


// set up variables using the SD utility library functions:

Sd2Card card;

SdVolume volume;

SdFile root;

// change this to match your SD shield or module;
// Default SPI on Uno and Nano: pin 10
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// MKRZero SD: SDCARD_SS_PIN

const int chipSelect = 10;


  Serial.print("\nInitializing SD card...");

  // we'll use the initialization code from the utility libraries

  // since we're just testing if the card is working!

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {

    Serial.println("initialization failed. Things to check:");

    Serial.println("* is a card inserted?");

    Serial.println("* is your wiring correct?");

    Serial.println("* did you change the chipSelect pin to match your shield or module?");

    while (1);

  } else {

    Serial.println("Wiring is correct and a card is present.");

  }

  // print the type of card

  Serial.println();

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32

  if (!volume.init(card)) {

    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");

    while (1);

  }

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");

  root.openRoot(volume);

  // list all files in the card with date and size

  root.ls(LS_R | LS_DATE | LS_SIZE);

  root.close();

  if (!SD.begin()) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
   if (SD.exists("WIREBE~1.TXT")) {
    Serial.println("FILE FOUND");
   }
   myFile = SD.open("WIREBE~1.TXT");
    if (myFile) {
      //check end of file
      while ( myFile.available() ){
        rawData.concat(char(myFile.read()));
      }
      
      //DEBUGGING PRINT STATEMENT CHECK IF STRING IS CORRECT
      Serial.println(rawData);
      myFile.close();
      //root.close();
      if (rawData.startsWith("Copper")) {
        material = "Copper";
      } else if (rawData.startsWith("Steel")) {
        material = "Steel";
      } else {
        Serial.println("Material not found.");
      }
    } else Serial.println("Unable to open file");
    
    // Enables the motor; homing will begin automatically if enabled
    motorX.EnableRequest(true);
    Serial.println("MotorX Enabled");
    motorY.EnableRequest(true);
    Serial.println("MotorY Enabled");
    motorZ.EnableRequest(true);
    Serial.println("MotorZ Enabled");

    // Waits for HLFB to assert (waits for homing to complete if applicable)
    Serial.println("Waiting for HLFB...");
    while (motorX.HlfbState() != MotorDriver::HLFB_ASSERTED) {
        continue;
    }
    Serial.println("MotorX Ready");
    while (motorY.HlfbState() != MotorDriver::HLFB_ASSERTED) {
        continue;
    }
    Serial.println("MotorY Ready");
    while (motorZ.HlfbState() != MotorDriver::HLFB_ASSERTED) {
        continue;
    }
    Serial.println("MotorZ Ready");
}

void readInstructions() {
    if (lastN == 0) {
      lastN = rawData.indexOf("\\") + 1;
    }
    int firstSpace = rawData.indexOf(" ", lastN + 1);
    moveForward = (rawData.substring(lastN + 1, firstSpace)).toFloat();
    int secondSpace = rawData.indexOf(" ", firstSpace + 1);
    degreeBend = (rawData.substring(firstSpace + 1, secondSpace)).toFloat();
    currentAxis = rawData.substring(secondSpace + 1, secondSpace + 2);
    lastN = secondSpace + 2;
}

void loop() {
    // Put your main code here, it will run repeatedly:
    if (lastN + 2 < rawData.length()) {
      readInstructions();
      //moveForward
      int degreeMove = int((800*degreeBend)/360);
      //moves relative to last position. Will initially move counter-clockwise and following directions will go in the opposite direction of last
      MoveAbsolutePosition(degreeMove);
    } else {
      exit(0);
    }
    
    // Move to +10000 counts (positive direction), then wait 2000ms
    //digitalWrite(IO5,true);
    //delay(1000);
    //MoveAbsolutePosition(10000);
    //delay(1000);
    // Move back to "home", then wait 2000ms
    //MoveAbsolutePosition(0);
    //delay(1000);
    //digitalWrite(IO5,false);
    //delay(1000);
    //HBridgeConnector.State(INT16_MAX/2);
    //delay(1000);
    //HBridgeConnector.State(-INT16_MAX/2);
    //delay(1000);
}

/*------------------------------------------------------------------------------
 * MoveAbsolutePosition
 *
 *    Command step pulses to move the motor's current position to the absolute
 *    position specified by "position"
 *    Prints the move status to the USB serial port
 *    Returns when HLFB asserts (indicating the motor has reached the commanded
 *    position)
 *
 * Parameters:
 *    int position  - The absolute position, in step pulses, to move to
 *
 * Returns: True/False depending on whether the move was successfully triggered.
 */
bool MoveAbsolutePosition(int position) {
    // Check if an alert is currently preventing motion
    if (currentAxis == "x") {
      if (motorX.StatusReg().bit.AlertsPresent) {
        Serial.println("MotorX status: 'In Alert'. Move Canceled.");
        return false;
      }

      Serial.print("Moving to absolute position: ");
      Serial.println(position);

      // Command the move of absolute distance
      motorX.Move(position, MotorDriver::MOVE_TARGET_ABSOLUTE);

      // Waits for HLFB to assert (signaling the move has successfully completed)
      Serial.println("Moving.. Waiting for HLFB");
      while (!motorX.StepsComplete() || motorX.HlfbState() != MotorDriver::HLFB_ASSERTED) {
        continue;
      }
    }
    if (currentAxis == "y") {
      if (motorY.StatusReg().bit.AlertsPresent) {
        Serial.println("MotorY status: 'In Alert'. Move Canceled.");
        return false;
      }

      Serial.print("Moving to absolute position: ");
      Serial.println(position);

      // Command the move of absolute distance
      motorY.Move(position, MotorDriver::MOVE_TARGET_ABSOLUTE);

      // Waits for HLFB to assert (signaling the move has successfully completed)
      Serial.println("Moving.. Waiting for HLFB");
      while (!motorY.StepsComplete() || motorY.HlfbState() != MotorDriver::HLFB_ASSERTED) {
        continue;
      }
    }
    if (currentAxis == "z") {
      if (motorZ.StatusReg().bit.AlertsPresent) {
        Serial.println("MotorZ status: 'In Alert'. Move Canceled.");
        return false;
      }

      Serial.print("Moving to absolute position: ");
      Serial.println(position);

      // Command the move of absolute distance
      motorZ.Move(position, MotorDriver::MOVE_TARGET_ABSOLUTE);

      // Waits for HLFB to assert (signaling the move has successfully completed)
      Serial.println("Moving.. Waiting for HLFB");
      while (!motorZ.StepsComplete() || motorZ.HlfbState() != MotorDriver::HLFB_ASSERTED) {
        continue;
      }
    }
   
    Serial.println("Move Done");
    return true;
}
//------------------------------------------------------------------------------
