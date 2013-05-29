#include "ReactorData.h"

static LLNODE*threads = NULL;
static volatile bool stopsimulation=false;

static void SimulateSensor(Trie*const sensor)
{
	if(!sensor)return;
	if(!sensor->e)return;
	{
		Sensor*const s=sensor->e;
		// is initialised?
		if(!s->delta) 
		{
			if(s->type==binarysensor)
			{
			}
			else if(s->type==integersensor)
			{
				initSensorValue(&(((iSensor*)s)->value),((iSensor*)s)->ubound);
				s->delta=AutoQe(&(((iSensor*)s)->value),s->interval);
			}
		}
		else
		{
			if(s->type==binarysensor)
			{
			}
			else if(s->type==integersensor)
			{
				s->delta=AutoQadd(s->delta,&(((iSensor*)s)->value));
				initSensorValue(&(((iSensor*)sensor)->value),((iSensor*)sensor)->ubound);
			}
		}
		Log(LOGL_DEBUG,"Simulated %d>>%s\n",((iSensor*)s)->value,s->name);
		PushS(s);
	}
}

static void*SimulateType(void*const rawtable)
{
	if(!rawtable)
		pthread_exit(NULL);
	{
		Trie*const table=rawtable;
		Sensor const*const example=table->e;
		if(example->interval)
		{
			while(!stopsimulation)
			{
				fortrie(table,&SimulateSensor);
				#ifdef _WIN32
				Sleep(example->interval*1000);
				#else
				sleep(example->interval);
				#endif
			}
		}
	}
	pthread_exit(NULL);
}

static void registerthread(Trie*const table)
{
	if(!table)return;
	if(!table->e)return;
	{
		pthread_t typethread;
		Log(LOGL_SYSTEM_ACTIVITY,"creating simulation thread for %s\n",table->id);
		pthread_create(&typethread,NULL,&SimulateType,table->e);
		if(!threads)
		{
			threads=lle(&typethread);
		}
		else
		{
			lladd(threads,&typethread);
		}
	}
}

void StartSensorSimulation(void)
{
	Trie*const db=Tables();
	fortrie(db,&registerthread);
}

static void
genbSensors(
		char const*const type,
		const int amount,
		unsigned int const interval,
		char const*const alarm
		)
{
	// generate the requested amount
	int i;
	for(i=0;i<amount;i++)
	{
		// generate a generic name
		const unsigned int namelen=1+strlen(type)+numlen((unsigned)i);
		char name[namelen];
		snprintf(name,sizeof(char)*namelen,"%s%d",type,i);

		// valid?
		if(namelen>SENSOR_HNAMELEN)
		{
			Log(LOGL_ERROR,"Name %s too long! (skipping rest of this batch)\n", name);
			break;
		}

		// make one
		{
			Sensor*const s=(Sensor*)makebSensor(name,type,interval,alarm);
			if(!s)
			{
				Log(LOGL_ERROR,"out of memory");
				break;
			}

			Log(LOGL_DEBUG,"generated %s %s{%s,%d,%d,%d,%s}\n",s->type==binarysensor?"binary":"integer",s->name,s->unit,s->interval,s->stamp,((bSensor*)s)->value,((bSensor*)s)->alarm);
			
			// register them with the databases
			if(registerSensor(s))
			{
				Log(LOGL_ERROR,"cannot register %s\n",s->name);
				free(s);
				continue;
			}
		}
	}
}

static void
geniSensors(
		char const*const type,
		const int amount,
		unsigned int const interval,
		int const lbound,
		int const ubound,
		char const*const lalarm,
		char const*const ualarm
		)
{
	// generate the requested amount
	int i;
	for(i=0;i<amount;i++)
	{
		// generate a generic name
		const unsigned int namelen=1+strlen(type)+numlen((unsigned)i);
		char name[namelen];
		snprintf(name,sizeof(char)*namelen,"%s%d",type,i);

		// valid?
		if(namelen>SENSOR_HNAMELEN)
		{
			Log(LOGL_ERROR,"Name %s too long! (skipping rest of this batch)\n", name);
			break;
		}

		{
			// make one
			Sensor*const s=(Sensor*)makeiSensor(name,type,interval,lbound,ubound,lalarm,ualarm);
			if(!s)
			{
				Log(LOGL_ERROR,"out of memory");
				break;
			}

			Log(LOGL_DEBUG,"generated %s %s{%s,%d, %d,%d,%d,%s,%s}\n",
				s->type==binarysensor?"binary":"integer",
				s->name,
				s->unit,
				s->interval,
				((iSensor*)s)->value,
				((iSensor*)s)->lbound,
				((iSensor*)s)->ubound,
				((iSensor*)s)->lalarm,
				((iSensor*)s)->ualarm);
			
			// register them with the databases
			if(registerSensor(s))
			{
				Log(LOGL_ERROR,"cannot register %s\n",s->name);
				free(s);
				continue;
			}
		}
	}

}

