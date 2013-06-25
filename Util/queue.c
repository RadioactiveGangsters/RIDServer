
init_queue(queue *q)
{
        q->first = 0;
        q->last = QUEUESIZE-1;
        q->count = 0;
}

enqueue(queue *q, void*x)
{
        if (q->count >= QUEUESIZE)
	{
		printf("Warning: queue overflow enqueue x=%d\n",x);
		//TODO: Make it Dynamic!!
		//resize();
	}
        else 
	{
                q->last = (q->last+1) % QUEUESIZE;
                q->q[ q->last ] = x;    
                q->count = q->count + 1;
        }
}

int dequeue(queue *q)
{
        void*x;

        if (q->count <= 0) printf("Warning: empty queue dequeue.\n");
        else 
	{
                x = q->q[ q->first ];
                q->first = (q->first+1) % QUEUESIZE;
                q->count = q->count - 1;
        }

        return(x);
}
