#include "Alarm.h"

void AlarmDetection(Sensor* sn)
{
	// Integer Sensor
	if(sn->type == integersensor)
	{
		iSensor* isn = (iSensor*) sn;
		
		if(isn->value > isn->ubound)
		{
			Log(LOGL_ALARM, LOGT_SERVER, "%s to HIGH\n", (sn->e)->name);
			sendAlarm(isn->ualarm);		
		} 
		else if (isn->value < isn->lbound)
		{
			sendAlarm(isn->lalarm);
			Log(LOGL_ALARM, LOGT_SERVER, "%s to LOW\n", (sn->e)->name);
		}
	} 
	
	// Binary Sensor
	else if(sn->type == binarysensor)
	{
		bSensor* bsn = (bSensor*) sn;
		 
		if(bsn->value)
		{		
			sendAlarm(bsn->alarm);	
			Log(LOGL_ALARM, LOGT_SERVER, "%s is TRUE\n", (sn->e)->name);
		}
	}
}

#ifndef FRONTEND_H
int main(void)
{	
	return 0;
}
#endif
