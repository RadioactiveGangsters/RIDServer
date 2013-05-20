#include "Database.h"

Trie*tbl;

int startDB()
{
	if(tbl)
	{
		// TODO: clear entire trie;
		tbl=NULL;
	}

	return LoadSensors();
}

Sensor*registerSensor(Sensor const*const s)
{
	if(!tbl)
	{
		return tbl=triee(s->name,s)->e;
	}
	else return trieadd(tbl,s->name,s)->e;
}