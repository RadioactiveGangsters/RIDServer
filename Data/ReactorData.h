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


void
geniSensors(
		char const*const type,
		const int amount,
		int const lbound,
		int const ubound,
		char const*const lalarm,
		char const*const ualarm
		);

void
genbSensors(
		char const*const type,
		const int amount,
		char const*const alarm
		);
#endif
