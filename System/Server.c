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
	if(!Sub(&AlarmDetection))return EXIT_FAILURE;
	if(OpenDatabase()!=EXIT_SUCCESS)return EXIT_FAILURE;
	if(AcceptClients()!=EXIT_SUCCESS)return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int StartServer(void)
{
	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Starting system..");

	StartPrinter();
	StartSensorSimulation();

	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "System started");

	return EXIT_SUCCESS;
}
