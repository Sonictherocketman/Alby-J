
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

uint16_t getTempFromSensorAddress(int sensorAddress)
{
  Wire.requestFrom(sensorAddress, 2);
  byte MSB = Wire.read();
  byte LSB = Wire.read();
  int TemperatureSum = ((MSB << 8) | LSB) >> 4; 
  uint16_t cel = (uint16_t)TemperatureSum*0.0625;
  return cel;
}

uint16_t getHeightFromSensorAddress(byte sensorAddress)
{
  //Gets data from the Z-height sensor.
  pinMode(sensorAddress, INPUT);
  float pulse = pulseIn(sensorAddress, HIGH);
  float inches = pulse/147;			//147uS per inch
  uint16_t cm = (uint16_t)(0.254 * inches);
  return cm;
}

uint16_t getVelocityFromSensorAddress(byte sensorAddress)
{
  //Get the voltage reading and convert it to pressure in kPa
  float vfss = 3.5;
  int pValue = analogRead(1);
  float vOut = map(pValue,0,1023,0,5);
  float q = vOut / 5 / 0.2 - 0.2;
  
  //Convert the kPa to m/s using q = 1/2 * rho * v^2
  //Need to look up the value for rho at sea level.
  float rho = 1.225; 
  uint16_t vel = (uint16_t)sqrt(2 * q / rho) * 1000;  //In cm/s

  return vel;
}

uint16_t getAccelFromSensorAddress(byte sensorAddress)
{
  uint16_t accel = (uint16_t)analogRead(sensorAddress);
  return accel;
}


