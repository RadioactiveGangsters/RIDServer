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
	int startvalue;
	int value;
	int lbound;
	int ubound;
	char lalarm[SENSOR_HALARMLEN];
	char ualarm[SENSOR_HALARMLEN];
	bool lboundcross;
	bool uboundcross;
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
		int const startvalue,
		int const lbound,
		int const ubound,
		char const*const lalarm,
		char const*const ualarm,
		bool const lboundcross,
		bool const uboundcross
		);

bSensor*
makebSensor(
		char const*const name,
		char const*const unit,
		char const*const alarm
		);

#endif	/* SENSOR_H */

