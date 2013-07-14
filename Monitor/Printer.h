/* 
 * File:   Printer.h
 * Author: Bas
 *
 */

#ifndef PRINTER_H
#define	PRINTER_H
#define MAXVALUES 43200

#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "../deps/iniparser/src/iniparser.h"

#include "../Data/sensor.h"
#include "../Data/Database.h"
#include "../System/Log.h"
#include "../Util/util.h"
#include "../Util/Path.h"
#include "../Util/Rest.h"
#include "../Util/Trie.h"
#include "../Util/DeathRow.h"

void getSensorData(Trie* sensor);
void getSensors(Trie* table);
void *getSensorTable(void *param);
int getMin(AutoQ* list);
int getMax(AutoQ* list);
int calcMean(AutoQ* list);
int storeToFile(char const*const path, char const*const data);
int StartPrinter(void);

#endif	/* PRINTER_H */
