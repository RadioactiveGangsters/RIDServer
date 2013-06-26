#include "Server.h"

int InitServer(const int argc, char const*const*const argv)
{
	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Initializing..\n");

/*  TODO: Change this below
	for(i=0;i<argc;i++)
	{
		Log(LOGL_DEBUG,"%s\n",argv[i]);
	}
*/
	
	// need not unsubscribe
	if(!Sub(&AlarmDetection))return EXIT_FAILURE;
	if(OpenDatabase()!=EXIT_SUCCESS)
	{
		Log(LOGL_WARNING,LOGT_SERVER,"db init failed\n");
		return EXIT_FAILURE;
	}
	if(AcceptClients()!=EXIT_SUCCESS)
	{
		Log(LOGL_WARNING,LOGT_SERVER,"Networ init failed\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int StartServer(void)
{
	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Starting system..\n");

	StartPrinter();
	StartSensorSimulation();

	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "System started\n");

	return EXIT_SUCCESS;
}
