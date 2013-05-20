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

	dictionary*ini=iniparser_load(sensorinipath());
	
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
	int typecount=iniparser_getint(ini,"sensor:typecount",0);

	if(typecount<1)goto exit_success;
	for(;typecount;typecount--)
	{
		Log(4,"polling sensor %d... ",typecount);
		char idstring[12+numlen(typecount)];
		char name[12+numlen(typecount)+4];
		char amount[12+numlen(typecount)+5];
		
		sprintf(idstring,"sensor:type%d",typecount);
		sprintf(name,"%s%s",idstring,"name");
		sprintf(amount,"%s%s",idstring,"count");

		if(!iniparser_find_entry(ini,name)||!iniparser_find_entry(ini,amount))
		{
			Log(255,"missing (%s, %s)\n",name, amount);
			continue;
		}
		generateSensors(iniparser_getstring(ini,name,"generic"),iniparser_getint(ini,amount,0));
	}

	exit_success:
		iniparser_freedict(ini);
		return EXIT_SUCCESS;
	exit_failure:
		iniparser_freedict(ini);
		return EXIT_FAILURE;
}
int generateSensors(char const*const type,const int amount)
{
	do
	{
		for(int i=0;i<amount;i++)
		{
			Sensor*s=(Sensor*)malloc(sizeof(Sensor));
			if(!s)break;
			char name[strlen(type)+numlen(i)];
			sprintf(name,"%s%d",type,i);
			strcpy(s->name,name);
			Log(2,"generated %s\n",name);
			registerSensor(s);
		}
	}
	while(0);
	return EXIT_SUCCESS;
}
