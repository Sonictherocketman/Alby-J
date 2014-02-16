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
 *
 *  Author: Tara DeRoier, October 2013
 *
 */

#ifndef UM_SensorDataCollection_H
#define UM_SensorDataCollection_H

class UM_Trajectory;
class UM_SensorDataCollection;
class UM_MissionController;

class UM_SensorDataCollection
{
public:

	float old_T1;
	float old_T2;
	float old_T3;
	float old_Z;
	float old_Ax;
	float old_Ay;
	float old_Az;
	float old_Gx;
	float old_Gy;
	float old_Gz;
	float old_Vel;

	float T1;
	float T2;
	float T3;
	float Z;
	float Ax;
	float Ay;
	float Az;
	float Gx;
	float Gy;
	float Gz;
	float Vel;
	bool newData;

    void init();
    
	//Gets the new data from sensors
	void refresh(float T1, float T2, float T3, float Z, float Ax, float Ay, float Az, float Gx, float Gy, float Gz, float Vel);

	//Tells it if the data is different from the previous readings
	bool hasNewData();

	//Checks if the acceleration change is significant. Set to 3% tolerance.
	bool hasSigAccelData();
};

#endif // UM_SensorDataCollection_H
