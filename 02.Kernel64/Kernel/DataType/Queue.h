// Queue data structure

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "DataType/Types64.h"
/*
 * Circular Queue
 */
typedef struct _CircularQueue {
	void*	node;
	int		size;
	int		limit;
	int		idx_enqueue;
	int		idx_dequeue;
} CircularQueue;

#endif
