#include "DataType/Queue.h"

/*
 * Initialize a queue
 */
void kInitializeQueue(Queue* pstQueue, void* pvQueueBuffer,
		int iMaxDataCount, int iDataSize) {
	pstQueue->limit = iMaxDataCount;
	pstQueue->datasize = iDataSize;
	pstQueue->node = pvQueueBuffer;
	pstQueue->idx_enqueue = 0;
	pstQueue->idx_dequeue = 0;

	pstQueue->bLastOperationEnqueue = FALSE;
}

BOOL kIsQueueFull(const Queue* pstQueue) {
	if ((pstQueue->idx_enqueue == pstQueue->idx_dequeue) &&
			(pstQueue->bLastOperationEnqueue == TRUE)) {
		return TRUE;
	}
	return FALSE;
}

BOOL kIsQueueEmpty(const Queue* pstQueue) {
	if ((pstQueue->idx_enqueue == pstQueue->idx_dequeue) &&
			(pstQueue->bLastOperationEnqueue == FALSE)) {
		return TRUE;
	}
	return FALSE;
}

BOOL kEnQueue(Queue* pstQueue, const void* pvData) {
	if (kIsQueueFull(pstQueue) == TRUE) {
		return FALSE;
	}

	kMemCpy ((char*) pstQueue->node + (pstQueue->datasize * pstQueue->idx_enqueue),
			pvData, pstQueue->datasize);

	pstQueue->idx_enqueue = (pstQueue->idx_enqueue + 1) % pstQueue->limit;
	pstQueue->bLastOperationEnqueue = TRUE;
	return TRUE;
}

BOOL kDeQueue(Queue* pstQueue, void* pvData) {
	if (kIsQueueEmpty(pstQueue) == TRUE) {
		return FALSE;
	}

	kMemCpy(pvData, (char*) pstQueue->node + (pstQueue->datasize * pstQueue->idx_dequeue),
			pstQueue->datasize);

	pstQueue->idx_dequeue = (pstQueue->idx_dequeue+1) % pstQueue->limit;
	pstQueue->bLastOperationEnqueue = FALSE;
	return TRUE;
}
