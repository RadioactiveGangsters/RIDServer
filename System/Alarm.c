#include "Alarm.h"




void AlarmDetection(Sensor* sn){
	/**Integer Sensor*/
	if(sn->type == integersensor)
	{
		iSensor* isn = (iSensor*) sn;
		
		if(isn->value > isn->ubound){
			sendAlarm(sn);
				
		} else if (isn->value < isn->lbound){
			sendAlarm(sn);
		}
		
		
	} 
	
	/**Binary Sensor */
	else if(sn->type == binarysensor)
	{
		 bSensor* bsn = (bSensor*) sn;
		 
		if(bsn->value){		
			sendAlarm(sn);	
		}
	}

}

void sendAlarm(Sensor* sn){
	
	if(sn->type == integersensor){
		iSensor* isn = (iSensor*) sn;
		send(isn->
		
	}
		
	
}	

#ifndef FRONTEND_H

int main(void){
	
	
	return 0;
}
#endif
