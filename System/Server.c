#include "Server.h"

int InitServer(const int argc, char const*const*const argv)
{
	int i;
	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Initializing..");

	for(i=1;i<argc;i++)
	{
		Log(LOGT_PROGRAM, LOGL_DEBUG, "has: %s",argv[i]);
	}
	
	// Alarm Detection
	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Activating Alarm Detection..");
	if(!Sub(&AlarmDetection)) // need not unsubscribe
	{
		Log(LOGT_SERVER, LOGL_ERROR, "Could not activate Alarm Detection!");
		return EXIT_FAILURE;
	}
	else Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Alarm Detection active");

	// Database
	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Database initialization..");
	if(OpenDatabase()!=EXIT_SUCCESS)
	{
		Log(LOGT_SERVER, LOGL_ERROR, "Could not initialize Database!");
		return EXIT_FAILURE;
	}
	else Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Database active");

	// Network
	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Configuring Network settings..");
	if(AcceptClients()!=EXIT_SUCCESS)
	{
		Log(LOGT_SERVER, LOGL_ERROR, "Could not configure Network!");
		return EXIT_FAILURE;
	}
	else Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Configuring Network complete");
	
	// Complete
	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Initialization complete");	
	return EXIT_SUCCESS;
}

int StartServer(void)
{
	// Printer
	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Enabling Printer..");
	if(StartPrinter()!=EXIT_SUCCESS)
	{
		Log(LOGT_SERVER, LOGL_ERROR, "Could not start Printer!");
		return EXIT_FAILURE;
	}
	else Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Printer ready");
	
	// Sensor Simulators
	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Starting Simulators..");
	if(StartSensorSimulation()!=EXIT_SUCCESS)
	{
		Log(LOGT_SERVER, LOGL_ERROR, "Could not activate all Sensor Simulators!");
		return EXIT_FAILURE;
	}
	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Simulators running");

	// Complete
	Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "System started");
	return EXIT_SUCCESS;
}
