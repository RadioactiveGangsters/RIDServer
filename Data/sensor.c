/* 
 * File:   sensor.c
 * Author: Bas
 *
 */

#include"sensor.h"

int _sensseed;

void getSensorValue(int const amount, int*const values, unsigned int const maxval)
{
    int i;
    int count = 0;
    
    //Create and set seed for random generator
    _sensseed = rand();
    srand(_sensseed);
    
    for (i = 0; i < amount; i++) 
    {
        //Set new random number in randomValues
        values[count] = (rand()%maxval + 1);
        count++;
    }
}

void initSensorValue(int*const value, unsigned int const maxval)
{
    //Create and set seed for random generator
    _sensseed = rand();
    srand(_sensseed );
    
    //Set random number in value
    *value = (rand()%maxval + 1); 
}

void SetupSensors(void)
{
    srand(time(NULL));
    _sensseed =rand();    
}