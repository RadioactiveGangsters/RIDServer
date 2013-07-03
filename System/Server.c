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
	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Activating Alarm Detection..");
	if(!Sub(&AlarmDetection))
	{
		Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Could not activate Alarm Detection!");
		return EXIT_FAILURE;
	}
	else Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Alarm Detection active");

	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Database initialization..");
	if(OpenDatabase()!=EXIT_SUCCESS)return EXIT_FAILURE;
	else Log(LOGL_SYSTEM_ACTIVITY, LOGT_DB, "Database active");

	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Configuring Network settings..");
	if(AcceptClients()!=EXIT_SUCCESS)return EXIT_FAILURE;
	else Log(LOGL_SYSTEM_ACTIVITY, LOGT_NETWORK, "Configuring complete");
	
	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Initialization complete");	
	return EXIT_SUCCESS;
}

int StartServer(void)
{
	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Enabling Printer..");
	if(StartPrinter()!=EXIT_SUCCESS)return EXIT_FAILURE;
	
	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Starting Simulators..");
	//if(StartSensorSimulation()!=EXIT_SUCCESS)return EXIT_FAILURE;
	StartSensorSimulation();
	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Simulators running");

	Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "System started");
	return EXIT_SUCCESS;
}
