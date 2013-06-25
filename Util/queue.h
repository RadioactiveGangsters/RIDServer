

#include "queue.h"
#include "bool.h"



#define	INIT_SIZE_QUEUE 8

typedef struct {
        void**array;				/* body of queue */
        int first;                      /* position of first element */
        int last;                       /* position of last element */
        int count;                      /* number of queue elements */
	int size;
} queue;
