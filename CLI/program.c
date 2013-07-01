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
		char colour[] ="\x1B[41;37";
	#endif

	char head[] ="Warning: ";

	switch(ll)
	{
		case LOGL_SERIOUS_ERROR:
			#ifdef _WIN32
				colour = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY;
			#else
				strncpy(colour, "\x1B[41;37m", sizeof(char)*8);
			#endif
			strncpy(head, "ERROR -> ", sizeof(char)*10);
			break;

		case LOGL_ERROR:
			#ifdef _WIN32
				colour = FOREGROUND_RED | FOREGROUND_INTENSITY;
			#else
				strncpy(colour, "\x1B[31m", sizeof(char)*8);
			#endif
			strncpy(head, "ERROR -> ", sizeof(char)*10);
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
				strncpy(colour, "\x1B[33m", sizeof(char)*8);
			#endif
			strncpy(head, "WARNING -> ", sizeof(char)*12);
			break;

		case LOGL_ALARM:
			#ifdef _WIN32
				colour = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY;
			#else
				strncpy(colour, "\x1B[41;37m", sizeof(char)*8);
			#endif
			strncpy(head, "ALARM -> ", sizeof(char)*10);
			break;

		case LOGL_DEBUG:
			#ifdef _WIN32
				colour = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			#else
				strncpy(colour, "\x1B[30;1m", sizeof(char)*8);
			#endif
			head[0] ='\0';
			break;

		default:
			#ifdef _WIN32
				colour = saved_attributes;
			#else
				colour[0] ='\0';
			#endif
			head[0] ='\0';
			break;
	}

	switch(lt)
	{
		case LOGT_SERVER:
			strncpy(head, "Server:   ", sizeof(char)*11);
			break;

		case LOGT_DB:
			strncpy(head, "Database: ", sizeof(char)*11);
			break;

		case LOGT_PRINTER:
			strncpy(head, "Printer:  ", sizeof(char)*11);
			break;

		case LOGT_NETWORK:
			strncpy(head, "Network:  ", sizeof(char)*11);
			break;

		case LOGT_CLIENT:
			strncpy(head, "Client:   ", sizeof(char)*11);
			break;

		default:
			head[0]='\0';
			break;
	}

	if(useColours)
	{
		#ifdef _WIN32
			SetConsoleTextAttribute(hConsole, colour);
			printf("%s", head);
		#else

			printf("%s%s", colour, head);
		#endif
	}

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
	printf("Done.\n");

	// TODO: I don't know.
	// join some other thread, maybe?
	Log(LOGL_BUG,LOGT_PROGRAM,"server will exit on keypress.\n");
	getchar();

	return EXIT_SUCCESS;
}
