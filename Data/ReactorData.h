#ifndef REACTORDATA_H
#define REACTORDATA_H

#include <pthread.h>
#include <time.h>
#include <limits.h>
#include <math.h>

#include "../deps/iniparser/src/iniparser.h"

#include "../System/Log.h"
#include "../Util/LinkedList.h"
#include "../Util/DeathRow.h"
#include "../Util/Trie.h"
#include "../Util/Path.h"
#include "../Util/util.h"
#include "../Util/Rest.h"
#include "sensor.h"
#include "Database.h"

int LoadSensors(void);
int StartSensorSimulation(void);
int StartSensorSimulation2(void);
int flux(int bound);
int getAverageValue(Trie*const Table);

#endif
