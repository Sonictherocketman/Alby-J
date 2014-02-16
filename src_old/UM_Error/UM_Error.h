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
 *  Error Manager.
 *  Author: Brian Schrader, October 2013
 *
 *  In collaboration with Bo Stump, and Tara DeRosier for
 *  UM_UAVGlider.project in accordance with San Diego State
 *  University's Aerospace Engineering Department.
 *
 */

#ifndef _UM_ERROR_H_
#define _UM_ERROR_H_

#include <stdio.h>
#include <string.h>

class UM_Error
{
public:
    inline void initWithCode_Message(int code, char message);
    int code()  {return this->errorCode;}
    char message()  {return this->errorMessage;}
    inline char description();

private:
    int errorCode;
    char errorMessage;

};

//Getters
void UM_Error::initWithCode_Message(int code, char message)
{
	this->errorCode = code;
	this->errorMessage = message;
}

char UM_Error::description()
{
        char *desc;
        sprintf(desc, "@Error: Code %d | ", this->errorCode);
        strcat (desc, (const char*)this->errorMessage);
        return *desc;
}

//Setters

#endif
