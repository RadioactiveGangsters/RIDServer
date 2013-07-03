#include "Server.h"

int InitServer(const int argc, char const*const*const argv)
{
	int i;
	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Initializing..");

	for(i=1;i<argc;i++)
	{
		Log(LOGT_PROGRAM, LOGL_DEBUG, "has: %s",argv[i]);
	}
	
	// need not unsubscribe
	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Activating Alarm Detection..");
	if(!Sub(&AlarmDetection))
	{
		Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Could not activate Alarm Detection!");
		return EXIT_FAILURE;
	}
	else Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Alarm Detection active");

	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Database initialization..");
	if(OpenDatabase()!=EXIT_SUCCESS)return EXIT_FAILURE;
	else Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Database active");

	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Configuring Network settings..");
	if(AcceptClients()!=EXIT_SUCCESS)return EXIT_FAILURE;
	else Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Configuring Network complete");
	
	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Initialization complete");	
	return EXIT_SUCCESS;
}

int StartServer(void)
{
	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Enabling Printer..");
	if(StartPrinter()!=EXIT_SUCCESS)return EXIT_FAILURE;
	
	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Starting Simulators..");
	//if(StartSensorSimulation()!=EXIT_SUCCESS)return EXIT_FAILURE;
	StartSensorSimulation();
	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Simulators running");

	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "System started");
	return EXIT_SUCCESS;
}
