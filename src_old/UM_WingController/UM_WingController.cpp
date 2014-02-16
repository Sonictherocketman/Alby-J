#include "UM_WingController.h"
#include <bool.h>

bool UM_WingController::execute(UM_Actions* actions, UM_Error *error)
{
		int len = actions->getLength();

		//Initialize pins and sensors

		//Command stuff
		for (int i = 0; i < len; i++) {
			UM_ActionType action = *actions->getActionAtIndex(i);
			UM_ActionResult magnitude;
			switch(action) {
			//Insert functions in appropriate places.
				case UMActionStartingAction:
				case UMActionFinishingAction:
					break;
				case UMActionTailUp1:
				case UMActionTailUp2:
				case UMActionTailUp3:
					break;
				case UMActionTailDown1:
				case UMActionTailDown2:
				case UMActionTailDown3:
					break;
				case UMActionLeftWingUp1:
				case UMActionLeftWingUp2:
				case UMActionLeftWingUp3:
					break;
				case UMActionLeftWingDown1:
				case UMActionLeftWingDown2:
				case UMActionLeftWingDown3:
					break;
				case UMActionRightWingUp1:
				case UMActionRightWingUp2:
				case UMActionRightWingUp3:
					break;
				case UMActionRightWingDown1:
				case UMActionRightWingDown2:
				case UMActionRightWingDown3:
					break;
				case UMActionRudderRight1:
				case UMActionRudderRight2:
				case UMActionRudderRight3:
					break;
				case UMActionRudderLeft1:
				case UMActionRudderLeft2:
				case UMActionRudderLeft3:
					break;
			}
		}
}
