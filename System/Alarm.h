#ifndef ALARM
#define ALARM

#include "../Data/sensor.h"
#include "../System/Log.h" 
#include "../Util/queue.h"

void AlarmDetection(Sensor* sn);
void resetNextAlarm();
void Sensor_enqueue(Sensor* sn);
Sensor* Sensor_dequeue();
void sendAlarm();
#endif
