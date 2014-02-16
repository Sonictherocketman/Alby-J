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
 *  Constants and values related to mission parameters.
 *  Author: Brian Schrader, October 2013
 *
 *  In collaboration with Bo Stump, and Tara DeRosier for
 *  UM_UAVGlider.project in accordance with San Diego State
 *  University's Aerospace Engineering Department.
 *
 */

#ifndef UM_MISSIONCONTROLLER_H
#define UM_MISSIONCONTROLLER_H

typedef struct UMVector
{
  float x;
  float y;
  float z;
  float magnitude;
}UM_Vector;

/* Edit these parameters for testing */
const UMVector MAX_MISSION_PARAMETER_TRAJECTORY = {1, 30, 5, 0};
const float AVERAGE_CRUISE_VELOCITY = 5.0;  //Meters per second.
const float MIN_V_CRUISE_FACTOR = 0.9;
const float MAX_V_CRUISE_FACTOR = 1.1;
const float ACCEL_DATA_SIGNIFICANCE_FACTOR = 0.3;

const byte MAX_ELEVATOR_DEFLECTION = 5;		//Degrees
const byte MID_ELEVATOR_DEFLECTION = 3;
const byte MIN_ELEVATOR_DEFLECTION = 1;

const byte MAX_AILERON_DEFLECTION = 5;
const byte MID_AILERON_DEFLECTION = 3;
const byte MIN_AILERON_DEFLECTION = 1;

const byte MAX_RUDDER_DEFLECTION = 5;
const byte MID_RUDDER_DEFLECTION = 3;
const byte MIN_RUDDER_DEFLECTION = 1;


/* Pin Configurations */
const byte VEL_ADR	= A1; 		//Velocity sensor

const byte TMP_ADR_1 = 0x48;	//Temperature sensors
const byte TMP_ADR_2 = 73;
const byte TMP_ADR_3 = 74;

const byte ACC_ADR_X = A5;		//Accelerometer Sensor
const byte ACC_ADR_Y = A7;
const byte ACC_ADR_Z = A9;

const byte SD_ADR_SD = 14;		//SD card 
const byte SD_ADR_RV = 15;

const byte HGT_ADR	= 9;		//Z-height sensor

const byte SRV_LWG	= 5;		//Servos
const byte SRV_RWG	= 10;
const byte SRV_TL	= 11;
const byte SRV_RDR	= 46;

#endif
