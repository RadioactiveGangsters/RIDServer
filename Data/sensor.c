#include"sensor.h"

unsigned int _sensseed;

void setNewSensorValue(int const amount, int*const values, unsigned int const maxval)
{
	int i;
	int count = 0;

	//Create and set seed for random generator
	_sensseed=(unsigned)rand();
	srand(_sensseed);

	for (i = 0; i < amount; i++) 
	{
		//Add new random value to old value
		values[count] += (rand()%maxval + 1);
		count++;
	}
}

int randSensorValue(int const minval, int const maxval)
{
	if(maxval-minval<1)return minval;
	//Create and set seed for random generator
	_sensseed = (unsigned)rand();
	srand(_sensseed );

	//Set random number in value
	return (rand()%maxval-minval) + minval; 
}

/* Generates a flux value for binary sensors. 
 * This value is randomly generated between the 
 * reed 1 and the -1000. The opportunity for 
 * '1' is 1 to 1000.
 */
bool binaryflux()
{
	srand(_sensseed=(unsigned)rand());
	return !(rand()%1000);
}

/* Generates a flux value for integer sensors.
 * This value will be used to increase or decrease
 * the original value. The random generated value
 * will be between -5 and 6.
 */
int integerflux(int pvalue)
{
	srand(_sensseed=(unsigned)rand());
	int flux = (rand()%11)-5;

	if(pvalue == 0)
	{
		if(flux < 0) flux = abs(flux);
		pvalue += flux;
	}
	else if((pvalue+flux)<0) pvalue = 0;
	else pvalue += flux;

	return pvalue;
	// return pvalue + ((pvalue+flux<0)?-1*flux:flux);
}

void SetupSensors(void)
{
	srand((unsigned int)time(NULL));
	_sensseed=(unsigned)rand();    
}

void DestroySensor(Sensor*const s)
{
	if(!s)return;
	DestroyAutoQ(s->delta);
	free(s);
}

bSensor*
makebSensor(
		char const*const name,
		char const*const unit,
		unsigned int const interval,
		char const*const alarm
		)
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

iSensor*
makeiSensor(
		char const*const name,
		char const*const unit,
		unsigned int const interval,
		int const lbound,
		int const ubound,
		char const*const lalarm,
		char const*const ualarm
		)
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
