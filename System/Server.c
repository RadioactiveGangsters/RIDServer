#include "Server.h"

int InitServer(const int argc,char const*const*const argv)
{
	int i;
	Log(4,"Initializing\n");
	for(i=0;i<argc;i++)
		Log(4,"%s\n",argv[i]);
	return OpenDatabase();
}

int StartServer(void)
{
	Log(4,"Starting\n");
	StartSensorSimulation();
	socklisten();
	return EXIT_SUCCESS;
}