int LoadSensors(void)
{
	//load the iniparser on the sensor path
	dictionary*ini=iniparser_load(sensorinipath());
	if(!ini)
		return EXIT_FAILURE;

	// just to be sure
	SetupSensors();

	if(!iniparser_find_entry(ini,"sensor"))
	{
		Log(LOGL_ERROR,"File %s does not contain sensor config section\n", sensorinipath());
		goto exit_failure;
	}
	if(!iniparser_find_entry(ini,"sensor:typecount"))
	{
		Log(LOGL_ERROR,"No sensor typecount present\n");
		goto exit_failure;
	}
	{
		int typecount=iniparser_getint(ini,"sensor:typecount",0);

		// user requested to turn off sensors.
		if(typecount<1)goto exit_success;
		for(;typecount;typecount--)
		{
			const unsigned int idstringlen=12+numlen((unsigned)typecount);
			const unsigned int namelen=idstringlen+4;
			const unsigned int amountlen=idstringlen+5;
			const unsigned int typelen=idstringlen+4;
			const unsigned int intervallen=idstringlen+8;
			char idstring[idstringlen];
			char name[namelen];
			char amount[amountlen];
			char typeq[typelen];
			char intervalq[intervallen];
			sensortype stype=integersensor;

			snprintf(idstring,sizeof(char)*idstringlen,"sensor:type%d",typecount);
			snprintf(name,sizeof(char)*namelen,"%s%s",idstring,"name");
			snprintf(amount,sizeof(char)*amountlen,"%s%s",idstring,"count");
			snprintf(intervalq,sizeof(char)*intervallen,"%s%s",idstring,"interval");

			if(!iniparser_find_entry(ini,name)||!iniparser_find_entry(ini,amount)||!iniparser_find_entry(ini,intervalq))
			{
				Log(LOGL_WARNING,"skipping incomplete %s definition\n",idstring);
				continue;
			}

			snprintf(typeq,sizeof(char)*typelen,"%s%s",idstring,"type");
			{
				char const*const typea = iniparser_getstring(ini,typeq,"integer");

				if(!strcmp(typea,"binary"))
				{
					stype=binarysensor;
				}
				else if(!strcmp(typea,"integer")){;/*fallthrough*/;}
				else
				{
					Log(LOGL_WARNING,"unrecognised type %s, falling back on integer");
				}
	
				if(stype==binarysensor)
				{
					const unsigned int alarmlen=idstringlen+5;
					char alarmq[alarmlen];

					snprintf(alarmq,sizeof(char)*alarmlen,"%s%s",idstring,"alarm");

					genbSensors(
						iniparser_getstring(ini,name,"genericb"),
						iniparser_getint(ini,amount,0),
						(unsigned)iniparser_getint(ini,intervalq,1000),
						iniparser_getstring(ini,alarmq,"Alarm!"));
				}
				else if(stype==integersensor)
				{
					const unsigned int alarmlen=idstringlen+6;
					const unsigned int boundlen=idstringlen+6;
					char lalarmq[alarmlen];
					char ualarmq[alarmlen];
					char lboundq[boundlen];
					char uboundq[boundlen];

					snprintf(lalarmq,sizeof(char)*alarmlen,"%s%s",idstring,"lalarm");
					snprintf(ualarmq,sizeof(char)*alarmlen,"%s%s",idstring,"ualarm");
					snprintf(lboundq,sizeof(char)*boundlen,"%s%s",idstring,"lbound");
					snprintf(uboundq,sizeof(char)*boundlen,"%s%s",idstring,"ubound");

					geniSensors(
						iniparser_getstring(ini,name,"generici"),
						iniparser_getint(ini,amount,0),
						(unsigned)iniparser_getint(ini,intervalq,1000),
						iniparser_getint(ini,lboundq,0),
						iniparser_getint(ini,uboundq,100),
						iniparser_getstring(ini,lalarmq,"lower bound Alarm!"),
						iniparser_getstring(ini,ualarmq,"upper bound Alarm!"));

				}
				else
				{
					Log(LOGL_ERROR,"unknown sensor type %d from %s.\n",stype,typea);
				}
			}		
		}
	}

	exit_success:
		iniparser_freedict(ini);
		return EXIT_SUCCESS;
	exit_failure:
		iniparser_freedict(ini);
		return EXIT_FAILURE;
}
