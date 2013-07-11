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
int getAverageValue(Trie*const Table);
int getSensorCollection(Trie*const trie);
Sensor* checkFullnessValues(Trie*const trie);
int countTrueInSet(Trie*db, int*skip, int*upto);

#endif
