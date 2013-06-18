#include "Server.h"

int InitServer(const int argc,char const*const*const argv)
{
	int i;
	Log(LOGL_SYSTEM_ACTIVITY,LOGT_PROGRAM,"\n\n");
	Log(LOGL_SYSTEM_ACTIVITY,LOGT_SERVER,"Initializing..\n");

/* TODO: Change this below
	for(i=0;i<argc;i++)
		Log(LOGL_DEBUG,"%s\n",argv[i]);
*/
	return OpenDatabase();
}

int StartServer(void)
{
	Log(LOGL_SYSTEM_ACTIVITY,LOGT_SERVER,"Starting system..\n");
	StartSensorSimulation();
	StartPrinter();
	Log(LOGL_SYSTEM_ACTIVITY,LOGT_SERVER,"System started\n");
	socklisten();
	return EXIT_SUCCESS;
}
