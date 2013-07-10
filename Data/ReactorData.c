#include "ReactorData.h"

static LLNODE*threads = NULL;
static volatile bool stopsimulation=false;
unsigned int _seed;
static int interval;
static Trie* RadiationTable;
static Trie* FlowTable;
static Trie* TemperatureTable;
static Trie* FullnessTable;
static Trie* PressureTable;

/*
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
				bSensor*const b=(bSensor*)s;
				// reserve memory for delta value
				bool*p=malloc(sizeof*p);
				if(!p)
				{
					Log(LOGT_SERVER, LOGL_SERIOUS_ERROR, "Out of memory!");
					return;
				}
				*p=b->value;
				// store current value in a new history
				s->delta=AutoQe(p, 10);
				if(!s->delta)
				{
					Log(LOGT_SERVER, LOGL_SERIOUS_ERROR, "Out of memory!");
					free(p);
					return;
				}
			}
			else if(s->type==integersensor)
			{
				iSensor*const i=(iSensor*)s;
				int*p=malloc(sizeof*p);
				if(!p)
				{
					Log(LOGT_SERVER, LOGL_SERIOUS_ERROR, "Out of memory!");
					return;
				}
				//i->value=randSensorValue(i->lbound, i->ubound);
				*p=i->value;
				s->delta=AutoQe(p, 1);
				if(!s->delta)
				{
					Log(LOGT_SERVER, LOGL_SERIOUS_ERROR, "Out of memory!");
					free(p);
					return;
				}
			}
		}
		else
		{
			if(s->type==binarysensor)
			{
				bSensor*const b=(bSensor*)s;
				bool*p=malloc(sizeof*p);
				if(!p)
				{
					Log(LOGT_SERVER, LOGL_SERIOUS_ERROR, "Out of memory!");
					return;
				}
				*p=b->value;
				s->delta=AutoQadd(s->delta, p);
				b->value=binaryflux();
			}
			else if(s->type==integersensor)
			{
				iSensor*const i=(iSensor*)s;
				int*p=malloc(sizeof*p);
				if(!p)
				{
					Log(LOGT_SERVER, LOGL_SERIOUS_ERROR, "Out of memory!");
					return;
				}
				*p=i->value;
				s->delta=AutoQadd(s->delta, p);
				i->value=integerflux(i->value);
			}
		}
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
		if(1)
		{
			while(!stopsimulation)
			{
				fortrie(table, &SimulateSensor);
				Rest(1);
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
		Log(LOGT_SERVER, LOGL_DEBUG, "Creating simulation thread for %s..", table->id);
		pthread_create(&typethread, NULL, &SimulateType, table->e);
		if(!threads)
		{
			threads=lle(&typethread);
		}
		else
		{
			lladd(threads, &typethread);
		}
	}
}

int StartSensorSimulation2(void)
{
	Trie*const db=Tables();
	fortrie(db, &registerthread);
	//if() return EXIT_SUCCESS;
	//else return EXIT_FAILURE;
	return EXIT_SUCCESS; //Temporarily
}
*/

static void SimulateRadiation(Trie*const sensorbox)
{
	if(!sensorbox)return;
	if(!sensorbox->e)return;
	{
		int newValue, fx;
		Sensor*const sensor = sensorbox->e;
		
		iSensor*const isensor = (iSensor*)sensor;
		int*p = malloc(sizeof*p);
		if(!p)
		{
			Log(LOGT_SERVER, LOGL_SERIOUS_ERROR, "Out of memory!");
			return;
		}
		*p = isensor->value;
		sensor->delta = AutoQadd(sensor->delta, p);
		
		srand(_seed=(unsigned)rand());
		fx = (rand()%(39))-20;

		newValue = (isensor->value) + fx;
		isensor->value = newValue;
		PushS(sensor);
	}
}

static void SimulateFlow(Trie*const sensorbox)
{
	if(!sensorbox)return;
	if(!sensorbox->e)return;
	{
		int newValue;
		Sensor*const sensor = sensorbox->e;
		
		iSensor*const isensor = (iSensor*)sensor;
		int*p = malloc(sizeof*p);
		if(!p)
		{
			Log(LOGT_SERVER, LOGL_SERIOUS_ERROR, "Out of memory!");
			return;
		}
		*p = isensor->value;
		sensor->delta = AutoQadd(sensor->delta, p);
		
		//TODO change value by operator
		newValue = (isensor->value);
		isensor->value = newValue;
		PushS(sensor);
	}
}

