#ifndef RPATH_H
#define RPATH_H

typedef struct
{
	char sensor[256];
} _path;

char*networkinipath(void);
char*sensorinipath(void);
char*printerinipath(void);
char*fileprinterpath(void);

#endif
