#include "program.h"

void console(const LOGL ll,char const*const le,va_list ap)
{
	switch(ll)
	{
		case LOGL_SERIOUS_ERROR:
			printf("\x1B[41;37m");
			printf("ERROR: ");
			break;
		case LOGL_ERROR:
			printf("\x1B[31m");
			printf("ERROR: ");
			break;
		case LOGL_WARNING:
			printf("\x1B[33m");
			printf("Warning: ");
			break;
		case LOGL_DEBUG:
			printf("\x1B[30;1m");
		default:
			break;
	}
	vprintf(le,ap);
	printf("\x1B[0m");
}

int main(int argc,char**argv)
{
	subscribe(&console);
	InitServer(argc,(char const*const*const)argv);
	// TODO: check if successful.
	(void)StartServer();
	printf("Done.\n");

	return EXIT_SUCCESS;
}
