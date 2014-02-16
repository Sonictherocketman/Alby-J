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
 *  Auto correcting trajectory
 *  Author: Brian Schrader, October 2013
 *
 */


#include "UM_Trajectory.h"
#include <stdlib.h>
#include <bool.h>
#include <math.h>

extern const UMVector MAX_MISSION_PARAMETER_TRAJECTORY;
extern const float AVERAGE_CRUISE_VELOCITY;
extern const float MIN_V_CRUISE_FACTOR;
extern const float MAX_V_CRUISE_FACTOR;

/*
 * Begin Public Method Declarations
 */
 
void UM_Trajectory::init()
{
	struct UMVector t = {0,0,0,0};
	*this->currentTrajectory = t;
}

bool UM_Trajectory::isDifferentFromTrajectory(UM_Trajectory *trajectory)
{
	float magRes = this->percentError(trajectory->currentTrajectory->magnitude, this->currentTrajectory->magnitude);
	float xRes = this->percentError(trajectory->currentTrajectory->x, this->currentTrajectory->x);
	float yRes = this->percentError(trajectory->currentTrajectory->y, this->currentTrajectory->y);
	float zRes = this->percentError(trajectory->currentTrajectory->z, this->currentTrajectory->z);

    //Only returns true if ALL the parameters are less than 10%.
	if (magRes <= 0.1 && xRes <= 0.1 && yRes <= 0.1 && zRes <= 0.1)	{return FALSE;}
	return TRUE;
}

bool UM_Trajectory::isWithinMissionParameters()
{
	if (this->currentTrajectory->x > MAX_MISSION_PARAMETER_TRAJECTORY.x ||
			this->currentTrajectory->x < -MAX_MISSION_PARAMETER_TRAJECTORY.x)
	{return FALSE;}

	if (this->currentTrajectory->y > MAX_MISSION_PARAMETER_TRAJECTORY.y ||
			this->currentTrajectory->y < -MAX_MISSION_PARAMETER_TRAJECTORY.y)
	{return FALSE;}

	if (this->currentTrajectory->z > MAX_MISSION_PARAMETER_TRAJECTORY.z ||
			this->currentTrajectory->z < -MAX_MISSION_PARAMETER_TRAJECTORY.z)
	{return FALSE;}

	return TRUE;
}

UM_Trajectory* UM_Trajectory::trajectoryFromSensorData(UM_SensorDataCollection *sensorData, bool missionCompleted, bool safeHeight, UM_Error *error)
{
	UM_Trajectory *trajectory = new UM_Trajectory();
	if (safeHeight)
	{
		if (missionCompleted)
		{
            UM_Vector vector = trajectory->vectorForMissionCompleted(sensorData);
            *trajectory->currentTrajectory = vector;
		}
		else
		{
            UM_Vector vector = trajectory->vectorForMissionNotCompleted(sensorData);
            *trajectory->currentTrajectory = vector;
		}
	}
	else
	{
		//Perform unsafe height maneuvers.
	}
	return trajectory;
}

/*
 * End Public Method Declarations
 *
 * Begin Private Method Declarations
 */

UMVector UM_Trajectory::vectorForMissionCompleted(UM_SensorDataCollection *sensorData)
{
	UM_Vector vector;

	//Depends on the Algorithm details. [TODO!]

	return vector;
}

