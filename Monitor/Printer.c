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
        Log(LOGL_ERROR, LOGT_PRINTER, "Cannot store data of %s to file\n", sensorName);
    }
}

void getSensors(Trie* t)
{
    // Get all sensors from sensortable and for each sensor do getSensorData()
    fortrie(t->e , &getSensorData);
}

void *getSensorTable(void *param)
{
    	time_t t;
    // 5 second; to wait for some data to be generated
	#ifdef _WIN32
	Sleep(5000
	#else
	sleep(5
	#endif
	);

    while(1)
    {
		Log(LOGL_SYSTEM_ACTIVITY, LOGT_PRINTER, "Printing new data\n");
		
		// Print current time and date
		char data[32];
		data[0] = '\0';
		strcat(data, " --- ");
		time(&t);
		strcat(data, ctime(&t));
		data[24+3+2]='\0';
		strcat(data, " --- ");
		storeToFile(fileprinterpath(), data);

		// Get all sensors types from table and for each type do printTable()
        fortrie(Tables(), &getSensors);
	   // Wait 10 minutes for the next printout
	   // TODO: configurable
		#ifdef _WIN32
		Sleep(600000
		#else
		sleep(600
		#endif
		);
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
        fprintf(printFile, "%s\n", data);

        fclose(printFile);
        return 1;
    }
}

void StartPrinter()
{
    pthread_t printThread;
    
    // Create new thread for printer, if failed print error to log
    if(pthread_create(&printThread, NULL, &getSensorTable, NULL)) 
    {
        Log(LOGL_ERROR, LOGT_SERVER, "Error creating thread for printer\n");
    }
    else
    {
        Log(LOGL_SYSTEM_ACTIVITY, LOGT_SERVER, "Printer Started\n"); 
    }
}
