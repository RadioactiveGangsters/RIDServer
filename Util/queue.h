

#include "queue.h"
#include "bool.h"



#define	INIT_SIZE_QUEUE 8

typedef struct {
	void**temparray;
        void**array;				
        int first;                      /* position of first element */
        int last;                       /* position of last element */
        int count;                      /* number of queue elements */
	int size;
} queue;
