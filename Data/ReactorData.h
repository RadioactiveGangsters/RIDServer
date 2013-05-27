#ifndef REACTORDATA_H
#define REACTORDATA_H

#include <iniparser.h>
#include <pthread.h>

#include "../Util/LinkedList.h"
#include "../System/Log.h"
#include "../Util/Path.h"
#include "../Util/util.h"
#include "sensor.h"
#include "Database.h"

int LoadSensors(void);

void StartSensorSimulation(void);

#endif
