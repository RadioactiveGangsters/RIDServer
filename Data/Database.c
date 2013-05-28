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
			((SensorCB*)x->e)(s);
			x=x->n;
		}
	}
}

void Sub(void(*cb)(Sensor*v))
{
	if(!subs)
	{
		subs=lle((void*)cb);
	}
	else
	{
		lladd(subs,(void*)cb);	
	}
}

void UnSub(void(*cb)(Sensor*v))
{
	LLNODE*h=llrm(subs,(void*)cb);
	if(h)free(h);
}

int OpenDatabase()
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