static void SimulateTemperature(Trie*const sensorbox)
{
	if(!sensorbox)return;
	if(!sensorbox->e)return;
	{
		int newValue, averageRadiation, averageFlow;
		Sensor*const sensor = sensorbox->e;
		
		iSensor*const isensor = (iSensor*)sensor;
		int*p = malloc(sizeof*p);
		if(!p)
		{
			Log(LOGT_SERVER, LOGL_SERIOUS_ERROR, "Out of memory!");
			return;
		}
		*p = isensor->value;
		sensor->delta = AutoQadd(sensor->delta, p);
		
		averageRadiation = getAverageValue(RadiationTable);
		averageFlow = getAverageValue(FlowTable);
		
		newValue = (isensor->value) + ((averageRadiation/100) + (flux(1)) - (averageFlow/50));
		isensor->value = newValue;
		PushS(sensor);
	}
}

static void SimulateFullness(bool set)
{
	if(set)
	{
		int snr = random(countTrie(FullnessTable));
		char name[9+numlen((unsigned)snr)];		
		snprintf(name, sizeof(char)*(9+numlen((unsigned)snr)), "%s%d", "Fullness", snr);
		Sensor*const sensor = (findinTrie(FullnessTable,name));

		bSensor*const bsensor = (bSensor*)sensor;
		bool*p=malloc(sizeof*p);
		if(!p)
		{
			Log(LOGT_SERVER, LOGL_SERIOUS_ERROR, "Out of memory!");
			return;
		}
		*p = bsensor->value;
		sensor->delta = AutoQadd(sensor->delta, p);
		bsensor->value = true;
		PushS(sensor);		
	}
	else
	{
		Sensor*const sensor = checkFullnessValues(FullnessTable);
		if(sensor)
		{
			bSensor*const bsensor = (bSensor*)sensor;
			bool*p=malloc(sizeof*p);
			if(!p)
			{
				Log(LOGT_SERVER, LOGL_SERIOUS_ERROR, "Out of memory!");
				return;
			}
			*p = bsensor->value;
			sensor->delta = AutoQadd(sensor->delta, p);
			bsensor->value = false;
			PushS(sensor);
		}
	}
}

static void SimulatePressure(Trie*const sensorbox)
{
	if(!sensorbox)return;
	if(!sensorbox->e)return;
	{
		int newValue;
		Sensor*const sensor = sensorbox->e;
		
		iSensor*const isensor = (iSensor*)sensor;
		int*p = malloc(sizeof*p);
		if(!p)
		{
			Log(LOGT_SERVER, LOGL_SERIOUS_ERROR, "Out of memory!");
			return;
		}
		*p = isensor->value;
		sensor->delta = AutoQadd(sensor->delta, p);
		
		//TODO Write code for pressure formula:
		//startvalue+([+flux]*Taverage*((amountofFnSET1[True]/2)+1))+([-flux]*(FWaverage/20)*((amountofFnSET2[True]/2)+1))

		isensor->value = newValue;
		PushS(sensor);
	}
}

static void*Simulator(void*const rawtable)
{
	if(!rawtable)
	{
		Log(LOGT_SERVER, LOGL_ERROR, "No database for simulator!");
 		pthread_exit(NULL);
	}
	{
		Trie*const db = rawtable;
				
		if(interval)
		{			
			if(!(RadiationTable = findinTrie(db, "Radiation")))
			{
				Log(LOGT_SERVER, LOGL_ERROR, "Can not find Radiation Table in Database!");
				pthread_exit(NULL);
			}
			if(!(FlowTable = findinTrie(db, "Flow")))
			{
				Log(LOGT_SERVER, LOGL_ERROR, "Can not find Flow Table in Database!");
				pthread_exit(NULL);
			}
			if(!(TemperatureTable = findinTrie(db, "Temperature")))
			{
				Log(LOGT_SERVER, LOGL_ERROR, "Can not find Temperature Table in Database!");
				pthread_exit(NULL);
			}
			if(!(FullnessTable = findinTrie(db, "Fullness")))
			{
				Log(LOGT_SERVER, LOGL_ERROR, "Can not find Fullness Table in Database!");
				pthread_exit(NULL);
			}
			if(!(PressureTable = findinTrie(db, "Pressure")))
			{
				Log(LOGT_SERVER, LOGL_ERROR, "Can not find Pressure Table in Database!");
				pthread_exit(NULL);
			}
			{
				int fwcollection, fwcollectionav, fwcollectiondelta, count;
				Sensor*const fwsensor = FlowTable->e;
				AutoQ *q = fwsensor->delta;
				count = countTrie(FullnessTable);
			
				while(!stopsimulation)
				{
					Rest(interval);
												
				//Radiation
					fortrie(RadiationTable, &SimulateRadiation);
				
				//Flow
					fortrie(FlowTable, &SimulateFlow);
				
				//Temperature
					fortrie(TemperatureTable, &SimulateTemperature);
				
				//Fullness
					while(count > 0)
					{
						fwcollection += *(int*)q->e;
						if(count == 30) fwcollectiondelta = *(int*)q->e;
						if(count == 1) fwcollectiondelta = fwcollectiondelta - (*(int*)q->e);
						count--;
						q=q->n;
					}
					fwcollectionav = (fwcollection/30);	
					if(fwcollectionav > 350 || fwcollectiondelta > 30) SimulateFlow(true);
					if(fwcollectionav < 300) SimulateFlow(false);

				//Pressure
					fortrie(PressureTable, &SimulatePressure);
				}
			}

		}
	}
	pthread_exit(NULL);
}

