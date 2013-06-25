#include "Server.h"

int InitServer(const int argc, char const*const*const argv)
{
	int i;
	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Initializing..\n");

/*  TODO: Change this below
	for(i=0;i<argc;i++)
	{
		Log(LOGL_DEBUG,"%s\n",argv[i]);
	}
*/
	
	// need not unsubscribe
	if(!Sub(&AlarmDetection))return false;;
	if(!OpenDatabase())return false;
	return true;
}

int StartServer(void)
{
	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Starting system..\n");

	StartPrinter();
	StartSensorSimulation();

	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "System started\n");

	socklisten();
	return EXIT_SUCCESS;
}
