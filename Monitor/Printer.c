#include "Printer.h"

/**	
 *	Geta all the sensor data from the assigned sensor
 *	and calculate the min, max and mean.
 */
void getSensorData(Trie* sensor)
{
    char *sensorName = ((Sensor*)sensor->e)->name;
    
    int min=0,max=0,mean=0,namesize=0;
    switch (((Sensor*)sensor->e)->type)
    {
	    case integersensor:
    		min = getMin(((Sensor*)sensor->e)->delta);
    		max = getMax(((Sensor*)sensor->e)->delta);
    		mean = calcMean(((Sensor*)sensor->e)->delta);
		break;
		default:
		break;
    }
    {
	    char data[(36 + SENSOR_HNAMELEN + numlen(min) + numlen(max) + numlen(mean))];
		char buffer[numlen(max)];
		
		// Set array as empty
		data[0] = '\0';
	
		// Add Sensor name
		strcat(data, "Sensor: ");
		namesize = strlen(sensorName);
		while((namesize-14) != 0)
		{
			strcat(sensorName, " ");
			namesize++;
		}
		strcat(data, sensorName);
	
		// Add Minimum value
		strcat(data," \t Min: "); 
		snprintf(buffer, sizeof(char)*numlen(min)+1, "%1d", min);
		strcat(data, buffer);
	
		// Add Maximum value
		strcat(data," \t Max: ");
		snprintf(buffer, sizeof(char)*numlen(max)+1, "%1d", max);
		strcat(data, buffer);
	
		// Add Mean value
		strcat(data," \t Mean: ");
		snprintf(buffer, sizeof(char)*numlen(mean)+1, "%1d", mean);
		strcat(data, buffer);
		
		// Store string in the file
		if(!storeToFile(fileprinterpath(), data))
		{
		    Log(LOGT_PRINTER, LOGL_ERROR, "Cannot store data of %s to file", sensorName);
		}
    }
}

/**	
 *	Get all sensors from the table
 *	and for each sensor do getSensorData()
 */
void getSensors(Trie* table)
{
    fortrie(table->e , &getSensorData);
	storeToFile(fileprinterpath(), " ");
}

/**	
 *	Start of printer
 *	Gets timer from ini file
 *	Runs the printer every [timer]
 */
void *getSensorTable(void *param)
{
	if(param)Log(LOGT_PRINTER, LOGL_BUG, "Did not expect printer thread parameters");
	{
		int timer = 3600; // 1 hour (default)
		// load the iniparser on the printer path
		dictionary*ini = iniparser_load(printerinipath());
		if(!ini) pthread_exit(NULL);

		if(!iniparser_find_entry(ini, "printer"))
		{
			Log(LOGT_PRINTER, LOGL_ERROR, "File %s does not contain printer config section",  printerinipath());
		}
		else timer = iniparser_getint(ini, "printer:timer", 3600);

		Rest(5); // 5 second; to wait for some data to be generated
		iniparser_freedict(ini);

		while(true)
		{
			char data[32];
			time_t t;
			Log(LOGT_PRINTER, LOGL_SYSTEM_ACTIVITY, "Printing new data");
		
			// Print current time and date
			data[0] = '\0';
			strcat(data, " --- ");
			t = time(NULL);
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
}

/**	
 *	Returns the minimum value of all
 *	values in the queue (history of sensor)
 */
int getMin(AutoQ* list)
{ 
    int min = INT_MAX;
    while(list)
    {	
		if((*(int*)list->e) && ((*(int*)list->e)>0))
		{
		    if(*((int*)list->e) < min)
		    {
		        min = *((int*)list->e);
		    }
		}
		list = list->n;
    }
    return min;
}

/**	
 *	Returns the maximum value of all
 *	values in the queue (history of sensor)
 */
int getMax(AutoQ* list)
{
    int max = INT_MIN;
    while(list->n)
    {
		if(*((int*)list->e) > max)
		{
		   max = *((int*)list->e);
		}
		list = list->n;
    }
    return max;
}

/**	
 *	Returns the mean value of all
 *	values in the queue (history of sensor)
 */
int calcMean(AutoQ* list)
{
    int total = 0, mean = 0, amount = 0;
    while(list->n)
    {
		total += *((int*)list->e);
		amount++;
		list = list->n;
    }
    if(!(total == 0)){ mean = (total/amount); }
    return mean;
}

/**	
 *	Stores the data to the printerfile
 */
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

/**	
 *	Create new thread for printer
 *	if failed print error to log
 */
int StartPrinter(void)
{
    pthread_t printThread;
    if(pthread_create(&printThread, NULL, &getSensorTable, NULL)) return EXIT_FAILURE;
    else return EXIT_SUCCESS;
}