int StartSensorSimulation(void)
{
	Trie*const db=Tables();
	pthread_t simulationthread;
	if(pthread_create(&simulationthread, NULL, &Simulator, db)) return EXIT_FAILURE;
	else return EXIT_SUCCESS;
}

int flux(int bound)
{
	int fx;
	srand(_seed=(unsigned)rand());
	fx = (rand()%(bound*2))-bound;
	return fx;
}

int getAverageValue(Trie*const Table)
{
	int collection, amount;
	collection = getSensorCollection(Table);
	amount = countTrie(Table);
	return (collection / amount);
}

int getSensorCollection(Trie*const trie)
{
	if(!trie)return 0;
	return getSensorCollection(trie->l) + getSensorCollection(trie->g) + ((iSensor*)(trie->e))->value;
}

Sensor* checkFullnessValues(Trie*const trie)
{
	if(!trie)return NULL;
	Sensor*e,g,l=checkfullnessvalues(trie->l);
	if(l)return l;
	g=checkfullnessvalues(trie->g);
	if(g)return g;
	e=trie->e->value?trie->e:null;
	return e;
}

static void genbSensors(
	char const*const type,  
	const int amount,  
	char const*const alarm)
{
	// generate the requested amount
	int i;
	for(i=0;i<amount;i++)
	{
		// generate a generic name
		const unsigned int namelen=1+strlen(type)+numlen((unsigned)i);
		char name[namelen];
		snprintf(name, sizeof(char)*namelen, "%s%d", type, i);

		// valid?
		if(namelen>SENSOR_HNAMELEN)
		{
			Log(LOGT_SERVER, LOGL_ERROR, "Name %s too long! (skipping rest of this batch)",  name);
			break;
		}

		// make one
		{
			Sensor*const s=(Sensor*)makebSensor(name, type, alarm);
			if(!s)
			{
				Log(LOGT_SERVER, LOGL_ERROR, "Out of memory!");
				break;
			}
			
			// register them with the databases
			if(registerSensor(s))
			{
				Log(LOGT_SERVER,LOGL_ERROR, "Cannot register %s", s->name);
				free(s);
				continue;
			}

			// set in log that all sensors of the group are generated
			if(i==(amount-1))
			{
				Log(LOGT_SERVER, LOGL_DEBUG, "Generated %d %s %s %s", 
				(i+1), s->type==binarysensor?"binary":"integer", s->unit, "Sensors");
			}
		}
	}
}

