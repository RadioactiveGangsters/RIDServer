#ifndef ALARM
#define ALARM

#include "../Data/sensor.h"
#include "../System/Log.h" 
#include "../Util/queue.h"

void AlarmDetection(Sensor* sn);
void sendAlarm();
Sensor* Sensor_dequeue(queue *q);

#endif
