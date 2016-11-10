// Queue data structure

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <DataType/Types64.h>

// 큐 공통
typedef struct _Queue {
	void*	data;
	int		size;			// the number of data queue has
	int		limit;			// maximum number of data
	int		node_size;		// size of data
} Queue;

/*
 * Circular Queue - Queue 상속
 */
typedef struct _CircularQueue {
	union {
		struct _Queue base;
	};
	int		idx_enqueue;
	int		idx_dequeue;
	BOOL	wasEnqueue;		// 마지막 연산이 enqueue인지 아닌지를 판단

	BOOL (*dequeue)(struct _CircularQueue*, void*);
	BOOL (*enqueue)(struct _CircularQueue*, void*);
	BOOL (*isFull)(struct _CircularQueue*);
	BOOL (*isEmpty)(struct _CircularQueue*);
} CircularQueue;

// 편하게 줄여서 CQueue라고 한다.
CircularQueue*	kNewCircularQueue(void* mem, int limit, int data_size);
BOOL	kCircularQueue_Enqueue(CircularQueue* queue, void* data);
BOOL	kCircularQueue_Dequeue(CircularQueue* queue, void* retValue);
BOOL	kCircularQueue_IsFull(CircularQueue* queue);
BOOL	kCircularQueue_IsEmpty(CircularQueue* queue);

BOOL	kIsQueueFull(struct _Queue* queue);
BOOL	kIsQueueEmpty(struct _Queue* queue);

#endif