static void geniSensors(
	char const*const type,  
	const int amount,  
	int const start, 
	int const lbound,  
	int const ubound,  
	char const*const lalarm,  
	char const*const ualarm,
	bool const lboundcross, 
	bool const uboundcross)
{
	// generate the requested amount
	int i;
	for(i=0;i<amount;i++)
	{
		// generate a generic name
		const unsigned int namelen=1+strlen(type)+numlen((unsigned)i);
		char name[namelen];
		snprintf(name, sizeof(char)*namelen, "%s%d", type, i);

		// valid?
		if(namelen>SENSOR_HNAMELEN)
		{
			Log(LOGT_SERVER, LOGL_ERROR, "Name %s too long! (skipping rest of this batch)",  name);
			break;
		}

		// make one
		{
			Sensor*const s=(Sensor*)makeiSensor(name, type, start, lbound, ubound, lalarm, ualarm, lboundcross, uboundcross);
			if(!s)
			{
				Log(LOGT_SERVER, LOGL_ERROR, "Out of memory!");
				break;
			}
			
			// register them with the databases
			if(registerSensor(s))
			{
				Log(LOGT_SERVER, LOGL_ERROR, "Cannot register %s", s->name);
				free(s);
				continue;
			}

			// set in log that all sensors of the group are generated
			if(i==(amount-1))
			{
				Log(LOGT_SERVER, LOGL_DEBUG, "Generated %d %s %s %s", 
				(i+1), s->type==binarysensor?"binary":"integer", s->unit, "Sensors");
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

	if(!iniparser_find_entry(ini, "sensor"))
	{
		Log(LOGT_SERVER, LOGL_ERROR, "File %s does not contain sensor config section",  sensorinipath());
		goto exit_failure;
	}
	if(!iniparser_find_entry(ini, "sensor:typecount"))
	{
		Log(LOGT_SERVER, LOGL_ERROR, "No sensor typecount present");
		goto exit_failure;
	}
	{
		int typecount = iniparser_getint(ini, "sensor:typecount", 0);
		interval = iniparser_getint(ini, "sensor:interval", 1);

		// user requested to turn off sensors.
		if(typecount<1)goto exit_success;
		for(;typecount;typecount--)
		{
			const unsigned int idstringlen=12+numlen((unsigned)typecount);
			const unsigned int namelen=idstringlen+4;
			const unsigned int amountlen=idstringlen+5;
			const unsigned int typelen=idstringlen+4;
			char idstring[idstringlen];
			char name[namelen];
			char amount[amountlen];
			char typeq[typelen];
			sensortype stype=integersensor;

			snprintf(idstring, sizeof(char)*idstringlen, "sensor:type%d", typecount);
			snprintf(name, sizeof(char)*namelen, "%s%s", idstring, "name");
			snprintf(amount, sizeof(char)*amountlen, "%s%s", idstring, "count");

			if(!iniparser_find_entry(ini, name)||!iniparser_find_entry(ini, amount))
			{
				Log(LOGT_SERVER, LOGL_WARNING, "Skipping incomplete %s definition", idstring);
				continue;
			}

			snprintf(typeq, sizeof(char)*typelen, "%s%s", idstring, "type");
			{
				char const*const typea = iniparser_getstring(ini, typeq, "integer");

				if(!strcmp(typea, "binary"))
				{
					stype=binarysensor;
				}
				else if(!strcmp(typea, "integer")){;/*fallthrough*/;}
				else
				{
					Log(LOGT_SERVER, LOGL_WARNING, "Unrecognised type %s,  falling back on integer");
				}
	
				if(stype==binarysensor)
				{
					const unsigned int alarmlen=idstringlen+5;
					char alarmq[alarmlen];

					snprintf(alarmq, sizeof(char)*alarmlen, "%s%s", idstring, "alarm");

					genbSensors(
						iniparser_getstring(ini, name, "genericb"), 
						iniparser_getint(ini, amount, 0), 
						iniparser_getstring(ini, alarmq, "Alarm!"));
				}
				else if(stype==integersensor)
				{
					const unsigned int startlen=idstringlen+5;
					const unsigned int alarmlen=idstringlen+6;
					const unsigned int boundlen=idstringlen+6;
					const unsigned int boundcrosslen=idstringlen+11;
					signed int min, max;
					char startq[startlen];
					char lalarmq[alarmlen];
					char ualarmq[alarmlen];
					char lboundq[boundlen];
					char uboundq[boundlen];
					char lboundcrossq[boundcrosslen];
					char uboundcrossq[boundcrosslen];

					snprintf(startq, sizeof(char)*startlen, "%s%s", idstring, "start");
					snprintf(lalarmq, sizeof(char)*alarmlen, "%s%s", idstring, "lalarm");
					snprintf(ualarmq, sizeof(char)*alarmlen, "%s%s", idstring, "ualarm");
					snprintf(lboundq, sizeof(char)*boundlen, "%s%s", idstring, "lbound");
					snprintf(uboundq, sizeof(char)*boundlen, "%s%s", idstring, "ubound");
					snprintf(lboundcrossq, sizeof(char)*boundcrosslen, "%s%s", idstring, "lboundcross");
					snprintf(uboundcrossq, sizeof(char)*boundcrosslen, "%s%s", idstring, "uboundcross");
					
					min = iniparser_getint(ini, lboundq, INT_MIN);
					max = iniparser_getint(ini, uboundq, INT_MAX);

					geniSensors(
						iniparser_getstring(ini, name, "generici"), 
						iniparser_getint(ini, amount, 0), 
						iniparser_getint(ini, startq, ((min+max)/2)),
						min,
						max, 
						iniparser_getstring(ini, lalarmq, "lower bound Alarm!"), 
						iniparser_getstring(ini, ualarmq, "upper bound Alarm!"),
						iniparser_getboolean(ini, lboundcrossq, true), 
						iniparser_getboolean(ini, uboundcrossq, true));
				}
				else
				{
					Log(LOGT_SERVER, LOGL_ERROR, "Unknown sensor type %d from %s", stype, typea);
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
