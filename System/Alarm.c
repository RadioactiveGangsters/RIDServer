#include "Alarm.h"




void AlarmDetection(Sensor* sn){
	/**Integer Sensor*/
	if(sn->type == integersensor)
	{
		iSensor* isn = (iSensor*) sn;
		
		if(isn->value > isn->ubound){
			sendAlarm(isn->ualarm);
				
		} else if (isn->value < isn->lbound){
			sendAlarm(isn->lalarm);
		}
		
		
	} 
	
	/**Binary Sensor */
	else if(sn->type == binarysensor)
	{
		 bSensor* bsn = (bSensor*) sn;
		 
		if(bsn->value){		
			sendAlarm(bsn->alarm);	
		}
	}

}


#ifndef FRONTEND_H

int main(void){
	
	
	return 0;
}
#endif