UM_Vector UM_Trajectory::vectorForMissionNotCompleted(UM_SensorDataCollection *sensorData)
{
	UM_Vector vector;

	//Check to see if the accelerometer has significant data. If so,
	//the algorithm will account for the gust before the other parameters.
	if(sensorData->hasSigAccelData())
	{
		//Corroborates the acceleration data.
		UMVector acceleration = {0,0,0,0};
		acceleration.x = -*sensorData->Ax;
		acceleration.y = -*sensorData->Ay;
		acceleration.z = -*sensorData->Az;

		if (acceleration.x > MAX_MISSION_PARAMETER_TRAJECTORY.x)	{acceleration.x = MAX_MISSION_PARAMETER_TRAJECTORY.x;}
		if (acceleration.x < -MAX_MISSION_PARAMETER_TRAJECTORY.x)	{acceleration.x = -MAX_MISSION_PARAMETER_TRAJECTORY.x;}

		if (acceleration.y > MAX_MISSION_PARAMETER_TRAJECTORY.y)	{acceleration.y = MAX_MISSION_PARAMETER_TRAJECTORY.y;}
		if (acceleration.y < -MAX_MISSION_PARAMETER_TRAJECTORY.y)	{acceleration.y = -MAX_MISSION_PARAMETER_TRAJECTORY.y;}

		if (acceleration.z > MAX_MISSION_PARAMETER_TRAJECTORY.z)	{acceleration.z = MAX_MISSION_PARAMETER_TRAJECTORY.z;}
		if (acceleration.z < -MAX_MISSION_PARAMETER_TRAJECTORY.z)	{acceleration.z = -MAX_MISSION_PARAMETER_TRAJECTORY.z;}

		vector = acceleration;
        free(&acceleration);
    }
	//No significant acceleration was detected. Default to Velocity and temp checks
	else
	{
		UM_Vector velocity = {0,0,0,0};
		UM_Vector temp = {0,0,0,0};

		UM_Vector T1 = {0,0,0,0};
		UM_Vector T2 = {0,0,0,0};
		UM_Vector T3 = {0,0,0,0};

		T1.y = *sensorData->T1;
		T2.y = *sensorData->T2;
		T3.y = *sensorData->T3;

		UM_Vector temps[] = {T1, T2, T3};
		UM_Vector largest = {0, 0, 0};
		int index;

		//Finds the largest temp
		for (int i = 0; i < sizeof(temps)/sizeof(temps[0]); i++)
		{
			//Not sure if works? FIX [POSSIBLE BUG]
			temps[i].x = 1;
			if (largest.y < temps[i].y)	{largest = temps[i]; index = i;}
		}
		switch(index)
		{
		case(0):
			temp.y = 0;
			break;
		case(1):
			temp.y = MAX_MISSION_PARAMETER_TRAJECTORY.y;
			break;
		case(2):
			temp.y = -MAX_MISSION_PARAMETER_TRAJECTORY.y;
			break;
		}

		//Gets the velocity factor.
		float Vcurrent = *sensorData->Vel;
		float Vfactor = Vcurrent/AVERAGE_CRUISE_VELOCITY;

		//Takes the log of the Vfactor to turn the equation into a vector slope.
		float dV = log10(Vfactor);

		//Checks to see if the velocity is below the critical factor values.
		if (Vfactor < MIN_V_CRUISE_FACTOR || Vfactor > MAX_V_CRUISE_FACTOR)	//NEED TO DEFINE MIN/MAX V FACTORS!
		{
			//If the Vfactor is unacceptable for stable cruise:
			//Checks to see if the dV is outside the max mission trajectory values.
			if (dV > MAX_MISSION_PARAMETER_TRAJECTORY.z)
			{
				velocity.x = MAX_MISSION_PARAMETER_TRAJECTORY.x;
				velocity.z = MAX_MISSION_PARAMETER_TRAJECTORY.z;
			}
			if (dV < -MAX_MISSION_PARAMETER_TRAJECTORY.z)
			{
				velocity.x = MAX_MISSION_PARAMETER_TRAJECTORY.x;
				velocity.z = -MAX_MISSION_PARAMETER_TRAJECTORY.z;
			}
			//Else, sets the values to the dV value and x to 1.
			else
			{
				velocity.x = 1;
				velocity.z = dV;
			}
		}
		else
		{
			//If the Vfactor is within acceptable cruise:
			dV = 0;
            velocity.x = 1;
            velocity.z = dV;
		}

		//Adds the two vectors (velocity, and temp) to get the resultant Unit vector.
		vector.x = velocity.x + temp.x;
		vector.y = velocity.y + temp.y;
		vector.z = velocity.z + temp.z;
	}

    //DEBUG: NSLog(@"%f,%f,%f,%f", vector.x, vector.y, vector.z, vector.magnitude);
	return vector;
}

float UM_Trajectory::percentError(float x, float y)
{
	return (x-y)/(x+y);
}

/*
 * End Private Method Declarations
 */


