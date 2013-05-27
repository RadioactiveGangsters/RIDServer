/* 
 * File:   Printer.c
 * Author: Bas
 *
 */

#include "Printer.h"

void getSensorData(Sensor* sensor);
int getMin(int Values[]);
int getMax(int Values[]);
int calcMean(int Values[]);
int storeToFile(char const*const path, char const*const data);


void getSensorData(Sensor* sensor)
{
    int Values[MAXVALUES];//TODO: Get Data from DB    
    char const*const path = fileprinterpath(); //TODO: Set Path of output file
    char *sensorName = sensor->name;
    
    int min = getMin(Values);
    int max = getMax(Values);
    int mean = calcMean(Values);
    
    char data[(34 + SENSOR_HNAMELEN + numlen(min) + numlen(max) + numlen(mean))];
    char buffer[numlen(max)];
    
	//Set array as empty
    data[0] = '\0';
	
    //Add Sensor name
    strcat(data, "Sensor: ");
    strcat(data, sensorName);
	
	//Add Minimum value
    strcat(data," | Min: ");
    snprintf(buffer, sizeof(char)*numlen(max), "%d", min);
    strcat(data, buffer);
	
	//Add Maximum value
    strcat(data," | Max: ");
    snprintf(buffer, sizeof(char)*numlen(max), "%d", max);
    strcat(data, buffer);
	
	//Add Mean value
    strcat(data," | Mean: ");
    snprintf(buffer, sizeof(char)*numlen(max), "%d", mean);
    strcat(data, buffer);
    
	//Write data to file and check if succesfull
    if(!storeToFile(path, data))
    {
        Log(1, "Couldn't store data of %s to file\n", sensorName);
    }
}

int getMin(int Values[])
{
    int min = 0,i;
    for (i = 0; i < MAXVALUES; i++)
    {
        if (i == 0)
        {         
            min = Values[i];
        } 
        else
        {
            if(Values[i] < min)
            {
                 min = Values[i];
            }
        }
    }
    return min;
}

int getMax(int Values[])
{
    int max = 0,i;
    for (i = 0; i < MAXVALUES; i++)
    {
        if (i == 0)
        {         
            max = Values[i];
        } 
        else
        {
            if(Values[i] > max)
            {
                 max = Values[i];
            }
        }
    }
    return max;
}

int calcMean(int Values[])
{
    int total = 0,i,mean;
    for(i = 0; i < MAXVALUES; i++)
    {
        total += Values[i];
    }
    mean = 0;
    if(!(total == 0)){ mean = (total/MAXVALUES); }
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
    
        if((printFile = fopen(path,"a")) == NULL )
        {
            return 0;
        }
        fprintf(printFile, "%s\n", data);

        fclose(printFile);
        return 1;
    }
}

