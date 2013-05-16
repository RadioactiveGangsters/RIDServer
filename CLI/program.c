#include "program.h"
#include <stdarg.h>

void console(const int ll,char const*const le,...)
{
	va_list ap;
	va_start(ap,le);
	vprintf(le,ap);
	va_end(ap);
}

int main(int argc,char**argv)
{
	subscribe(&console);
	InitServer(argc,argv);
	// TODO: check if successful.
	(void)StartServer();
	console(3,"Done.\n");

	return EXIT_SUCCESS;
}
