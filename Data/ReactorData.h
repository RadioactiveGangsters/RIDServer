#ifndef REACTORDATA_H
#define REACTORDATA_H

#include <iniparser.h>

#include "../Util/LinkedList.h"
#include "../System/Log.h"
#include "../Util/Path.h"
#include "../Util/util.h"
#include "sensor.h"
#include "Database.h"

void Sub(void(*cb)(Sensor*v));
void UnSub(void(*cb)(Sensor*v));

int LoadSensors(void);
int generateSensors(char const*const,int const);
#endif
