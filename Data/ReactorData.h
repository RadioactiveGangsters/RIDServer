#ifndef REACTORDATA_H
#define REACTORDATA_H
#include"../Util/LinkedList.h"

typedef struct 
{
	char Name[15];
	unsigned int value;
}Sensor;

void Sub(void(*cb)(Sensor*v));
void UnSub(void(*cb)(Sensor*v));

#endif
