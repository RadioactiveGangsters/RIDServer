
init_queue(queue *q)
{
	q->size=INIT_SIZE_QUEUE;
	q->array=malloc(sizeof(void*)*q->size));
        q->first = 0;
        q->last = q->size-1;
        q->count = 0;
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
	int seccount = 0;
	q->size = q->size*2;
	q->array=malloc(sizeof(void*)*q-size));
	// !=0 kan eigenlijk weg toch?
	while(q->count != 0) 
	{		
	enqueue(q->temparray, dequeue(q-array));
	seccount++;
	}

	free(q->array);

	while(seccount != 0)
	{	
		enqueue(q->array, dequeue(q->temparray));
		seccount--;
	}
}

