#include "ReactorData.h"

LLNODE*subs;

void Sub(void(*cb)(Sensor*v))
{
	if(!subs)
	{
		subs=lle(cb);
	}
	else
	{
		lladd(subs,cb);	
	}
}

void UnSub(void(*cb)(Sensor*v))
{
	LLNODE*h=llrm(subs,cb);
	if(h)free(h);
}

int LoadSensors(void)
{
	//load the iniparser on the sensor path
	dictionary*ini=iniparser_load(sensorinipath());
	if(!ini)
		return EXIT_FAILURE;

	if(!iniparser_find_entry(ini,"sensor"))
	{
		Log(1,"File %s does not contain sensor config section\n", sensorinipath());
		goto exit_failure;
	}
	if(!iniparser_find_entry(ini,"sensor:typecount"))
	{
		Log(1,"No sensor typecount present\n");
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
			char idstring[idstringlen];
			char name[namelen];
			char amount[amountlen];
			char typeq[typelen];
			sensortype stype=integersensor;

			snprintf(idstring,sizeof(char)*idstringlen,"sensor:type%d",typecount);
			snprintf(name,sizeof(char)*namelen,"%s%s",idstring,"name");
			snprintf(amount,sizeof(char)*amountlen,"%s%s",idstring,"count");

			if(!iniparser_find_entry(ini,name)||!iniparser_find_entry(ini,amount))
			{
				Log(255,"skipping incomplete %s definition\n",idstring);
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
					Log(2,"unrecognised type %s, falling back on integer");
				}
	
				if(stype==binarysensor)
				{
					const unsigned int alarmlen=idstringlen+5;
					char alarmq[alarmlen];

					snprintf(alarmq,sizeof(char)*alarmlen,"%s%s",idstring,"alarm");

					genbSensors(
						iniparser_getstring(ini,name,"genericb"),
						iniparser_getint(ini,amount,0),
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
						iniparser_getint(ini,lboundq,0),
						iniparser_getint(ini,uboundq,100),
						iniparser_getstring(ini,lalarmq,"lower bound Alarm!"),
						iniparser_getstring(ini,ualarmq,"upper bound Alarm!"));

				}
				else
				{
					Log(1,"unknown sensor type %d from %s.\n",stype,typea);
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

void
genbSensors(
		char const*const type,
		const int amount,
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
			Log(1,"Name %s too long! (skipping rest of this batch)\n", name);
			break;
		}

		// make one
		{
			Sensor*const s=(Sensor*)makebSensor(name,type,alarm);
			if(!s)
			{
				Log(1,"out of memory");
				break;
			}

			Log(2,"generated %s{%d,%s,%d,%d,%s}\n",s->name,s->type,s->unit,s->stamp,((bSensor*)s)->value,((bSensor*)s)->alarm);
			
			// register them with the databases
			if(registerSensor(s))
			{
				Log(1,"cannot register %s\n",s->name);
				free(s);
				continue;
			}
		}
	}
}

void
geniSensors(
		char const*const type,
		const int amount,
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
			Log(1,"Name %s too long! (skipping rest of this batch)\n", name);
			break;
		}

		{
			// make one
			Sensor*const s=(Sensor*)makeiSensor(name,type,lbound,ubound,lalarm,ualarm);
			if(!s)
			{
				Log(1,"out of memory");
				break;
			}

			Log(2,"generated %s{%d,%s,%d,%d,%d,%s,%s}\n",s->name,s->type,s->unit,((iSensor*)s)->value,((iSensor*)s)->lbound,((iSensor*)s)->ubound,((iSensor*)s)->lalarm,((iSensor*)s)->ualarm);
			
			// register them with the databases
			if(registerSensor(s))
			{
				Log(1,"cannot register %s\n",s->name);
				free(s);
				continue;
			}
		}
	}

}
