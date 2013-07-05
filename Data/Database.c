#include "Database.h"

static Trie*db;

static LLNODE*subs;

void PushS(Sensor*const s)
{
	if(!subs)return;
	{
		LLNODE*x=subs;
		do
		{
			((SensorCB*)x->e)->cb(s);
			x=x->n;
		}
		while(x!=NULL);
	}
}

void const* Sub(void(*cb)(Sensor*))
{
	if(!cb)return NULL;
	{
		SensorCB c=
		{
			.cb=cb, 
		}, *p=malloc(sizeof*p);

		if(!p)return NULL;
		memcpy(p, &c, sizeof*p);

		if(!subs)
		{
			subs=lle(p);
		}
		else
		{
			lladd(subs, p);
		}
		return p;
	}
}

void UnSub(void const*const ticket)
{
	LLNODE*h=llrm(subs, ticket);
	if(h)
	{
		free(h->e);
		free(h);
	}
}

Sensor*findSensorinTable(Trie const*const table,char const*const name)
{
	if(!table)return NULL;
	if(!name)return NULL;
	{
	const int diff = strcmp(table->id,name);

	if(!diff)
		return table->e;
	
	if(diff<0)
		return findSensorinTable(table->l,name);
	else
		return findSensorinTable(table->g,name);
	}
}

Sensor*findSensorinDB(Trie const*const db,char const*const name)
{
	if(!db){return NULL;};
	if(!name){return NULL;};
	return(Sensor*)(
		(uintptr_t)findSensorinDB(db->l,name)^
		(uintptr_t)findSensorinDB(db->g,name)^
		(uintptr_t)findSensorinTable(db->e,name));
}

Sensor*findSensor(char const*const name)
{
	return findSensorinDB(db,name);
}

static void DestroyTable(Trie*const e)
{
	if(!e)return;
	DestroySensor(e->e);
	free(e);
}

static void DestroyDB(Trie*const e)
{
	if(!e)return;
	// for each leaf in the table
	fortrie(e->e, &DestroyTable);
	free(e);
}

int OpenDatabase()
{
	// for each table in the db
	fortrie(db, &DestroyDB);
	return LoadSensors();
}

int registerSensor(Sensor*const s)
{
	// cannot register invalid sensor
	if(!s||!s->unit||!s->name) return 0;
	// do we have a db yet?
	if(!db)
	{
		Trie*const tbl=triee(s->name, s);
		// Make a new Trie with the sensor
		if(!tbl)
		{
			Log(LOGT_DB, LOGL_ERROR, "Cannot register unit type %s", s->unit);
			return EXIT_SUCCESS;
		}

		// Link the new Table to the empty database,  creating one.
		db=triee(s->unit, tbl);
		if(!db)
		{
			Log(LOGT_DB, LOGL_ERROR, "Cannot create database", s->unit);
			DestroyTable(tbl);
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
	else
	{
		// find the relevant table
		Trie*tbl=trav(db, s->unit);
		if(!tbl)
		{
			// not found or something, make a new table.
			Trie*const newtable=triee(s->name, s);
			if(!newtable)
			{
				Log(LOGT_DB, LOGL_ERROR, "Cannot create new table for %s", s->unit);
			}
			else
			{
				// link the table to the database.
				tbl=trieadd(db, s->unit, newtable);
				if(!tbl)
				{
					Log(LOGT_DB, LOGL_ERROR, "Cannot expand database with table for %s", s->unit);
					DestroyTable(newtable);
					// FIXME: double deallocation of s by caller?
					return EXIT_FAILURE;
				}
			}
		}
		else if(strcmp(tbl->id, s->unit))
		{
			// found something like it,  but not exact,  add it.
			Trie*const newtable=triee(s->name, s);
			if(!newtable)
			{
				Log(LOGT_DB, LOGL_ERROR, "Cannot create new table for %s", s->unit);
				return EXIT_FAILURE;
			}
			// linking like this saves a traversal.
			tbl=trieadd(tbl, s->unit, triee(s->name, s));
			if(!tbl)
			{
				Log(LOGT_DB, LOGL_ERROR, "Cannot expand database with table for %s", s->unit);
				DestroyTable(newtable);
				return EXIT_FAILURE;
			}
		}
		else
		{
			// found the table
			// can it be added?
			if(!trieadd(tbl->e, s->name, s))
			{
				// the table has no... table?
				if(!tbl->e)
				{
					Log(LOGT_DB, LOGL_SERIOUS_ERROR, "Structure corrupt, this is a bug");
				}
				else
				{
					Log(LOGT_DB, LOGL_ERROR, "Cannot add to table %s", ((Trie*)tbl->e)->id);
				}
				return EXIT_FAILURE;
			}
		}
		return EXIT_SUCCESS;
	}
}

Trie const*Sensortable(char const*const type)
{
	if(!type)return NULL;
	if(!db)return NULL;
	{
		Trie*found=trav(db, type);
		if(!strcmp(found->id, type))return found;
		return NULL;
	}
}

Trie*Tables(void)
{
	return db;
}
