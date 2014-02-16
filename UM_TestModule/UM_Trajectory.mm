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
#include "UM_SensorDataCollection.h"
#include "stdio.h"

/*
 * Begin Public Method Declarations
 */

bool UM_Trajectory::isDifferentFromTrajectory(UM_Trajectory *trajectory)
{
	if (!trajectory || !this->currentTrajectory)	{return YES;}

	float magRes = this->percentError(trajectory->currentTrajectory->magnitude, this->currentTrajectory->magnitude);
	float xRes = this->percentError(trajectory->currentTrajectory->x, this->currentTrajectory->x);
	float yRes = this->percentError(trajectory->currentTrajectory->y, this->currentTrajectory->y);
	float zRes = this->percentError(trajectory->currentTrajectory->z, this->currentTrajectory->z);

    //Only returns true if ALL the parameters are less than 10%.
	if (magRes <= 0.1 && xRes <= 0.1 && yRes <= 0.1 && zRes <= 0.1)	{return NO;}
	return YES;
}

bool UM_Trajectory::isWithinMissionParameters()
{
	if (this->currentTrajectory->x > MAX_MISSION_PARAMETER_TRAJECTORY.x ||
			this->currentTrajectory->x < -MAX_MISSION_PARAMETER_TRAJECTORY.x)
	{return NO;}

	if (this->currentTrajectory->y > MAX_MISSION_PARAMETER_TRAJECTORY.y ||
			this->currentTrajectory->y < -MAX_MISSION_PARAMETER_TRAJECTORY.y)
	{return NO;}

	if (this->currentTrajectory->z > MAX_MISSION_PARAMETER_TRAJECTORY.z ||
			this->currentTrajectory->z < -MAX_MISSION_PARAMETER_TRAJECTORY.z)
	{return NO;}

	return YES;
}

UM_Trajectory* UM_Trajectory::trajectoryFromSensorData(UM_SensorDataCollection *sensorData, bool missionCompleted, bool safeHeight)
{
	UM_Trajectory *trajectory = new UM_Trajectory();
	if (safeHeight)
	{
		if (missionCompleted)
		{
            Vector vector = trajectory->vectorForMissionCompleted(sensorData);
            trajectory->currentTrajectory = &vector;
		}
		else
		{
            Vector vector = trajectory->vectorForMissionNotCompleted(sensorData);
            trajectory->currentTrajectory = &vector;
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

Vector UM_Trajectory::vectorForMissionCompleted(UM_SensorDataCollection *sensorData)
{
	Vector vector;

	//Depends on the Algorithm details. [TODO!]

	return vector;
}

Vector UM_Trajectory::vectorForMissionNotCompleted(UM_SensorDataCollection *sensorData)
{
	Vector vector;

	//Check to see if the accelerometer has significant data. If so,
	//the algorithm will account for the gust before the other parameters.
	if(sensorData->hasSigAccelData())
	{
		//Corroborates the acceleration data.
		Vector acceleration = {0,0,0,0};
		acceleration.x = -sensorData->Ax;
		acceleration.y = -sensorData->Ay;
		acceleration.z = -sensorData->Az;

		if (acceleration.x > MAX_MISSION_PARAMETER_TRAJECTORY.x)	{acceleration.x = MAX_MISSION_PARAMETER_TRAJECTORY.x;}
		if (acceleration.x < -MAX_MISSION_PARAMETER_TRAJECTORY.x)	{acceleration.x = -MAX_MISSION_PARAMETER_TRAJECTORY.x;}

		if (acceleration.y > MAX_MISSION_PARAMETER_TRAJECTORY.y)	{acceleration.y = MAX_MISSION_PARAMETER_TRAJECTORY.y;}
		if (acceleration.y < -MAX_MISSION_PARAMETER_TRAJECTORY.y)	{acceleration.y = -MAX_MISSION_PARAMETER_TRAJECTORY.y;}

		if (acceleration.z > MAX_MISSION_PARAMETER_TRAJECTORY.z)	{acceleration.z = MAX_MISSION_PARAMETER_TRAJECTORY.z;}
		if (acceleration.z < -MAX_MISSION_PARAMETER_TRAJECTORY.z)	{acceleration.z = -MAX_MISSION_PARAMETER_TRAJECTORY.z;}

		vector = acceleration;
    }
	//No significant acceleration was detected. Default to Velocity and temp checks
	else
	{
		Vector velocity = {0,0,0,0};
		Vector temp = {0,0,0,0};

		Vector T1 = {0,0,0,0};
		Vector T2 = {0,0,0,0};
		Vector T3 = {0,0,0,0};

		T1.y = sensorData->T1;
		T2.y = sensorData->T2;
		T3.y = sensorData->T3;

		Vector temps[] = {T1, T2, T3};
		float smallestValue = 100000;

		//Sorts the array
		for (int i = 0; i < sizeof(temps)/sizeof(temps[0]); i++)
		{
			//Not sure if works? FIX [POSSIBLE BUG]
			temps[i].x = 1;
			if (smallestValue > temps[i].y)	{smallestValue = temps[i].y;}

		}
		//Subtracts the smallest value and eliminates it from calculation
		for (int i = 0; i < sizeof(temps)/sizeof(temps[0]); i++)
		{
			temps[i].y -= smallestValue;
		}

		//Adds the vectors together
		for (int i = 0; i < sizeof(temps)/sizeof(temps[0]); i++)
		{
			temp.y += temps[i].y;
		}

		//The case for if the center is the weakest but the two wings
		//have ~ equal values.
		if ((this->percentError(T1.y, T2.y) <= 0.1) && (T2.y == smallestValue))
		{
			//If the lateral trajectories are approx equal, and the forward trajectory is the smallest
			//of the three, then the plane will bias right.
			temp.y = MAX_MISSION_PARAMETER_TRAJECTORY.y;
		}

		//Convert the T vector to a Unit Vector.
		temp.magnitude = sqrt(pow(temp.y, 2) + temp.x);
		temp.x = temp.x/temp.magnitude;
		temp.y = temp.y/temp.magnitude;

		//Gets the velocity factor.
		float Vcurrent = sensorData->Vel;
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
		//Convert the V vector to a Unit Vector.
		velocity.magnitude = sqrt(pow(velocity.z, 2) + velocity.x);
		velocity.x = velocity.x/velocity.magnitude;
		velocity.z = velocity.z/velocity.magnitude;
		//free(Vcurrent, Vfactor, dV);

		//Adds the two vectors (velocity, and temp) to get the resultant Unit vector.
		vector.x = velocity.x + temp.x;
		vector.y = velocity.y + temp.y;
		vector.z = velocity.z + temp.z;
		//delete(temp, velocity);
	}

    NSLog(@"%f,%f,%f,%f", vector.x, vector.y, vector.z, vector.magnitude);
	return vector;
}

float UM_Trajectory::percentError(float x, float y)
{
	return (x-y)/(x+y);
}

/*
 * End Private Method Declarations
 */




