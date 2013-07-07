#include "Printer.h"

void getSensorData(Trie* s)
{
    char const*const path = fileprinterpath(); 
    char *sensorName = ((Sensor*)s->e)->name;
    
    int min = getMin(((Sensor*)s->e)->delta);
    int max = getMax(((Sensor*)s->e)->delta);
    int mean = calcMean(((Sensor*)s->e)->delta);
    
    char data[(35 + SENSOR_HNAMELEN + numlen(min) + numlen(max) + numlen(mean))];
    char buffer[numlen(max)];
    
    //Set array as empty
    data[0] = '\0';
	
    //Add Sensor name
    strcat(data, "Sensor: ");
    strcat(data, sensorName);
	
    //Add Minimum value
    strcat(data,"  \t Min: "); 
    snprintf(buffer, sizeof(char)*numlen(min)+1, "%1d", min);
    strcat(data, buffer);
	
    //Add Maximum value
    strcat(data," \t Max: ");
    snprintf(buffer, sizeof(char)*numlen(max)+1, "%1d", max);
    strcat(data, buffer);
	
    //Add Mean value
    strcat(data," \t Mean: ");
    snprintf(buffer, sizeof(char)*numlen(mean)+1, "%1d", mean);
    strcat(data, buffer);
    
    if(!storeToFile(path, data))
    {
        Log(LOGT_PRINTER, LOGL_ERROR, "Cannot store data of %s to file", sensorName);
    }
}

void getSensors(Trie* t)
{
    // Get all sensors from sensortable and for each sensor do getSensorData()
    fortrie(t->e , &getSensorData);
}

void *getSensorTable(void *param)
{
	int timer = 3600; //1 hour

	//load the iniparser on the printer path
	dictionary*ini = iniparser_load(printerinipath());
	if(!ini) return EXIT_FAILURE;

	if(!iniparser_find_entry(ini, "printer"))
	{
		Log(LOGT_PRINTER, LOGL_ERROR, "File %s does not contain printer config section",  printerinipath());
	}
	else timer = iniparser_getint(ini, "printer:timer", 3600);

	Rest(5); // 5 second; to wait for some data to be generated

    while(true)
    {
		char data[32];
		Log(LOGT_PRINTER, LOGL_SYSTEM_ACTIVITY, "Printing new data");
		
		// Print current time and date
		data[0] = '\0';
		strcat(data, " --- ");
		time_t t = time(NULL);
		strcat(data, ctime(&t));
		data[24+3+2]='\0';
		strcat(data, " --- ");
		storeToFile(fileprinterpath(), data);

		// Get all sensors types from table and for each type do printTable()
        fortrie(Tables(), &getSensors);

	    // Wait [timer] seconds for the next printout
		Rest(timer);
    }
}

int getMin(AutoQ* list)
{ 
    int min = INT_MAX;
    while(list)
    {
        if(*((int*)list->e) < min)
        {
            min = *((int*)list->e);
        }
        list = list->n;
    }
    return min;
}

int getMax(AutoQ* list)
{
    int max = INT_MIN;
    while(list)
    {
        if(*((int*)list->e) > max)
        {
            max = *((int*)list->e);
        }
        list = list->n;
    }
    return max;
}

int calcMean(AutoQ* list)
{
    int total = 0, mean = 0, amount = 0;
    while(list)
    {
        total += *((int*)list->e);
        amount++;
        list = list->n;
    }
    if(!(total == 0)){ mean = (total/amount); }
    return mean;
}

int storeToFile(char const*const path, char const*const data)
{
    if(path == NULL)
    {
        return 0;
    }
    else
    {
        FILE *printFile;
    
        // Append data to the file
        if((printFile = fopen(path, "a")) == NULL )
        {
            return 0;
        }
        fprintf(printFile, "%s", data);

        fclose(printFile);
        return 1;
    }
}

int StartPrinter(void)
{
    pthread_t printThread;
    
    // Create new thread for printer, if failed print error to log
    if(pthread_create(&printThread, NULL, &getSensorTable, NULL)) 
    {
        Log(LOGT_SERVER, LOGL_ERROR, "Error starting Printer");
		return EXIT_FAILURE;
    }
    else
    {
        Log(LOGT_SERVER, LOGL_SYSTEM_ACTIVITY, "Printer ready"); 
		return EXIT_SUCCESS;
    }
}
