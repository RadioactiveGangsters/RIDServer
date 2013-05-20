#include "program.h"

void console(const int ll,char const*const le,va_list ap)
{
	vprintf(le,ap);
	fflush(stdout);
}

int main(int argc,char**argv)
{
	subscribe(&console);
	InitServer(argc,argv);
	// TODO: check if successful.
	(void)StartServer();
	printf("Done.\n");

	return EXIT_SUCCESS;
}
