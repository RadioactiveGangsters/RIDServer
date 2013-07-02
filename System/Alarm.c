#include "Alarm.h"

static queue q={.size=0}; 
void AlarmDetection(Sensor* sn)
{
			
	if(!sn)return;
	// Integer Sensor
	if(sn->type == integersensor)
	{
		iSensor* isn = (iSensor*) sn;
		
		if(isn->value > isn->ubound)
		{
			Log(LOGL_ALARM, LOGT_SERVER, "%s to HIGH", (sn->name));
			SensorQueue(sn);
			//sendAlarm(isn->ualarm);		
		} 
		else if (isn->value < isn->lbound)
		{
			//sendAlarm(isn->lalarm);
			Log(LOGL_ALARM, LOGT_SERVER, "%s to LOW", (sn->name));
			SensorQueue(sn);
		}
	} 
	
	// Binary Sensor 
	else if(sn->type == binarysensor)
	{
		bSensor* bsn = (bSensor*) sn;
		 
		if(bsn->value)
		{		
			//sendAlarm(bsn->alarm);	
			Log(LOGL_ALARM, LOGT_SERVER, "%s is TRUE", (sn->name));
			SensorQueue(sn);
		}
	}
}

void SensorQueue(Sensor* sn)
{	
	if(!q.size)
	{
		enqueue(&q, sn);
	}
	else
	{
		init_queue(&q);
		enqueue(&q, sn);
	}	
}

Sensor* Sensor_dequeue(queue *q)
{
	if(q.size) return *dequeue(&q);
	else return NULL;
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


