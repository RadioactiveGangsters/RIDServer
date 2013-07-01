#include "program.h"

void console(const LOGL ll, const LOGT lt, char const*const le, va_list ap)
{
	#ifdef _WIN32
	    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	    WORD saved_attributes, colour;

	    /* Save current attributes */
	    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	    saved_attributes = consoleInfo.wAttributes;
    #else
		char colour[] ="\x1B[41;37;1m";
	#endif
	size_t lcolour=sizeof(char)*10;
	
	char const*const origin[]=
	{
		//UNDEFINED
		"",
		// PROGRAM
		"",
		//SERVER
		"Server",
		//DB
		"Database",
		//PRINTER
		"Printer",
		//NETWORK
		"Network",
		//CLIENT
		"Client",
	},

	*const head[]=
	{
		//UNDEFINED
		": ",
		//ERROR
		" error: ",
		//WARNING
		" warning: ",
		//SERIOUS_ERROR
		" ERROR: ",
		//ALARM
		" alarm: ",
		// DEBUG
		": ",
		// SYSTEM_ACTIVITY
		": ",
		// CLIENT_ACTIVITY
		": ",
	};


	const char*const porigin=origin[lt],*const phead=head[ll];

	switch(ll)
	{
		case LOGL_SERIOUS_ERROR:
			#ifdef _WIN32
				colour = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY;
			#else
				strncpy(colour, "\x1B[41;37m", lcolour);
			#endif
			break;

		case LOGL_ERROR:
			#ifdef _WIN32
				colour = FOREGROUND_RED | FOREGROUND_INTENSITY;
			#else
				strncpy(colour, "\x1B[31m", lcolour);
			#endif
			break;

		case LOGL_BUG:
			#ifdef _WIN32
				colour = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | FOREGROUND_BLUE;
			#else
				strncpy(colour, "\x1B[43;30m",sizeof(char)*8);
			#endif
			strncpy(head, "BUG -> ", sizeof(char)*12);
			break;

		case LOGL_WARNING:
			#ifdef _WIN32
				colour = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			#else
				strncpy(colour, "\x1B[33m", lcolour);
			#endif
			break;

		case LOGL_ALARM:
			#ifdef _WIN32
				colour = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED ;
			#else
				strncpy(colour, "\x1B[41;37m", lcolour);
			#endif
			break;

		case LOGL_DEBUG:
			#ifdef _WIN32
				colour = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			#else
				strncpy(colour, "\x1B[30;1m", lcolour);
			#endif
			break;

		default:
			#ifdef _WIN32
				colour = saved_attributes;
			#else
				colour[0] ='\0';
			#endif
			break;
	}

	if(useColours)
	{
		#ifdef _WIN32
			SetConsoleTextAttribute(hConsole, colour);
		#else
			printf("%s", colour);
		#endif
	}
	printf("%s%s",porigin, phead);

	(void)vprintf(le, ap);

    /* Restore original attributes */
	if(useColours)
	{
		#ifdef _WIN32
		    SetConsoleTextAttribute(hConsole, saved_attributes);
	    #else
			printf("\x1B[0m");
		#endif
	}
	printf("\n");
}

int main(int argc, char**argv)
{
	printf("\n\n------- PROJECT IRI --- RADIOACTIVE GANGSTERS -------\n\n");

	// no need to save the unsubscribe ticket
	(void)subscribe(&console);
	if(InitServer(argc, (char const*const*const)argv) != EXIT_SUCCESS)
	{
		printf("Failed to initialize.\n");
		return EXIT_FAILURE;
	}

	#ifdef _WIN32
	Sleep(10000);
	#else
	sleep(10);
	#endif

	// TODO: check if successful.
	(void)StartServer();
	printf("Done.");

	// TODO: I don't know.
	// join some other thread, maybe?
	Log(LOGL_BUG,LOGT_PROGRAM,"server will exit on keypress.");
	getchar();

	return EXIT_SUCCESS;
}
