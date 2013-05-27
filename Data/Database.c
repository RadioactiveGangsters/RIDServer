#include "Database.h"

Trie*db;

int startDB()
{
	if(db)
	{
		while(db->l)
		{
			free(trierm(db,db->l->id));
		}
		while(db->g)
		{
			free(trierm(db,db->g->id));
		}
		free(db);
	}
	return LoadSensors();
}

int registerSensor(Sensor*const s)
{
	// cannot register invalid sensor
	if(!s||!s->unit||!s->name) return 0;
	// do we have a db yet?
	if(!db)
	{
		Log(4,"DB uninitialised, making one.\n");
		// make a new db with a table with the sensor
		db=triee(s->unit,triee(s->name,s));
		if(!db)
		{
			
			Log(1,"Cannot register unit type %s\n",s->unit);
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
	else
	{
		// find the relevant table
		Trie*tbl=trav(db,s->unit);
		if(!tbl)
		{
			// not found or something, add it.
			// FIXME: memory leak
			tbl=trieadd(db,s->unit,triee(s->name,s));
		}
		else if(strcmp(tbl->id,s->unit))
		{
			// found something like it, but not exact, add it.
			// linking like this saves a traversal.
			// FIXME: memory leak
			tbl=trieadd(tbl,s->unit,triee(s->unit,s));
		}
		else
		{
			return trieadd(tbl,s->name,s)?EXIT_SUCCESS:EXIT_FAILURE;
		}
		if(!tbl)
		{
			Log(1,"cannot link new %s table\n",s->unit);
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
}
