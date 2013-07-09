#include "queue.h"

void init_queue(queue *q)
{
	q->size=INIT_SIZE_QUEUE;
	q->array=malloc(sizeof(void*)*q->size);
	q->first = 0;
	q->last = q->size-1;
	q->count = 0;
}

void init_bigqueue(queue *b, int s)
{
	b->size=s;
	b->array=malloc(sizeof(void*)*b->size);
	b->first=0;
	b->last=b->size-1;
	b->count = 0;
}

void enqueue(queue *q, void*x)
{
    if (q->count >= q->size)
	{
		resize(q);
	}
		if(!Queue_has(q, x))
		{
		//printf("Push: %d\n", *(int*)x);
        q->last = (q->last+1) % q->size;
        q->array[ q->last ] = x;    
        q->count = q->count + 1;
		}
}

void* dequeue(queue *q)
{
    void*x;
    
	if(q->count <= 0) return NULL;
	
        x = q->array[ q->first];
        q->first = (q->first+1) % q->size;
        q->count = q->count - 1;

    return(x);
}

void print(queue *q)
{
	int count = q->first;
	printf("Queue: [");
	do
	{
		if(q->array[count]) printf("%d", *(int*)(q->array[count]));
		printf(", ");
		count++;
	}
	while(count < q->size);
	printf("]\n");
}

bool Queue_has(queue *q, void*x){
	
	int first = q->first;
	
	do
	{
	
		if(q->array[first] == x)
		{
			return true;
		}
		first=(first+1);
	
	}while(first < q->last);
	
	return false;
}

void resize(queue *q)
{
	
	//Making that delicious double sauced queue
	queue i, *temp=&i;
	
	init_bigqueue(temp, q->size*2);
	
	//While those ingredients are hopping on ya whopper
	while(q->count)
	{
		enqueue(temp, dequeue(q));
	}
	
	//Cleaning that shit 
	free(q->array);
	//DONE, get the f*ck out of here
	memcpy(q, temp, sizeof(*temp));
	
	
}
/*
#ifndef FRONTEND_H
int main(void){
	queue i,*q=&i;
	init_queue(q);
	int bliep = 1,blap = 2, blop = 3, blup = 4, blip = 5, bloep = 6, blaup = 7, bluup = 8, blep = 9;
	int xiep = 1,xap = 2, xop = 3, xup = 4, xip = 5, xoep = 6, xaup = 7, xuup = 8, xep = 9;
	
	enqueue(q, &bliep);	print(q);
	enqueue(q, &blap);	print(q);
	enqueue(q, &blop); print(q);
	enqueue(q, &bliep);	print(q);
	
	
	return 0;
	
}
#define FRONTEND_H
#endif
*/
