/* 
 * File:   Printer.h
 * Author: Bas
 *
 */

#ifndef PRINTER_H
#define	PRINTER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "../Data/sensor.h"
#include "../System/Log.h"
#include "../Util/util.h"
#include "../Util/Path.h"

void getSensorData(Sensor* sensor);
int getMin(int Values[]);
int getMax(int Values[]);
int calcMean(int Values[]);
int storeToFile(char const*const path, char const*const data);
void StartPrinter(void);

#endif	/* PRINTER_H */