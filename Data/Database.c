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

Sensor*registerSensor(Sensor*const s)
{
	if(!tbl)
	{
		tbl=triee(s->name,s);
		return (tbl)?tbl->e:tbl;
	}
	{
		Trie*added=trieadd(tbl,s->name,s);
		return (added)?added->e:added;
	}
}