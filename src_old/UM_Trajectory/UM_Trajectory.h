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
 *  In collaboration with Bo Stump, and Tara DeRosier for
 *  UM_UAVGlider.project in accordance with San Diego State
 *  University's Aerospace Engineering Department.
 *
 */

#ifndef UM_TRAJECTORY_H
#define UM_TRAJECTORY_H

#include <UM_SensorDataCollection.h>
#include <UM_MissionController.h>
#include <UM_Error.h>
#include <UM_MissionController.h>

class UM_Trajectory
{
public:

	struct UMVector *currentTrajectory;
	
	void init();

	//Compares the current trajectory to the one provided. Looks for over a 5% deviation.
	bool isDifferentFromTrajectory(UM_Trajectory *trajectory);

	//Compares the current trajectory with the mission parameters.
	bool isWithinMissionParameters();

	UM_Trajectory* trajectoryFromSensorData(UM_SensorDataCollection *sensorData, bool missionCompleted, bool heightSafe, UM_Error *error);

private:

	struct UMVector vectorForMissionCompleted(UM_SensorDataCollection *sensorData);
	struct UMVector vectorForMissionNotCompleted(UM_SensorDataCollection *sensorData); //[TODO]
	float percentError(float x, float y);
};

#endif
