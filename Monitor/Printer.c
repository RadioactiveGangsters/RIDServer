/* 
 * File:   Printer.c
 * Author: Bas
 *
 */

#include "Printer.h"

void getSensorData(Sensor* sensor)
{
    int Values[4320000];//TODO Get Data from DB    
    char const*const path = fileprinterpath(); //Has to be somewhere else
    char *sensorName = sensor->name;
    
    int min = getMin(Values);
    int max = getMax(Values);
    int mean = calcMean(Values);
    
    char data[(28 + SENSOR_HNAMELEN + numlen(min) + numlen(max) + numlen(mean))];
    strcat(data, "Sensor: ");
    strcat(data, sensorName);
    strcat(data," Min: ");
    sprintf(data, "%d", min);
    strcat(data," Max: ");
    sprintf(data, "%d", max);
    strcat(data," Mean: ");
    sprintf(data, "%d", mean);
    
    if(!storeToFile(path, data))
    {
        Log(1, "Couldn't store data of %s to file\n", sensorName);
    }
}

int getMin(int Values[])
{
    int min = 0;
    for (int i = 0; i < 4320000; i++)
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
    int max = 0;
    for (int i = 0; i < 4320000; i++)
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
    int total = 0;
    for(int i = 0; i < 4320000; i++)
    {
        total += Values[i];
    }
    int mean = 0;
    if(!(total == 0)){ mean = (total/4320000); }
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

void *printSensors(void *param)
{
    printf("%s \n", param);
}

void StartPrinter()
{
    pthread_t printThread;
    char *param = "Thread started";
    
    if(pthread_create(&printThread, NULL, printSensors, (void*) param)) 
    {
        printf("Error creating thread for printer.\n"); 
    }
    else
    {
        printf("Printer Thread started.\n"); 
    }
    
}




