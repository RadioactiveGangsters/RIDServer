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

#define SENSOR_HNAMELEN 256

typedef struct
{
	char name[SENSOR_HNAMELEN];
} Sensor;

#endif	/* SENSOR_H */

