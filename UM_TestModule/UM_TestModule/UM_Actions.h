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

#ifndef UM_ACTIONS_H
#define UM_ACTIONS_H

typedef enum UM_ActionType
{
	UMActionNoAction = -1,

	UMActionLeftWingUp1 = 0,	//Determines the control surface to move and the severity of the movement.
	UMActionLeftWingUp2,		//The higher the severity, the more the surface will deflect.
	UMActionLeftWingUp3,		//Deflection angles are determined by the UMActionResult enumerator.

	UMActionLeftWingDown1 = 5,
	UMActionLeftWingDown2,
	UMActionLeftWingDown3,

	UMActionRightWingUp1 = 10,
	UMActionRightWingUp2,
	UMActionRightWingUp3,

	UMActionRightWingDown1 = 15,
	UMActionRightWingDown2,
	UMActionRightWingDown3,

	UMActionTailUp1 = 20,
	UMActionTailUp2,
	UMActionTailUp3,

	UMActionTailDown1 = 25,
	UMActionTailDown2,
	UMActionTailDown3,

	UMActionRudderRight1 = 30,
	UMActionRudderRight2,
	UMActionRudderRight3,

	UMActionRudderLeft1 = 35,
	UMActionRudderLeft2,
	UMActionRudderLeft3,

	UMActionFinishingAction = 100,
    UMActionStartingAction = 101,
}UM_ActionType;

typedef enum UM_ActionResult
{
	UMActionResult1 = 0,
	UMActionResult2,
	UMActionResult3,
}UM_ActionResult;

class UM_Trajectory;
class UM_DataLogger;
class UM_SensorDataCollection;
class UM_MissionController;
#include <vector>

class UM_Actions
{
public:
    UM_Actions();
    ~UM_Actions();
	UM_Actions* actionsForTrajectory(UM_Trajectory* trajectory);
	int countActions();
    UM_ActionType* getActionAtIndex(int index);
    int getLength();
    char* charValue(UM_ActionType);

private:
    std::vector<UM_ActionType> actions;
    std::vector<UM_ActionType>::iterator length;
	void addAction(UM_ActionType actionToAdd);
	void setActions(UM_ActionType* actions);

};

#endif
