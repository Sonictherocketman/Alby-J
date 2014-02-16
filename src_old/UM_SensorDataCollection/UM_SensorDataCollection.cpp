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
   along with this program.  If FALSEt, see <http://www.gnu.org/licenses/>.
 */

/*
 *
 *  Author: Tara DeRosier, October 2013
 *
 */

#include <UM_SensorDataCollection.h>
#include <bool.h>
#include <Wire.h>
#include <Serial.h>

void UM_SensorDataCollection()
{
	return;
}

void UM_SensorDataCollection::init()
{
	//Prepare the values of the data collection.
	*this->old_T1 = 0;
	*this->old_T2 = 0;
	*this->old_T3 = 0;
	*this->old_Z = 0;
	*this->old_Ax = 0;
	*this->old_Ay = 0;
	*this->old_Az = 0;
	*this->old_Gx = 0;
	*this->old_Gy = 0;
	*this->old_Gz = 0;
	*this->old_Vel = 0;

	*this->T1 = 0;
	*this->T2 = 0;
	*this->T3 = 0;
	*this->Z = 0;
	*this->Ax = 0;
	*this->Ay = 0;
	*this->Az = 0;
	*this->Gx = 0;
	*this->Gy = 0;
	*this->Gz = 0;
	*this->Vel = 0;
}

void UM_SensorDataCollection::refresh(float *Z, float *Vel, float *Ax, float *Ay, float *Az, float *T1, float *T2, float *T3,  float *Gx, float *Gy, float *Gz)
{
	this->old_Z = this->Z;

	this->old_Vel = this->Vel;

	this->old_Ax = this->Ax;
	this->old_Ay = this->Ay;
	this->old_Az = this->Az;

	this->old_T1 = this->T1;
	this->old_T2 = this->T2;
	this->old_T3 = this->T3;

	this->old_Gx = this->Gx;
	this->old_Gy = this->Gy;
	this->old_Gz = this->Gz;

	this->Z = Z;

	this->Vel = Vel;

	this->Ax = Ax;
	this->Ay = Ay;
	this->Az = Az;

	this->T1 = T1;
	this->T2 = T2;
	this->T3 = T3;

	this->Gx = Gx;
	this->Gy = Gy;
	this->Gz = Gx;

	this->newData = TRUE;

	if (this->old_Z == this->Z)	{this->newData = FALSE;}

	if (this->old_Vel == this->Vel)	{this->newData = FALSE;}

	if (this->old_Ax == this->Ax)	{this->newData = FALSE;}
	if (this->old_Ay == this->Ay)	{this->newData = FALSE;}
	if (this->old_Az == this->Az)	{this->newData = FALSE;}

	if (this->old_T1 == this->T1)	{this->newData = FALSE;}
	if (this->old_T2 == this->T2)	{this->newData = FALSE;}
	if (this->old_T3 == this->T3)	{this->newData = FALSE;}

	if (this->old_Gx == this->Gx)	{this->newData = FALSE;}
	if (this->old_Gy == this->Gy)	{this->newData = FALSE;}
	if (this->old_Gz == this->Gz)	{this->newData = FALSE;}
}

bool UM_SensorDataCollection::hasSigAccelData()
{
	float result = 0;
	int changes = 0;

	//Can change the tolerance of the acceleration change here
	result = .03* *this->old_Ax;

	if ((*this->old_Ax+result) < *this->Ax)	{changes++;}
	if ((*this->old_Ax-result) > *this->Ax)	{changes++;}

	if ((*this->old_Ay+result) < *this->Ay)	{changes++;}
	if ((*this->old_Ay-result) > *this->Ay)	{changes++;}

	if ((*this->old_Az+result) < *this->Az)	{changes++;}
	if ((*this->old_Az-result) > *this->Az)	{changes++;}

	if (changes != 0)	{return TRUE;}
	return FALSE;
}

bool UM_SensorDataCollection::hasNewData()
{
	return this->newData;
}

void UM_SensorDataCollection::refresh()
{	
	//Gets data from the temperature sensors.
	return;
}

