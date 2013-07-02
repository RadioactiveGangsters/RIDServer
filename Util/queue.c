#include "queue.h"

init_queue(queue *q)
{
	q->size=INIT_SIZE_QUEUE;
	q->array=malloc(sizeof(void*)*q->size);
	q->first = 0;
	q->last = q->size-1;
	q->count = 0;
}

init_bigqueue(queue *b, int s)
{
	b->size=s;
	b->array=malloc(sizeof(void*)*b->size);
	b->first=0;
	b->last=b->size-1;
	b->count = 0;
}

enqueue(queue *q, void*x)
{
    if (q->count >= q->size)
	{
		resize(q);
	}
  
		//printf("Push: %d\n", *(int*)x);
        q->last = (q->last+1) % q->size;
        q->array[ q->last ] = x;    
        q->count = q->count + 1;
}

void* dequeue(queue *q)
{
    void*x;
	
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
#ifndef FRONTEND_H

int main(void){
	queue i,*q=&i;
	init_queue(q);
	int bliep = 1,blap = 2, blop = 3, blup = 4, blip = 5, bloep = 6, blaup = 7, bluup = 8, blep = 9;
	int xiep = 1,xap = 2, xop = 3, xup = 4, xip = 5, xoep = 6, xaup = 7, xuup = 8, xep = 9;
	
	enqueue(q, &bliep);	print(q);
	enqueue(q, &blap);	print(q);
	enqueue(q, &blop);	print(q);
	enqueue(q, &blup);	print(q);
	enqueue(q, &blip);	print(q);
	enqueue(q, &bloep);	print(q);
	enqueue(q, &blaup);	print(q);
	enqueue(q, &bluup);	print(q);
	enqueue(q, &blep);	print(q);
	
	enqueue(q, &xiep);	print(q);
	enqueue(q, &xap);	print(q);
	enqueue(q, &xop);	print(q);
	enqueue(q, &xup);	print(q);
	enqueue(q, &xip);	print(q);
	enqueue(q, &xoep);	print(q);
	enqueue(q, &xaup);	print(q);
	enqueue(q, &xuup);	print(q);
	enqueue(q, &xep);	print(q);
	
	
	printf("Pop: %d\n", *(int*)dequeue(q));	print(q);
	printf("Pop: %d\n", *(int*)dequeue(q));	print(q);
	printf("Pop: %d\n", *(int*)dequeue(q));	print(q);
	printf("Pop: %d\n", *(int*)dequeue(q));	print(q);
	printf("Pop: %d\n", *(int*)dequeue(q));	print(q);
	printf("Pop: %d\n", *(int*)dequeue(q));	print(q);
	printf("Pop: %d\n", *(int*)dequeue(q));	print(q);
	printf("Pop: %d\n", *(int*)dequeue(q));	print(q);
	
	
	return 0;
	
}
#define FRONTEND_H
#endef
