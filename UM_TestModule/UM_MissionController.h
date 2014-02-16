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
 *  Controller for autonomous mission goals.
 *  Author: Brian Schrader, October 2013
 *
 *  In collaboration with Bo Stump, and Tara DeRosier for
 *  UM_UAVGlider.project in accordance with San Diego State
 *  University's Aerospace Engineering Department.
 *
 */

#ifndef UM_MISSIONCONTROLLER_H
#define UM_MISSIONCONTROLLER_H

//#define NULL 0

class UM_Trajectory;
class UM_SensorDataCollection;

/*
typedef enum BOOL
{
	NO = 0,
	YES = 1,
};
*/
 
struct Vector
{
	float x;
	float y;
	float z;
	float magnitude;
};

const struct Vector MAX_MISSION_PARAMETER_TRAJECTORY = {1, 120, 8, 0};
const float MAX_V_CRUISE_FACTOR = 1.1;
const float MIN_V_CRUISE_FACTOR = 0.9;
const float AVERAGE_CRUISE_VELOCITY = 0.5;

class UM_MissionController
{
public:
	bool missionCompleted;

	//Updates the entire controller.
	void refresh(UM_SensorDataCollection *sensorData);
    UM_Trajectory* testRefresh(UM_SensorDataCollection *sensorData);

private:
	//UM_WingController *wingController;
	UM_SensorDataCollection *sensorData;
	UM_Trajectory *oldTrajectory;
	UM_Trajectory *trajectory;
	//UM_Actions *wingActions;
	//UM_DataLogger *logger;

	//Checks the status of the mission, and collects the
	//data to determine if the mission has been completed.
	bool missionWasCompleted();

	//Fetches the new trajectory based on mission actions.
	UM_Trajectory* getNewTrajectory();

	//Updates the actions for the current trajectory
	void updateCourseTrajectoryActions();

	//Sends the updated course trajectory actions to the wingController.
	bool sendActionsToWingController();
};

#endif
