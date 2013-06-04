/*
 * File:   Printer.c
 * Author: Bas
 *
 */

#include "Printer.h"

void getSensorData(Trie* s)
{
    char const*const path = fileprinterpath(); 
    char *sensorName = ((Sensor*)s->e)->name;
    
    int min = getMin(((Sensor*)s->e)->delta);
    int max = getMax(((Sensor*)s->e)->delta);
    int mean = calcMean(((Sensor*)s->e)->delta);
    
    char data[(34 + SENSOR_HNAMELEN + numlen(min) + numlen(max) + numlen(mean))];
    char buffer[numlen(max)];
    
    //Set array as empty
    data[0] = '\0';
	
    //Add Sensor name
    strcat(data, "Sensor: ");
    strcat(data, sensorName);
	
    //Add Minimum value
    strcat(data," | Min: ");
    snprintf(buffer, sizeof(char)*numlen(min)+1, "%1d", min);
    strcat(data, buffer);
	
    //Add Maximum value
    strcat(data," | Max: ");
    snprintf(buffer, sizeof(char)*numlen(max)+1, "%1d", max);
    strcat(data, buffer);
	
    //Add Mean value
    strcat(data," | Mean: ");
    snprintf(buffer, sizeof(char)*numlen(mean)+1, "%1d", mean);
    strcat(data, buffer);
    
    if(!storeToFile(path, data))
    {
        Log(LOGL_ERROR, "Couldn't store data of %s to file\n", sensorName);
    }
}

void getSensors(Trie* t)
{
    // Get all sensors from sensortable and for each sensor do getSensorData()
    fortrie(t->e , &getSensorData);
}

void *getSensorTable(void *param)
{
    usleep(60000000); // 10 sec sleep to wait for some data to be generated

    while(1)
    {
        // Get all sensors types from table and for each type do printTable()
        fortrie(Tables(), &getSensors);
        usleep(600000000); // 10 min sleep
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
        if((printFile = fopen(path,"a")) == NULL )
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
        Log(LOGL_ERROR, "Error creating thread for printer\n");
    }
    else
    {
        Log(LOGL_DEBUG, "Printer Started\n"); 
    }
    
}
