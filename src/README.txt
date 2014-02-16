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
 
 /*
 * The algorithm depends on 3 crucial variables for its data transformation.
 * These variables are:
 *   - uint16_t sensorData[]  :  A collection of all of the data from the sensors. 
 *                      It is laid out as follows.
 *              T1  :  Temperature detected (in celsius) at the left wing sensor.
 *              T2  :  Temperature detected (in celsius) at the nose sensor.
 *              T3  :  Temperature detected (in celsius) at the right wing sensor.
 *              Z   :  The vertical height (in centimeters) of the aircraft above the ground.
 *              Vel :  The aircraft's current flight speed (in meters per second).
 *              Ax  :  Acceleration detected in the fore/aft axis.
 *              Ay  :  Acceleration detected in the spanwise axis. 
 *              Az  :  Acceleration detected in the plane perpendicular to the span/fusi plane.
 *
 *  - uint16_t trajectory[]  :  The trajectory vector obtained from analysis of the raw sensor data.
 *              
 *              x   :  The fore/aft magnitude of the new trajectory vector.
 *              y   :  The magnitude of the trajectory vector in the spanwise axis.
 *              z   :  The longitudinal magnitude of the trajectory vector.
 *
 *  - int actions[]          :  The set of actions needed to obtain the trajectory above.
 *
 *              1   :  The default starting action. This must be present in the array for the validity of the actions to be proven.
 *              2   :  The action and magnitude to be taken by the left wing (as seen from top, nose out).
 *              3   :  The action and magnitude to be taken by the right wing (as seen from top, nose out).
 *              4   :  The action and magnitude to be taken by the tail elevators..
 *              5   :  The action and magnitude to be taken by the rudder.
 *              6   :  The default finishing action.
 *
 *
 *
 */  
