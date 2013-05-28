#ifndef REACTORDATA_H
#define REACTORDATA_H

#include <pthread.h>

#include "../iniparser/src/iniparser.h"

#include "../System/Log.h"
#include "../Util/LinkedList.h"
#include "../Util/DeathRow.h"
#include "../Util/Trie.h"
#include "../Util/Path.h"
#include "../Util/util.h"
#include "sensor.h"
#include "Database.h"

int LoadSensors(void);

void StartSensorSimulation(void);

#endif
