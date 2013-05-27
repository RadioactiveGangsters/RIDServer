#ifndef DATABASE_H
#define DATABASE_H

#include "sensor.h"
#include "../Util/Trie.h"
#include "ReactorData.h"

int OpenDatabase(void);

int registerSensor(Sensor*const);

void Sub(void(*)(Sensor*const));
void UnSub(void(*)(Sensor*const));

Trie const*Sensortable(char const*const);

Trie*Tables(void);

#endif
