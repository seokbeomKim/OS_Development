// Queue data structure

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "DataType/Types64.h"
/*
 * Circular Queue
 */
typedef struct _CircularQueue {
	void*	node;
	int		datasize;
	int		limit;
	int		idx_enqueue;
	int		idx_dequeue;

	BOOL	bLastOperationEnqueue;
} Queue;

void kInitializeQueue(Queue* pstQueue, void* pvQueueBuffer,
		int iMaxDataCount, int iDataSize);
BOOL kIsQueueFull(const Queue* pstQueue);
BOOL kIsQueueEmpty(const Queue* pstQueue);
BOOL kEnQueue(Queue* pstQueue, const void* pvData);
BOOL kDeQueue(Queue* pstQueue, void* pvData);

#endif
