

#include "queue.h"
#include "bool.h"


#define QUEUESIZE       1000

typedef struct {
        void*q[QUEUESIZE+1];		/* body of queue */
        int first;                      /* position of first element */
        int last;                       /* position of last element */
        int count;                      /* number of queue elements */
} queue;
