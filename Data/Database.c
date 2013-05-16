#include "Database.h"

Trie*tbl;

int startDB()
{
	if(tbl)
	{
		// TODO: clear entire trie;
		tbl=NULL;
	}
}

Sensor*registerSensor(Sensor const*const s)
{
	return trieadd(tbl,s->id,s)->e;
}