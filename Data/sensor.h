/* 
 * File:   sensor.h
 * Author: Bas
 *
 */

#ifndef SENSOR_H
#define	SENSOR_H

#include <time.h>
#include <stdbool.h>
#include "../Util/DeathRow.h"

#define SENSOR_HNAMELEN 41
#define SENSOR_HUNITLEN 15
#define SENSOR_HALARMLEN 127

typedef enum{unit_undefined,unit_temperature,unit_flow,unit_pressure,unit_fullness,unit_radiation} unittype;
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
int getSensorNumberOf(char const*const name);

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


#ifndef S_SPLINT_S
unittype unitbystring(char const[static 2]);
#else
unittype unitbystring(char const*const);
#endif
#endif	/* SENSOR_H */

