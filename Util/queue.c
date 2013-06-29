init_queue(queue *q)
{
	q->size=INIT_SIZE_QUEUE;
	q->array=malloc(sizeof(void*)*q->size));
	q->first = 0;
	q->last = q->size-1;
	q->count = 0;
}

init_bigqueue(queue *b, int s)
{
	b->size=s;
	b->array=malloc(sizeof(void*)*b->size));
	b->first=0;
	b->last=b->size-1;
	b->count = 0;
}

enqueue(queue *q, void*x)
{
        if (q->count >= q->size)
	{
		printf("Warning: queue overflow enqueue x=%d\n",x);
		//TODO: Make it Dynamic!!
		//resize();
	}
        else 
	{
                q->last = (q->last+1) % q->size;
                q->array[ q->last ] = x;    
                q->count = q->count + 1;
        }
}

void* dequeue(queue *q)
{
        void*x;

        if (q->count <= 0) printf("Warning: empty queue dequeue.\n");
        else 
	{
                x = q->array[ q->first];
                q->first = (q->first+1) % q->size;
                q->count = q->count - 1;
        }

        return(x);
}

void resize(queue *q)
{
	// HOWTO: FIXME:
	// TODO:
	// init a twice-as-big queue
	// while q has elements
	// 	enqueue into twice-as-big queue the dequeued element of q
	// free q->array (it is empty now)
	// memcpy into q twice-as-big-queue
	
	// the problem was that we did not need a temparray, but a tempqueue
	// it can be done with temparray, but to do that beautifully you need Level 2 Pointermagic (try it sometime!)
	
	//Making that delicious double sauced queue
	init_bigqueue(temp, q->size*2);
	
	
	//While those ingredients are hopping on ya whopper
	while(q->count)
	{
		enqueue(temp, dequeue(q));
	}
	
	//Cleaning that shit 
	free(q->array):
	//DONE, get the f*ck out of here
	memcpy(q, temp, sizeof(temp));
	
	
/*	prev solution:
   	q->size = q->size*2;
	void**temparray=malloc(sizeof(void*)*q-size));
	
	void**p=temparray+q->first;
	while(q->count) 
	{	
		enqueue(q->temparray, dequeue(q->array));
	}
	free(q->array);
	q->array = temparray
*/
}


-
