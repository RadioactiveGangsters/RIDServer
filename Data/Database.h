#ifndef DATABASE_H
#define DATABASE_H

#include "sensor.h"
#include "../Util/Trie.h"
#include "ReactorData.h"

int OpenDatabase(void);

int registerSensor(Sensor*const);

#endif
