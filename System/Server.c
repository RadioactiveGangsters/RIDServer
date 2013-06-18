#include "Server.h"

int InitServer(const int argc,char const*const*const argv)
{
	int i;
	Log(LOGL_DEBUG,"Initializing..\n");
	for(i=0;i<argc;i++)
		Log(LOGL_DEBUG,"%s\n",argv[i]);
	return OpenDatabase();
}

int StartServer(void)
{
	Log(LOGL_SYSTEM_ACTIVITY,"Starting Server..\n");
	StartSensorSimulation();
	StartPrinter();
	Log(LOGL_SYSTEM_ACTIVITY,"Server Started\n");
	socklisten();
	return EXIT_SUCCESS;
}
