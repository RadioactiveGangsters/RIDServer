
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#define	INIT_SIZE_QUEUE 8

typedef struct {
	    void**array;				
        int first;                      /* position of first element */
        int last;                       /* position of last element */
        int count;                      /* number of queue elements */
	int size;
} queue;
