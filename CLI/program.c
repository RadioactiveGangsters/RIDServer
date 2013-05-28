#include "program.h"

void console(const LOGL ll,char const*const le,va_list ap)
{
	if(useColours)switch(ll)
	{
		case LOGL_SERIOUS_ERROR:
			printf("\x1B[41;37mERROR: ");
			break;
		case LOGL_ERROR:
			printf("\x1B[31mERROR: ");
			break;
		case LOGL_WARNING:
			printf("\x1B[33mWarning: ");
			break;
		case LOGL_DEBUG:
			printf("\x1B[30;1m");
			break;
		default:
			break;
	}
	(void)vprintf(le,ap);
	printf("\x1B[0m");
}

int main(int argc,char**argv)
{
	subscribe(&console);
	if(InitServer(argc,(char const*const*const)argv)!=EXIT_SUCCESS)
	{
		printf("Failed to initialize.\n");
		return EXIT_FAILURE;
	}
	// TODO: check if successful.
	(void)StartServer();
	printf("Done.\n");

	return EXIT_SUCCESS;
}
