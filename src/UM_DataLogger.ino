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
 
 void UMLog(char message[50])
 {
   Serial3.print(",,,,,,,,,,,,,,,,,,");
   Serial3.print(*message);
   Serial3.print("\n");
 }
 
 void UMDataLog()
 {
  Serial3.print(",");
  //Log the sensor data.
  for (int i = 0; i < 8; i++)
  {
    Serial3.print(*(sensorData + i));
    Serial3.print(", ");
  }
  //Log the trajectory data.
  for (int i = 0; i < 4; i++)
  {
    Serial3.print(*(trajectory + i));  
    Serial3.print(", ");
  }
  //Log the actions data.
  for (int i = 0; i < 6; i++)
  {
    Serial3.print(*(actions + i));
    Serial3.print(", ");
  }
  Serial3.print(timer);
  Serial3.print(", /n");
 }
