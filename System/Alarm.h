#ifndef ALARM
#define ALARM

#include "../Data/sensor.h"
#include "../System/Log.h" 
#include "../Util/queue.h"
#include "../Monitor/connection.h"

struct Alarmdata
{
	Sensor *sn;
	int actnr;
};

void AlarmDetection(Sensor* sn);
void resetNextAlarm();
void Sensor_enqueue(Sensor* sn);
Sensor* Sensor_dequeue();
void sendAlarm(Sensor* sn, int actnr);
int getCounterActionNr(unittype sensor, bool toohigh);
#endif
