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
  
  UMActionLevelOutAction = 99,

  UMActionFinishingAction = 100,
  UMActionStartingAction = 101,
}
UM_ActionType;

void actionsForTrajectory()
{
  struct UMVector *vector;
  vector->x = *(trajectory + 0);
  vector->y = *(trajectory + 1);
  vector->z = *(trajectory + 2);

  //Vector straightAndTrue = {1, 0, 0, 1};
  int tail = 0;
  int rudder = 0;
  int leftWing = 0;
  int rightWing = 0;

  UMVector angles;
  UMVector maxAngles;

  angles.x = 0.0;
  angles.y = atan(vector->y/vector->x);
  angles.z = atan(vector->z/vector->x);

  maxAngles.y = MAX_MISSION_PARAMETER_TRAJECTORY.y/3;
  maxAngles.z = MAX_MISSION_PARAMETER_TRAJECTORY.z/3;

  maxAngles.y = maxAngles.y / angles.y;
  maxAngles.z = maxAngles.z / angles.z;


  //Sort the Y angles
  if (angles.z == 0 && angles.y == 0)
  {
    *(actions + 0) = UMActionStartingAction;
    *(actions + 1) = UMActionLevelOutAction;
    *(actions + 2) = UMActionLevelOutAction;
    *(actions + 3) = UMActionLevelOutAction;
    *(actions + 4) = UMActionLevelOutAction;
    *(actions + 5) = UMActionFinishingAction;
    return;
  }
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


  /* Starting Action */         *(actions + 0) = UMActionStartingAction;
  if (leftWing <= -3)	        {*(actions + 1) = UMActionLeftWingDown3;}
  else if (leftWing <= -2)	{*(actions + 1) = UMActionLeftWingDown2;}
  else if (leftWing <= -1)	{*(actions + 1) = UMActionLeftWingDown1;}
  else if (leftWing < 1)	{*(actions + 1) = UMActionNoAction;}
  else if (leftWing <= 2)	{*(actions + 1) = UMActionLeftWingUp1;}
  else if (leftWing < 3)	{*(actions + 1) = UMActionLeftWingUp2;}
  else if (leftWing < 4)	{*(actions + 1) = UMActionLeftWingUp3;}

  if (rightWing <= -3)		{*(actions + 2) = UMActionRightWingDown3;}
  else if (rightWing <= -2)	{*(actions + 2) = UMActionRightWingDown2;}
  else if (rightWing <= -1)	{*(actions + 2) = UMActionRightWingDown1;}
  else if (rightWing < 1)	{*(actions + 2) = UMActionNoAction;}
  else if (rightWing <= 2)	{*(actions + 2) = UMActionRightWingUp1;}
  else if (rightWing < 3)	{*(actions + 2) = UMActionRightWingUp2;}
  else if (rightWing < 4)	{*(actions + 2) = UMActionRightWingUp3;}

  if (tail <= -3)		{*(actions + 3) = UMActionTailDown3;}
  else if (tail <= -2)		{*(actions + 3) = UMActionTailDown2;}
  else if (tail <= -1)		{*(actions + 3) = UMActionTailDown1;}
  else if (tail < 1)		{*(actions + 3) = UMActionNoAction;}
  else if (tail <= 2)		{*(actions + 3) = UMActionTailUp1;}
  else if (tail < 3)		{*(actions + 3) = UMActionTailUp2;}
  else if (tail < 4)		{*(actions + 3) = UMActionTailUp3;}

  if (rudder <= -3)		{*(actions + 4) = UMActionRudderLeft3;}
  else if (rudder <= -2)	{*(actions + 4) = UMActionRudderLeft2;}
  else if (rudder <= -1)	{*(actions + 4) = UMActionRudderLeft1;}
  else if (rudder < 1)		{*(actions + 4) = UMActionNoAction;}
  else if (rudder <= 2)		{*(actions + 4) = UMActionRudderRight1;}
  else if (rudder < 3)		{*(actions + 4) = UMActionRudderRight2;}
  else if (rudder < 4)		{*(actions + 4) = UMActionRudderRight3;}
  /* Finishing action */        *(actions + 5) = UMActionFinishingAction;
}

