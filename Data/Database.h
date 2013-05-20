#ifndef DATABASE_H
#define DATABASE_H

#include "sensor.h"
#include "../Util/Trie.h"
#include "ReactorData.h"

int OpenDatabase(void);

Sensor*registerSensor(Sensor const*const);

#endif
