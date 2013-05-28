#include "Database.h"

static Trie*db;

static LLNODE*subs;

void PushS(Sensor*const s)
{
	if(!subs)return;
	{
		LLNODE*x=subs;
		while(x->n!=NULL)
		{
			((SensorCB*)x->e)->cb(s);
			x=x->n;
		}
	}
}

void const*
Sub(void(*cb)(Sensor*))
{
	if(!cb)return NULL;
	{
		SensorCB c=
		{
			.cb=cb,
		},*p=malloc(sizeof*p);

		if(!p)return NULL;
		memcpy(p,&c,sizeof*p);

		if(!subs)
		{
			subs=lle(p);
		}
		else
		{
			lladd(subs,p);
		}
		return p;
	}
}

void UnSub(void const*const ticket)
{
	LLNODE*h=llrm(subs,ticket);
	if(h)
	{
		free(h->e);
		free(h);
	}
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
	fortrie(e->e,&DestroyTable);
	free(e);
}

int OpenDatabase()
{
	// for each table in the db
	fortrie(db,&DestroyDB);
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
			tbl=trieadd(tbl,s->unit,triee(s->name,s));
		}
		else
		{
			if(!trieadd(tbl->e,s->name,s))
			{
				if(!tbl->e)
				{
					Log(1,"Database structure corrupt, this is a bug.");
				}
				else
				{
					tbl=tbl->e;
					Log(1,"Cannot add to table %s\n",tbl->id);
				}
				return EXIT_FAILURE;
			}
		}

		if(!tbl)
		{
			Log(1,"cannot link new %s table\n",s->unit);
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
}

Trie const*Sensortable(char const*const type)
{
	if(!type)return NULL;
	if(!db)return NULL;
	{
		Trie*found=trav(db,type);
		if(!strcmp(found->id,type))return found;
		return NULL;
	}
}

Trie*Tables(void)
{
	return db;
}
