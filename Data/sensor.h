/* 
 * File:   sensor.h
 * Author: Bas
 *
 */

#ifndef SENSOR_H
#define	SENSOR_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "../Util/DeathRow.h"

#define SENSOR_HNAMELEN 41
#define SENSOR_HUNITLEN 15
#define SENSOR_HALARMLEN 127

typedef enum{integersensor,binarysensor} sensortype;

typedef struct
{
	sensortype const type;
	char name[SENSOR_HNAMELEN];
	char unit[SENSOR_HUNITLEN];
	time_t stamp;
	unsigned int interval;
	AutoQ*delta;
} Sensor;

typedef struct
{
	Sensor base;
	bool value;
	char alarm[SENSOR_HALARMLEN];
} bSensor;

typedef struct
{
	Sensor base;
	int value;
	int lbound;
	int ubound;
	char lalarm[SENSOR_HALARMLEN];
	char ualarm[SENSOR_HALARMLEN];
} iSensor;

void SetupSensors(void);
bool binaryflux();
int integerflux(int const);
void ResetSensor(Sensor*const);
void DestroySensor(Sensor*const);

iSensor*
makeiSensor(
		char const*const name,
		char const*const unit,
		unsigned int const interval,
		int const lbound,
		int const ubound,
		char const*const lalarm,
		char const*const ualarm
		);

bSensor*
makebSensor(
		char const*const name,
		char const*const unit,
		unsigned int const interval,
		char const*const alarm
		);

#endif	/* SENSOR_H */

