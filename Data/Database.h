#ifndef DATABASE_H
#define DATABASE_H

#include <string.h>

#include "sensor.h"
#include "../Util/Trie.h"
#include "ReactorData.h"

struct SensorCB
{
	void(*cb)(Sensor*const);
};

int OpenDatabase(void);

int registerSensor(Sensor*const);

/*@null@*/void const*
Sub(void(*)(Sensor*const));

void UnSub(void const*const);

Trie const*Sensortable(char const*const);

Trie*Tables(void);

#endif
