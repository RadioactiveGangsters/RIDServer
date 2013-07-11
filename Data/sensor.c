#include"sensor.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

static unsigned int _sensseed;

#ifndef S_SPLINT_S
unittype unitbystring(char const id[static 2])
#else
unittype unitbystring(char const*const id)
#endif
{
	if(!id)return unit_undefined;

	switch(id[0])
	{
		case 'T':
			return unit_temperature;
		case 'P':
			return unit_pressure;
		case 'R':
			return unit_radiation;
		case 'F':
			switch(id[1])
			{
				case 'l':
					return unit_flow;
				case 'u':
					return unit_fullness;
			}
	}

	return unit_undefined;
}

/* Generates a flux value for binary sensors. 
 * This value is randomly generated between the 
 * reed 1 and the -10000. The opportunity for 
 * '1' is 1 to 10000.
 */
bool binaryflux()
{
	srand(_sensseed=(unsigned)rand());
	return ((!(rand()%225) && !(rand()%225)));
}

/* Generates a flux value for integer sensors.
 * This value will be used to increase or decrease
 * the original value. The random generated value
 * will be between -1 and 2.
 */
int integerflux(int pvalue)
{
	int flux;
	srand(_sensseed=(unsigned)rand());
	flux = (rand()%3)-1;

	if(pvalue == 0)
	{
		if(flux < 0) flux = abs(flux);
		pvalue += flux;
	}
	else if((pvalue+flux)<0) pvalue = 0;
	else pvalue += flux;

	return pvalue;
}

void SetupSensors(void)
{
	srand((unsigned int)time(NULL));
	_sensseed=(unsigned)rand();    
}

void ResetSensor(Sensor*const s)
{
	if(s->type == binarysensor) ((bSensor*)s)->value = 0;
	else ((iSensor*)s)->value = ((((iSensor*)s)->lbound)+(((iSensor*)s)->ubound))/2;
}

void DestroySensor(Sensor*const s)
{
	if(!s)return;
	DestroyAutoQ(s->delta);
	free(s);
}

bSensor* makebSensor(char const*const name,char const*const unit,unsigned int const interval,char const*const alarm)
{
	Sensor base=
	{
		.name="genericb",
		.unit="on/off",
		.type=binarysensor,
		.stamp=time(0),
		.interval=interval,
		.delta=NULL,
	};

	bSensor s=
	{
		.base=base,
		.value=0,
		.alarm="Alarm!",
	};

	strncpy(s.base.name, name, sizeof(char)*SENSOR_HNAMELEN);
	strncpy(s.base.unit, unit, sizeof(char)*SENSOR_HUNITLEN);
	strncpy(s.alarm, alarm, sizeof(char)*SENSOR_HALARMLEN);

	{
		bSensor*const p=malloc(sizeof*p);

		if(!p)return p;

		memcpy(p, &s, sizeof*p);
		return p;
	}
}

iSensor* makeiSensor(char const*const name,char const*const unit,unsigned int const interval,int const lbound,int const ubound,char const*const lalarm,char const*const ualarm)
{
	Sensor base=
	{
		.name="generici",
		.unit="generici",
		.type=integersensor,
		.stamp=time(0),
		.interval=interval,
		.delta=NULL,
	};

	iSensor s=
	{
		.base=base,
		.lbound=lbound,
		.ubound=ubound,
		.lalarm="lower bound Alarm!",
		.ualarm="upper bound Alarm!",
		.value=(lbound+ubound)/2,
	};

	strncpy(s.base.name, name, sizeof(char)*SENSOR_HNAMELEN);
	strncpy(s.base.unit, unit, sizeof(char)*SENSOR_HUNITLEN);
	strncpy(s.lalarm, lalarm, sizeof(char)*SENSOR_HALARMLEN);
	strncpy(s.ualarm, ualarm, sizeof(char)*SENSOR_HALARMLEN);

	{
		iSensor*const p=malloc(sizeof*p);
		if(!p)return p;
	
		memcpy(p, &s, sizeof*p);
		return p;
	}
}
