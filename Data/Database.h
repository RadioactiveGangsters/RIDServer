#ifndef DATABASE_H
#define DATABASE_H

#include <string.h>
#include <stdint.h>

#include "sensor.h"
#include "../Util/Trie.h"
#include "ReactorData.h"

typedef struct
{
	void(*cb)(Sensor*const);
} SensorCB;

int OpenDatabase(void);

int registerSensor(Sensor*const);
void PushS(Sensor*const);

/*@null@*/void const*
Sub(void(*)(Sensor*const));

void UnSub(void const*const);

Trie const*Sensortable(char const*const);

Sensor* findSensor(char const*const);

volatile Trie*Tables(void);

#endif
