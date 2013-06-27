#include "Server.h"

int InitServer(const int argc, char const*const*const argv)
{
	int i;
	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Initializing..");

	for(i=1;i<argc;i++)
	{
		Log(LOGL_DEBUG,LOGT_PROGRAM,"has: %s",argv[i]);
	}
	
	// need not unsubscribe
	if(!Sub(&AlarmDetection))return false;;
	if(!OpenDatabase())return false;
	return true;
}

int StartServer(void)
{
	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Starting system..");

	StartSensorSimulation();
	StartPrinter();

	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "System started");

	socklisten();
	return EXIT_SUCCESS;
}
