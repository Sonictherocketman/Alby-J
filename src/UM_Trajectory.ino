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

void trajectoryFromSensorData()
{
  //Checks for extreme angle of turn cases.
  //Exits if these conditions are met.
  if (unsafeTurnCondition())  
  {
    *(trajectory + 0) = 0;
    *(trajectory + 1) = 0;
    *(trajectory + 2) = 0;
    return;
  }
  
  UM_Vector vector;
  //Check to see if the accelerometer has significant data. If so,
  //the algorithm will account for the gust before the other parameters.
  if(hasSigAccelData())
  {
    //Corroborates the acceleration data.
    UMVector acceleration = {
      0,0,0,0    };
    acceleration.x = -*(sensorData + 5);
    acceleration.y = -*(sensorData + 7);
    acceleration.z = -*(sensorData + 6);

    if (acceleration.x > MAX_MISSION_PARAMETER_TRAJECTORY.x)	{
      acceleration.x = MAX_MISSION_PARAMETER_TRAJECTORY.x;
    }
    if (acceleration.x < -MAX_MISSION_PARAMETER_TRAJECTORY.x)	{
      acceleration.x = -MAX_MISSION_PARAMETER_TRAJECTORY.x;
    }

    if (acceleration.y > MAX_MISSION_PARAMETER_TRAJECTORY.y)	{
      acceleration.y = MAX_MISSION_PARAMETER_TRAJECTORY.y;
    }
    if (acceleration.y < -MAX_MISSION_PARAMETER_TRAJECTORY.y)	{
      acceleration.y = -MAX_MISSION_PARAMETER_TRAJECTORY.y;
    }

    if (acceleration.z > MAX_MISSION_PARAMETER_TRAJECTORY.z)	{
      acceleration.z = MAX_MISSION_PARAMETER_TRAJECTORY.z;
    }
    if (acceleration.z < -MAX_MISSION_PARAMETER_TRAJECTORY.z)	{
      acceleration.z = -MAX_MISSION_PARAMETER_TRAJECTORY.z;
    }

    vector = acceleration;
    free(&acceleration);
  }
  //No significant acceleration was detected. Default to Velocity and temp checks
  else
  {
    UM_Vector velocity = {
      0,0,0,0    };
    UM_Vector temp = {
      0,0,0,0    };

    UM_Vector T1 = {
      0,0,0,0    };
    UM_Vector T2 = {
      0,0,0,0    };
    UM_Vector T3 = {
      0,0,0,0    };

    T1.y = *(sensorData + 0);
    T2.y = *(sensorData + 1);
    T3.y = *(sensorData + 2);

    UM_Vector temps[] = {
      T1, T2, T3    };
    UM_Vector largest = {
      0, 0, 0    };
    int index;

    //Finds the largest temp
    for (int i = 0; i < sizeof(temps)/sizeof(temps[0]); i++)
    {
      //Not sure if works? FIX [POSSIBLE BUG]
      temps[i].x = 1;
      if (largest.y < temps[i].y)	{
        largest = temps[i]; 
        index = i;
      }
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
    float Vcurrent = *(sensorData + 4);
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
  *(trajectory + 0) = vector.x;
  *(trajectory + 1) = vector.y;
  *(trajectory + 2) = vector.z;
}


static bool hasSigAccelData() 
{
  float result = 0;
  int changes = 0;

  //Can change the tolerance of the acceleration change here
  result = ACCEL_DATA_SIGNIFICANCE_FACTOR* *(oldSensorData + 5);

  if ((*(oldSensorData + 5) + result) < *(sensorData + 5))	{
    changes++;
  }
  if ((*(oldSensorData + 5) - result) > *(sensorData + 5))	{
    changes++;
  }

  if ((*(oldSensorData + 6) + result) < *(sensorData + 6))	{
    changes++;
  }
  if ((*(oldSensorData + 6) - result) > *(sensorData + 6))	{
    changes++;
  }

  if ((*(oldSensorData + 7) + result) < *(sensorData + 7))	{
    changes++;
  }
  if ((*(oldSensorData + 7) - result) > *(sensorData + 7))	{
    changes++;
  }

  if (changes != 0)	{
    return TRUE;
  }
  return FALSE;
}

static bool unsafeTurnCondition()
{
  return FALSE;   
  
}

