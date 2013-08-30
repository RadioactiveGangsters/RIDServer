#include "Alarm.h"

static queue q={.array=NULL,.first=-1,.last=-1,.count=0,.size=0}; 

void AlarmDetection(Sensor* sn)
{		
	if(!sn)return;

	// Integer Sensor
	if(sn->type == integersensor)
	{
		iSensor* isn = (iSensor*) sn;
		
		if(isn->value > isn->ubound)
		{
			sendAlarm(sn, getCounterActionNr(unitbystring(sn->name), true));
			Sensor_enqueue(sn);
			Log(LOGT_SERVER, LOGL_ALARM, "%s: %s (%d > %d)", sn->name ,isn->ualarm, isn->value, isn->ubound);
		} 
		else if (isn->value < isn->lbound)
		{
			sendAlarm(sn, getCounterActionNr(unitbystring(sn->name), false));
			Sensor_enqueue(sn);
			Log(LOGT_SERVER, LOGL_ALARM, "%s: %s (%d < %d)", sn->name ,isn->lalarm, isn->value, isn->lbound);
		}
	} 
	
	// Binary Sensor 
	else if(sn->type == binarysensor)
	{
		bSensor* bsn = (bSensor*) sn;
		 
		if(bsn->value)
		{
			sendAlarm(sn, getCounterActionNr(unitbystring(sn->name), true));
			Sensor_enqueue(sn);
			Log(LOGT_SERVER, LOGL_ALARM, "%s: %s", sn->name ,bsn->alarm);
		}
	}
}

void resetNextAlarm()
{
	Sensor* sn = Sensor_dequeue();
	if(sn)
	{
		ResetSensor(sn);
		Log(LOGT_SERVER, LOGL_DEBUG, "Alarm %s has been reset.", (sn->name));
	}
	else Log(LOGT_SERVER, LOGL_DEBUG, "There are no more alarms to reset.");
}

void Sensor_enqueue(Sensor* sn)
{	
	if(!q.size)
	{
		init_queue(&q);
		enqueue(&q, sn);
	}
	else
	{
		enqueue(&q, sn);
	}	
}

Sensor* Sensor_dequeue()
{
	if(q.size) return dequeue(&q);
	else return NULL;
}

void sendAlarm(Sensor* sn, int actnr)
{
	if(!sn)return;
	struct Alarmdata newalarm =
	{
		.sn=sn,
		.actnr=actnr,
	};
	forClients(&mysender, &newalarm);
}

int getCounterActionNr(unittype sensor, bool toohigh)
{
	return ((sensor-1)*2 + toohigh?1:2);
}


