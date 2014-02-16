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

#include "UM_MissionController.h"
#include "UM_Trajectory.h"
#include "UM_SensorDataCollection.h"
#include "UM_Actions.h"

/*
 * Begin Public Method Declarations
 */

//Override the constructor.
void UM_MissionController()
{
	//Construct
}
/*
//Updates the entire controller.
void UM_MissionController::refresh(UM_SensorDataCollection *sensorData)
{
	this->sensorData = sensorData;
	if (this->missionWasCompleted())
	{
		//Set the mode to stabilize and shut off.
	}
	else
	{
		this->getNewTrajectory();

		if (!this->trajectory->isDifferentFromTrajectory(this->oldTrajectory))
		{
			this->logger.logTrajectory(NO_NEW_TRAJECTORY, NULL);
			return;
		}
		else
		{
			this->logger.logTrajectory(NEW_TRAJECTORY, this->trajectory);
			this->updateCourseTrajectoryActions();
			bool success = this->sendActionsToWingController();
			if (!success)
			{
				this->logger.logActions(ACTIONS_FAILED_TO_EXECUTE_ERROR, this->wingActions);
				return;
			}
			this->logger.logActions(ACTIONS_DID_EXECUTE, this->wingActions);
		}
	}

}
*/

UM_Trajectory* UM_MissionController::testRefresh(UM_SensorDataCollection *sensorData)
{
	this->sensorData = sensorData;
	if (this->missionWasCompleted())
	{
		//Set the mode to stabilize and shut off.
        return this->trajectory;
	}
	else
	{
		UM_Trajectory *aTraj = new UM_Trajectory();
        aTraj = aTraj->trajectoryFromSensorData(this->sensorData, NO, YES);
        this->setTrajectory(aTraj);

		if (!this->trajectory->isDifferentFromTrajectory(this->oldTrajectory))
		{
			return this->oldTrajectory;
		}
		else
		{
            return aTraj;
		}
	}
}
/*
 * End Public Method Declarations
 *
 * Begin Private Method Declarations
 */

//Checks the status of the mission, and collects the
//data to determine if the mission has been completed.
bool UM_MissionController::missionWasCompleted()
{
	return NO;
}

//Fetches the new trajectory based on mission actions.
UM_Trajectory* UM_MissionController::getNewTrajectory()
{
	this->oldTrajectory = this->trajectory;
    UM_Trajectory *aTraj = this->trajectory->trajectoryFromSensorData(this->sensorData, this->missionCompleted, YES);
	return aTraj;
}

//Updates the actions for the current trajectory
void UM_MissionController::updateCourseTrajectoryActions()
{
	this->setWingActions(this->wingActions->actionsForTrajectory(this->trajectory));
}

//Getters and Setters

UM_Trajectory*& UM_MissionController::getOldTrajectory() {
	return oldTrajectory;
}

void UM_MissionController::setOldTrajectory(UM_Trajectory*& oldTrajectory) {
	this->oldTrajectory = oldTrajectory;
}

UM_SensorDataCollection*& UM_MissionController::getSensorData() {
	return sensorData;
}

void UM_MissionController::setSensorData(UM_SensorDataCollection*& sensorData) {
	this->sensorData = sensorData;
}

UM_Trajectory*& UM_MissionController::getTrajectory() {
	return trajectory;
}

void UM_MissionController::setTrajectory(UM_Trajectory*& trajectory) {
	this->trajectory = trajectory;
}

UM_Actions* UM_MissionController::getWingActions() {
	return wingActions;
}

void UM_MissionController::setWingActions(UM_Actions* wingActions) {
	this->wingActions = wingActions;
}
/*
UM_WingController* UM_MissionController::getWingController() {
	return wingController;
}

void UM_MissionController::setWingController(
		UM_WingController* wingController) {
	this->wingController = wingController;
}

//Sends the updated course trajectory actions to the wingController.
bool UM_MissionController::sendActionsToWingController()
{
	return this->wingController->execute(this->wingActions);
    return NO;
}
*/
/*
 * End Private Method Declarations
 */


