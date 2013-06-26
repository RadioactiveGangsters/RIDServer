#include "Alarm.h"


void AlarmDetection(Sensor* sn)
{
			
	if(!sn)return;
	// Integer Sensor
	if(sn->type == integersensor)
	{
		iSensor* isn = (iSensor*) sn;
		
		if(isn->value > isn->ubound)
		{
			Log(LOGL_ALARM, LOGT_SERVER, "%s to HIGH\n", (sn->name));
			//sendAlarm(isn->ualarm);		
		} 
		else if (isn->value < isn->lbound)
		{
			//sendAlarm(isn->lalarm);
			Log(LOGL_ALARM, LOGT_SERVER, "%s to LOW\n", (sn->name));
		}
	} 
	
	// Binary Sensor 
	else if(sn->type == binarysensor)
	{
		bSensor* bsn = (bSensor*) sn;
		 
		if(bsn->value)
		{		
			//sendAlarm(bsn->alarm);	
			Log(LOGL_ALARM, LOGT_SERVER, "%s is TRUE\n", (sn->name));
		}
	}
}

//void warn(Client*c)
//{
	//TODO: makepkt


	//c->sendpacket(pp);

	//TODO: freepkt
//}

void SendAlarm(Sensor*fb)
{
	if(!fb)return;
//	forClient(&warn);
}


