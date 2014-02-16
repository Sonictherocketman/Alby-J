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


void initServos()
{
  lw.attach(SRV_LWG);
  rw.attach(SRV_RWG);
  tl.attach(SRV_TL);
  rd.attach(SRV_RDR);
  
  lw.write(90);  //Moves all the servos to the halfway point.
  rw.write(90);
  tl.write(90);
  rd.write(90);
  
  Serial3.println("Servos attached.");
}

void levelOut()
{
  zeroServo(lw);
  zeroServo(rw);
  zeroServo(tl);
  zeroServo(rd);
}

void executeActions()
{
   for(int i = 0; i < 6; i++)
  {
    uint16_t action = *(actions + i);
    switch(action){
      case UMActionNoAction:
      case UMActionFinishingAction:
      case UMActionStartingAction:
        break;
      case UMActionLeftWingUp1:    	//Determines the control surface to move and the severity of the movement.
        lw.write(MIN_AILERON_DEFLECTION);
        break;
      case UMActionLeftWingUp2:		//The higher the severity, the more the surface will deflect.
        lw.write(MID_AILERON_DEFLECTION);
        break;        
      case UMActionLeftWingUp3:		//Deflection angles are determined by the UMActionResult enumerator.
        lw.write(MAX_AILERON_DEFLECTION);
        break;

      case UMActionLeftWingDown1:
        lw.write(-MIN_AILERON_DEFLECTION);
        break;
      case UMActionLeftWingDown2:
        lw.write(-MID_AILERON_DEFLECTION);
        break;
      case UMActionLeftWingDown3:
        lw.write(-MAX_AILERON_DEFLECTION);
        break;

      case UMActionRightWingUp1:
        rw.write(MIN_AILERON_DEFLECTION);
        break;
      case UMActionRightWingUp2:
        rw.write(MID_AILERON_DEFLECTION);
        break;
      case UMActionRightWingUp3:
        rw.write(MAX_AILERON_DEFLECTION);
        break;

      case UMActionRightWingDown1:
        rw.write(-MIN_AILERON_DEFLECTION);
        break;
      case UMActionRightWingDown2:
        rw.write(-MID_AILERON_DEFLECTION);
        break;
      case UMActionRightWingDown3:
        rw.write(-MAX_AILERON_DEFLECTION);
        break;

      case UMActionTailUp1:
        tl.write(MIN_ELEVATOR_DEFLECTION);
        break;
      case UMActionTailUp2:
        tl.write(MID_ELEVATOR_DEFLECTION);
        break;
      case UMActionTailUp3:
        tl.write(MAX_ELEVATOR_DEFLECTION);
        break;

      case UMActionTailDown1:
        tl.write(-MIN_ELEVATOR_DEFLECTION);
        break;
      case UMActionTailDown2:
        tl.write(-MID_ELEVATOR_DEFLECTION);
        break;
      case UMActionTailDown3:
        tl.write(-MAX_ELEVATOR_DEFLECTION);
        break;

      case UMActionRudderRight1:
        rd.write(MIN_RUDDER_DEFLECTION);
        break;
      case UMActionRudderRight2:
        rd.write(MID_RUDDER_DEFLECTION);
        break;
      case UMActionRudderRight3:
        rd.write(MAX_RUDDER_DEFLECTION);
        break;

      case UMActionRudderLeft1:
        rd.write(-MIN_RUDDER_DEFLECTION);
        break;
      case UMActionRudderLeft2:
        rd.write(-MID_RUDDER_DEFLECTION);
        break;
      case UMActionRudderLeft3:
        rd.write(-MAX_RUDDER_DEFLECTION);
        break;
        
      case UMActionLevelOutAction:
        zeroServo(lw);
        zeroServo(rw);
        zeroServo(tl);
        zeroServo(rd);
        break;
    }
  } 
 UMLog("Actions Executed."); 
}

//Accepts a DELTA position. This position is relative to the previous postion.
static void moveServo(Servo servo, uint16_t dp)
{
  int pos = servo.read();
  if(dp > 0)
  {
    for(pos; pos < (dp + pos); pos++)     // goes from 0 degrees to 180 degrees 
    {                                  // in steps of 1 degree 
      servo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(15);                       // waits 15ms for the servo to reach the position 
    }
  }
  if(dp < 0)
  {
   for(pos; pos > (pos - dp); pos--)     // goes from 0 degrees to 180 degrees 
    {                                  // in steps of 1 degree 
      servo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(15);                       // waits 15ms for the servo to reach the position 
    }  
  }
  else
  {
    return;
  }
}

static void zeroServo(Servo servo)
{
  servo.write(0);
}


