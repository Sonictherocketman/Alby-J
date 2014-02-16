/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
/*
   This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *  AutoPilot for unpowered glider flight.
 *  Author: Brian Schrader, October 2013
 *
 *  In collaboration with Bo Stump, and Tara DeRosier for
 *  UM_UAVGlider.project in accordance with San Diego State
 *  University's Aerospace Engineering Department.
 *
 */

#include <Serial3.h>
#include <SD.h>
#include <Wire.h>
#include <MemoryFree.h>
#include <avr/pgmspace.h>
#include <Servo.h>

#include <StandardCplusplus.h>
#include <bool.h>
#include <UM_Error.h>
#include <UM_MissionController.h>
#include <math.>

/* Some convinence stuff */
#define NULL 0
static const byte MIN_FREE_RAM = 25;

/* Global Variables */
static UM_Error *error;
static byte missionCompleted;
static byte safeHeight;
static File dataFile;

uint16_t *sensorData;
uint16_t *oldSensorData;
uint16_t *trajectory;
int *actions;
uint32_t timer;

/* Servos */
Servo lw, rw, tl, rd;

/* Forward Function Declarations */
void refreshSensorData();
void safetyCheck();
void UMLog(char*);
void finialize();
void dealloc();
int freeRAM();
void trajectoryFromSensorData();
void actionsForTrajectory();
uint16_t getTempFromSensorAddress(int sensorAddress);
uint16_t getHeightFromSensorAddress(byte sensorAddress);
uint16_t getVelocityFromSensorAddress(byte sensorAddress);
uint16_t getAccelFromSensorAddress(byte sensorAddress);
void initServos();
void executeActions();
void initServos();
void executeActions();
void levelOut();

/* External Mission Parameters */
/* Pin Configurations */
extern const byte VEL_ADR; 	//Velocity sensor

extern const byte TMP_ADR_1;	//Temperature sensors
extern const byte TMP_ADR_2;
extern const byte TMP_ADR_3;

extern const byte SD_ADR_RV;	//SD card 
extern const byte SD_ADR_SD;	//SD card 

extern const byte HGT_ADR;	//Z-height sensor

extern const byte ACC_ADR_X;	//Accelerometer
extern const byte ACC_ADR_Y;
extern const byte ACC_ADR_Z;

extern const byte SRV_LWG;	//Servos
extern const byte SRV_RWG;
extern const byte SRV_TL;
extern const byte SRV_RDR;

extern const UMVector MAX_MISSION_PARAMETER_TRAJECTORY;
extern const float AVERAGE_CRUISE_VELOCITY;
extern const float MIN_V_CRUISE_FACTOR;
extern const float MAX_V_CRUISE_FACTOR;
extern UM_Error *error;

extern const byte MAX_ELEVATOR_DEFLECTION;
extern const byte MID_ELEVATOR_DEFLECTION;
extern const byte MIN_ELEVATOR_DEFLECTION;

extern const byte MAX_AILERON_DEFLECTION;
extern const byte MID_AILERON_DEFLECTION;
extern const byte MIN_AILERON_DEFLECTION;

extern const byte MAX_RUDDER_DEFLECTION;
extern const byte MID_RUDDER_DEFLECTION;
extern const byte MIN_RUDDER_DEFLECTION;

extern const float ACCEL_DATA_SIGNIFICANCE_FACTOR;

/* Begin Main */
void setup() {
  missionCompleted = FALSE;
  safeHeight = FALSE;
  UMalloc();
  //Starts up the servos and attaches them to the pins.
  initServos();
  //Open the Serial3 and pin ports.
  Serial3.begin(115200);
  Wire.begin();
  //Open the SD card for logging.
  pinMode(10, OUTPUT);
  // see if the card is present and can be initialized:
  if (!SD.begin(SD_ADR_RV)) {
    UMLog("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial3.println("Sensor Data, , , , , , , Trajectory Components, , , , Actions, , , , , ,Time,, Log Messages,");
  Serial3.println(" ,T1, T2, T3, Vel, Ax, Ay, Az, , X, Y, Z, , A1, A2, A3, A4,, ");
  Serial3.println(" , C, C, C, cm/s, [UNIT], [UNIT], [UNIT], , , , , , [Action], [Action], [Action], [Actions], [TIME], ");

  UMLog("Initialization Complete.");
}

void loop() {
  //Records the time since the program has started.
  timer = (uint32_t)millis() / 1000;
  //If the timer goes past 5 mins, the program stops.
  if ((timer / 60) > 5)  
  {
    levelOut();  
    UMLog("Plane has leveled. Landing sequence executing.");
    return;
  }
  //Get get sensor data from sensors.
  refreshSensorData();
  UMLog("Sensor data refreshed.");
  //Check for safeHeight and missionCompleted.
  safetyCheck();
  //Get new trajectory from data. Then turn that into actions.
  trajectoryFromSensorData();
  if (error) {
    UMLog((char*)error->description());
    return;
  }
  actionsForTrajectory();
  if (error) {
    UMLog((char*)error->description());
    return;
  }
  UMLog("Trajectory obtained, and actions formulated.");
  //Send actions to wingController for execution.
  executeActions();
  if (error) {
    UMLog((char*)error->description());
    return;
  }
  UMLog("Actions have been executed.");
  UMDataLog();
  if (freeRAM() < (int)MIN_FREE_RAM){
    dealloc();
  }
}
/* End Main */


void safetyCheck()
{
  if (*(sensorData + 3) < 1000)  //Is the altitude of the plane less than 1 meter?
   {
   safeHeight = FALSE;
   }
}

void refreshSensorData() 
{
  //Copies the old copy to the variable oldSensorData for later use.
  for (int i = 0; i < 8; i++)
  {
    *(oldSensorData + i) = *(sensorData + i);
  }
  //Gets the new data from the sensors.
  *(sensorData + 0) = getTempFromSensorAddress(TMP_ADR_1);    //T1
  *(sensorData + 1) = getTempFromSensorAddress(TMP_ADR_2);    //T2
  *(sensorData + 2) = getTempFromSensorAddress(TMP_ADR_3);    //T3
  *(sensorData + 3) = getHeightFromSensorAddress(HGT_ADR);    //Z
  *(sensorData + 4) = getVelocityFromSensorAddress(VEL_ADR);  //Vel
  *(sensorData + 5) = getAccelFromSensorAddress(ACC_ADR_X);   //Ax
  *(sensorData + 6) = getAccelFromSensorAddress(ACC_ADR_Y);   //Ay
  *(sensorData + 7) = getAccelFromSensorAddress(ACC_ADR_Z);   //Az
}

void UMalloc() 
{
  //Allocate space for the arrays.
  sensorData = (uint16_t*)malloc(8*sizeof(uint16_t));       //Sensor data array has enough space to hold each of the sensor's data.
  oldSensorData = (uint16_t*)malloc(8*sizeof(uint16_t));    //Same as sensor data array.
  trajectory = (uint16_t*)malloc(4*sizeof(uint16_t));       //trajectory array has enough to hold an [x, y, z, magnitude]
  actions = (int*)malloc(6*sizeof(uint16_t));               //actions has a max of 5 actions. One for each control surface.
}





