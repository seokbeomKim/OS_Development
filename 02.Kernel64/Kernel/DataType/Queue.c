#include <DataType/Queue.h>
#include <Include/Mem.h>

CircularQueue*
kNewCircularQueue(void* mem, int limit, int data_size) {
	CircularQueue* queue = (CircularQueue *) queue;
	queue->idx_enqueue = 0;
	queue->idx_dequeue = 0;

	queue->base.limit	= limit;
	queue->base.size	= 0;
	queue->base.data	= NULL;
	queue->base.node_size = data_size;
	queue->wasEnqueue	= FALSE;

	queue->dequeue	= (void*)kCircularQueue_Dequeue;
	queue->enqueue 	= (void*)kCircularQueue_Enqueue;
	queue->isFull	= (void*)kCircularQueue_IsFull;
	queue->isEmpty	= (void*)kCircularQueue_IsEmpty;
}

/*
 * Circular Queue 관련 메서드
 */
BOOL
kCircularQueue_Enqueue(CircularQueue* queue, void* data) {
	if (queue->isFull(queue) == TRUE) {
		// Queue에 자료 추가 불가능
		return FALSE;
	}
	kMemCpy((char*)queue->base.data + queue->base.node_size * queue->idx_enqueue,
			data,
			queue->base.node_size);
	queue->idx_enqueue	= (queue->idx_enqueue + 1) % queue->base.limit;
	queue->wasEnqueue	= TRUE;

	return TRUE;
}

BOOL
kCircularQueue_Dequeue(CircularQueue* queue, void* retValue) {
	if (queue->isFull(queue) == TRUE) {
		return FALSE;
	}
	kMemCpy(retValue,
			queue->base.data + queue->base.node_size * queue->idx_dequeue,
			queue->base.node_size);
	queue->idx_dequeue	= (queue->idx_dequeue + 1) % queue->base.limit;
	queue->wasEnqueue	= FALSE;

	return TRUE;
}

BOOL
kCircularQueue_IsFull(CircularQueue* queue) {
	return kIsQueueFull(&queue->base);
}

BOOL
kCircularQueue_IsEmpty(CircularQueue* queue) {
	return kIsQueueEmpty(&queue->base);
}

/*
 * 여기서 사용되는 Queue는 큐 종류의 base 부분을 가지고 판단한다.
 */
BOOL
kIsQueueFull(struct _Queue* queue) {
	if (queue->size == queue->limit) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL
kIsQueueEmpty(struct _Queue* queue) {
	if (queue->size == 0) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}
