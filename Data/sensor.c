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

void ResetSensor(Sensor*const s)
{
	if(s->type == binarysensor) ((bSensor*)s)->value = false;
	else ((iSensor*)s)->value = ((iSensor*)s)->startvalue;
}

void DestroySensor(Sensor*const s)
{
	if(!s)return;
	DestroyAutoQ(s->delta);
	free(s);
}

bSensor* makebSensor(
	char const*const name,
	char const*const unit,
	char const*const alarm)
{
	int*ptrtomyint=malloc(sizeof(int));
	*ptrtomyint=false;

	Sensor base=
	{
		.name="genericb",
		.unit="on/off",
		.type=binarysensor,
		.stamp=time(0),
		.delta=AutoQe(ptrtomyint,(60*60)),
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

iSensor* makeiSensor(
	char const*const name,
	char const*const unit,
	int const startvalue,
	int const lbound,
	int const ubound,
	char const*const lalarm,
	char const*const ualarm,
	bool const lboundcross,
	bool const uboundcross)
{
	iSensor s=
	{
		.base=
		{
			.name="generici",
			.unit="generici",
			.type=integersensor,
			.stamp=time(0),
			.delta=AutoQe(malloc(sizeof(startvalue)),(60*60)),
		},
		.startvalue=startvalue,
		.lbound=lbound,
		.ubound=ubound,
		.lalarm="lower bound Alarm!",
		.ualarm="upper bound Alarm!",
		.value=startvalue,
		.lboundcross=lboundcross,
		.uboundcross=uboundcross,
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
