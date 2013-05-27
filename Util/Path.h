#ifndef RPATH_H
#define RPATH_H

typedef struct
{
	char sensor[256];
} _path;

char*sensorinipath(void);
char*fileprinterpath(void);

#endif