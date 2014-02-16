#include "stdlib.h"
#include "stdio.h"
#include <math.h>
#include "UM_Actions.h"
#include "UM_MissionController.h"
#include "UM_Trajectory.h"

UM_Actions::UM_Actions()
{
	//Instantiates the actions array as having a max of 1 element.
    this->length = this->actions.begin();
    this->length = this->actions.insert(this->length, UMActionStartingAction);
}


UM_Actions* UM_Actions::actionsForTrajectory(UM_Trajectory* trajectory)
{
	UM_Actions *actionClass = new UM_Actions();
	Vector vector = *trajectory->currentTrajectory;
	//Vector straightAndTrue = {1, 0, 0, 1};
	int tail = 0;
	int rudder = 0;
	int leftWing = 0;
	int rightWing = 0;

	Vector angles;
	Vector maxAngles;

	angles.x = 0.0;
	angles.y = atan(vector.y/vector.x);
	angles.z = atan(vector.z/vector.x);

	maxAngles.y = MAX_MISSION_PARAMETER_TRAJECTORY.y/3;
	maxAngles.z = MAX_MISSION_PARAMETER_TRAJECTORY.z/3;

	maxAngles.y = maxAngles.y / angles.y;
	maxAngles.z = maxAngles.z / angles.z;
    

	//Sort the Y angles
	if (abs(maxAngles.y) >= 1 && abs(maxAngles.y) < 2)
	{
		if (maxAngles.y < 0)
		{
			leftWing--;
			rightWing++;
			rudder--;
		}
		else
		{
			leftWing++;
			rightWing--;
			rudder++;
		}
	}
	else if (abs(maxAngles.y) >= 2 && abs(maxAngles.y) < 3)
	{
		if (maxAngles.y < 0)
		{
			leftWing -= 2;
			rightWing += 2;
			rudder -= 2;
		}
		else
		{
			leftWing += 2;
			rightWing -= 2;
			rudder += 2;
		}
	}
	else if (abs(maxAngles.y) >= 3)
	{
		if (maxAngles.y < 0)
		{
			leftWing -= 3;
			rightWing += 3;
			rudder -= 3;
		}
		else
		{
			leftWing += 3;
			rightWing -= 3;
			rudder += 3;
		}
	}
	//Sort the Z angles
	if (abs(maxAngles.z) >= 1 && abs(maxAngles.z) < 2)
	{
		if (maxAngles.z < 0)
		{
			tail++;
			rightWing++;
			leftWing++;
		}
		else
		{
			tail--;
			rightWing--;
			leftWing--;
		}
	}
	else if (abs(maxAngles.z) >= 2 && abs(maxAngles.z) < 3)
	{
		if (maxAngles.z < 0)
		{
			tail += 2;
			rightWing += 2;
			leftWing += 2;

		}
		else
		{
			tail -= 2;
			rightWing -= 2;
			leftWing -= 2;
		}
	}
	else if (abs(maxAngles.z) >= 3)
	{
		if (maxAngles.z < 0)
		{
			tail += 3;
			rightWing += 3;
			leftWing += 3;
		}
		else
		{
			tail -= 3;
			rightWing -= 3;
			leftWing -= 3;
		}
	}
    

	if (leftWing <= -3)	{actionClass->addAction(UMActionLeftWingDown3);}
	else if (leftWing <= -2)	{actionClass->addAction(UMActionLeftWingDown2);}
	else if (leftWing <= -1)	{actionClass->addAction(UMActionLeftWingDown1);}
	else if (leftWing < 1)		{actionClass->addAction(UMActionNoAction);}
	else if (leftWing <= 2)		{actionClass->addAction(UMActionLeftWingUp1);}
	else if (leftWing < 3)		{actionClass->addAction(UMActionLeftWingUp2);}
	else if (leftWing < 4)		{actionClass->addAction(UMActionLeftWingUp3);}

	if (rightWing <= -3)		{actionClass->addAction(UMActionRightWingDown3);}
	else if (rightWing <= -2)	{actionClass->addAction(UMActionRightWingDown2);}
	else if (rightWing <= -1)	{actionClass->addAction(UMActionRightWingDown1);}
	else if (rightWing < 1)		{actionClass->addAction(UMActionNoAction);}
	else if (rightWing <= 2)	{actionClass->addAction(UMActionRightWingUp1);}
	else if (rightWing < 3)		{actionClass->addAction(UMActionRightWingUp2);}
	else if (rightWing < 4)		{actionClass->addAction(UMActionRightWingUp3);}

	if (tail <= -3)				{actionClass->addAction(UMActionTailDown3);}
	else if (tail <= -2)		{actionClass->addAction(UMActionTailDown2);}
	else if (tail <= -1)		{actionClass->addAction(UMActionTailDown1);}
	else if (tail < 1)			{actionClass->addAction(UMActionNoAction);}
	else if (tail <= 2)			{actionClass->addAction(UMActionTailUp1);}
	else if (tail < 3)			{actionClass->addAction(UMActionTailUp2);}
	else if (tail < 4)			{actionClass->addAction(UMActionTailUp3);}

	if (rudder <= -3)			{actionClass->addAction(UMActionRudderLeft3);}
	else if (rudder <= -2)		{actionClass->addAction(UMActionRudderLeft2);}
	else if (rudder <= -1)		{actionClass->addAction(UMActionRudderLeft1);}
	else if (rudder < 1)		{actionClass->addAction(UMActionNoAction);}
	else if (rudder <= 2)		{actionClass->addAction(UMActionRudderRight1);}
	else if (rudder < 3)		{actionClass->addAction(UMActionRudderRight2);}
	else if (rudder < 4)		{actionClass->addAction(UMActionRudderRight3);}
	/* Finishing action */		actionClass->addAction(UMActionFinishingAction);
    

	return actionClass;
}

int UM_Actions::getLength()
{
    return (int)this->actions.size();
}

void UM_Actions::addAction(UM_ActionType actionToAdd)
{
    this->actions.push_back(actionToAdd);

}

//Getters and Setters

UM_ActionType* UM_Actions::getActionAtIndex(int index)
{
	return &this->actions.at(index);
}