#include"program.h"

int main(int argc,char**argv)
{
	// TODO: pass on commad-line arguments.
	InitServer(argc,argv);
	// TODO: check if successful.
	StartServer();
	printf("Done.\n");
	return EXIT_SUCCESS;
}
