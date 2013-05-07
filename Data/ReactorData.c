#include "ReactorData.h"


LLNODE subs;

void Sub(void(*cb)(Sensor*v))
{
	lladd(&subs,cb);	
}

void UnSub(void(*cb)(Sensor*v))
{
	llrm(&subs,cb);
}

