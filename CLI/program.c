#include "program.h"

void console(const LOGL ll,char const*const le,va_list ap)
{
	#ifdef _WIN32
	    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	    WORD saved_attributes,colour;

	    /* Save current attributes */
	    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	    saved_attributes = consoleInfo.wAttributes;
    #else

		char colour[]="\x1B[41;37";

	#endif
	char head[]="Warning: ";
	switch(ll)
	{
		case LOGL_SERIOUS_ERROR:
			#ifdef _WIN32
				colour=FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY;
			#else
				strncpy(colour,"\x1B[41;37m",sizeof(char)*8);
			#endif
			strncpy(head,"ERROR: ",sizeof(char)*9);
			break;
		case LOGL_ERROR:
			#ifdef _WIN32
				colour=FOREGROUND_RED | FOREGROUND_INTENSITY;
			#else
				strncpy(colour,"\x1B[31m",sizeof(char)*8);
			#endif
			strncpy(head,"ERROR: ",sizeof(char)*9);
			break;
		case LOGL_WARNING:
			#ifdef _WIN32
				colour=FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			#else
				strncpy(colour,"\x1B[33m",sizeof(char)*8);
			#endif
			strncpy(head,"Warning: ",sizeof(char)*9);
			break;
		case LOGL_DEBUG:
			#ifdef _WIN32
				colour=FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			#else
				strncpy(colour,"\x1B[30;1m",sizeof(char)*8);
			#endif
			head[0]='\0';
			break;
		default:
			#ifdef _WIN32
				colour=saved_attributes;
			#else
				colour[0]='\0';
			#endif
			head[0]='\0';
			break;
	}

	if(useColours)
	{
		#ifdef _WIN32
			SetConsoleTextAttribute(hConsole,colour);
			printf("%s", head);
		#else

			printf("%s%s", colour,head);
		#endif
	}

	(void)vprintf(le,ap);

    /* Restore original attributes */
	if(useColours)
	{
		#ifdef _WIN32
		    SetConsoleTextAttribute(hConsole, saved_attributes);
	    #else
			printf("\x1B[0m");
		#endif
	}
}

int main(int argc,char**argv)
{
	// no need to save the unsubscribe ticket
	(void)subscribe(&console);
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
