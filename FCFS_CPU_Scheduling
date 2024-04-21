#include <stdio.h>
#include <stdlib.h>

typedef struct prcs_t
{
	int pid;
	int arrival_tm;
	int burst_tm;
	int comp_tm;
	int turnAround_tm;
	int wait_tm;
} PRCS_T;

typedef struct prcs_result
{
	int tot_comp_tm;
	int tot_turna_tm;
	int tot_wait_tm;
	double avg_comp_tm;
	double avg_turna_tm;
	double avg_wait_tm;
} PRESULT;


typedef struct prcs_queue_t
{
	PRCS_T** queue;
	size_t que_size;
	ssize_t front;
	ssize_t rear;
} PRCS_QUE;


PRCS_T* PRCS_CREAT(int pid, int arrival_tm, int burst_tm);
void PRCS_RUN(PRCS_QUE* pQUE, PRESULT* res);

int PRCS_QUE_INIT(PRCS_QUE* *dpQUE, size_t nmemb);
int PRCS_QUE_ENQUEUE(PRCS_QUE* pQUE, PRCS_T* pPROC);
PRCS_T* PRCS_QUE_DEQUEUE(PRCS_QUE* pQUE);
int PRCS_QUE_EMPTY(PRCS_QUE* pQUE);
void PRCS_QUE_RELEASE(PRCS_QUE* *dpQUE);


int main() {
	size_t nprcsMemb;
	int arrival_tm;
	int burst_tm;
	PRCS_QUE* RDY_QUEUE;
	PRESULT prcs_tm;

	printf("Enter no. of process: ");
	scanf("%zu", &nprcsMemb);

	if (PRCS_QUE_INIT(&RDY_QUEUE, nprcsMemb)) {
		perror("ERROR [READY-QUEUE]");
		return 1;
	} else {
		printf("READY-QUEUE initialized\n");
	}

	for (size_t i=0; i<nprcsMemb; i++) {
		printf("Process: %zu\n", i+1);
		printf("\tEnter arrival time: ");
		scanf("%d", &arrival_tm);
		printf("\tEnter burst time: ");
		scanf("%d", &burst_tm);
		

		PRCS_T* pPROC = PRCS_CREAT(i+1, arrival_tm, burst_tm);
		if (pPROC == NULL) {
			perror("ERROR [NEW PROCESS]");
			return 2;
		}

		switch(PRCS_QUE_ENQUEUE(RDY_QUEUE, pPROC))
		{
			case -1:
				perror("ERROR [READY-QUEUE]");
				break;
			case -2:
				perror("ERROR [PROCESS QUEUE]");
				break;
			case 0:
				break;
			default:
				perror("ERROR [PROCESS QUEUE OVERFLOW]");
				break;
		}

		printf("\n");
	}

	printf("Running Processes...\n\n");

	PRCS_RUN(RDY_QUEUE, &prcs_tm);

	printf("\n");

	printf("Avg Turn Around Time: %.2lf\n", prcs_tm.avg_turna_tm);
	printf("Avg Waiting Time: %.2lf\n", prcs_tm.avg_wait_tm);

	PRCS_QUE_RELEASE(&RDY_QUEUE);

	return 0;
}


PRCS_T* PRCS_CREAT(int pid, int arrival_tm, int burst_tm) {
	PRCS_T* PPRCS =  (PRCS_T*) malloc(sizeof(PRCS_T));
	PPRCS->pid = pid;
	PPRCS->arrival_tm = arrival_tm;
	PPRCS->burst_tm = burst_tm;

	return PPRCS;
}


void PRCS_RUN(PRCS_QUE* pQUE, PRESULT* res) {
	if (pQUE == NULL) {
		return;
	}

	PRCS_T* pPROC;

	int comp_tm = 0;
	if (res) {
		res->tot_comp_tm = 0;
		res->tot_turna_tm = 0;
		res->tot_wait_tm = 0;
		res->avg_comp_tm = 0;
		res->avg_turna_tm = 0;
		res->avg_wait_tm = 0;
	}

	printf("P.NO\t\tA.T\t\tB.T\t\tC.T\t\tT.A.T\t\tW.T\n\n");

	while (!PRCS_QUE_EMPTY(pQUE)) {
		pPROC = PRCS_QUE_DEQUEUE(pQUE);

		if (comp_tm == 0) {
			comp_tm = pPROC->burst_tm + pPROC->arrival_tm;	
		} else {
			// check if CPU is in IDLE stage
			if (pPROC->arrival_tm > comp_tm) {
				// if CPU gets IDLE then compute new process completion time with increment of IDLE time taken by CPU
				comp_tm += pPROC->arrival_tm - comp_tm;
			}

			comp_tm += pPROC->burst_tm;
		}

		pPROC->comp_tm = comp_tm;
		pPROC->turnAround_tm = pPROC->comp_tm - pPROC->arrival_tm;
		pPROC->wait_tm = pPROC->turnAround_tm - pPROC->burst_tm;


		printf(
				"%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", // P.NO, A.T, B.T, C.T, T.A.T, W.T
				pPROC->pid,
				pPROC->arrival_tm,
				pPROC->burst_tm,
				pPROC->comp_tm,
				pPROC->turnAround_tm,
				pPROC->wait_tm
			 );

		if (res) {
			res->tot_comp_tm += pPROC->comp_tm;
			res->tot_turna_tm += pPROC->turnAround_tm;
			res->tot_wait_tm += pPROC->wait_tm;
		}

		free(pPROC);
		pPROC = NULL;
	}

	if (res) {
		res->avg_comp_tm = (double) res->tot_comp_tm/pQUE->que_size;
		res->avg_turna_tm = (double) res->tot_turna_tm/pQUE->que_size;
		res->avg_wait_tm = (double) res->tot_wait_tm/pQUE->que_size;
	}
}


int PRCS_QUE_INIT(PRCS_QUE* *dpQUE, size_t nmemb) {
	(*dpQUE) = (PRCS_QUE*) malloc(sizeof(PRCS_QUE));
	if (*dpQUE == NULL) {
		return -1;
	}

	(*dpQUE)->queue = (PRCS_T**) calloc(nmemb, sizeof(PRCS_T*));
	if ((*dpQUE)->queue == NULL) {
		return -2;
	}

	(*dpQUE)->que_size = nmemb;
	(*dpQUE)->front = -1;
	(*dpQUE)->rear = -1;

	return 0;
}


int PRCS_QUE_ENQUEUE(PRCS_QUE* pQUE, PRCS_T* pPROC) {
	if (pQUE == NULL) {
		return -1;
	}

	if (pQUE->queue == NULL) {
		return -2;
	}

	if (pQUE->front == -1 && pQUE->rear == -1) {
		pQUE->front++;
	}

	pQUE->rear++;
	if (pQUE->rear >= (ssize_t) pQUE->que_size) {
		return pQUE->rear;
	}


	if (pQUE->rear == 9) {
		pQUE->queue[0] = pPROC;
	} else {
		ssize_t j = pQUE->rear-1;
		while (j>=0 && pQUE->queue[j]->arrival_tm > pPROC->arrival_tm) {
			pQUE->queue[j+1] = pQUE->queue[j];
			j--;
		}

		pQUE->queue[j+1] = pPROC; 
	}

	return 0;
}


PRCS_T* PRCS_QUE_DEQUEUE(PRCS_QUE* pQUE) {
	if (pQUE == NULL || pQUE->queue == NULL) {
		return NULL;
	}

	if (pQUE->front == pQUE->rear) {
		PRCS_T* pPROC = pQUE->queue[pQUE->front];
		pQUE->queue[pQUE->front] = NULL;
		pQUE->front = -1;
		pQUE->rear = -1;

		return pPROC;
	} else if (pQUE->front > pQUE->rear) {
		pQUE->front = -1;
		pQUE->rear = -1;

		return NULL;
	}
	
	PRCS_T* pPROC = pQUE->queue[pQUE->front];
	pQUE->queue[pQUE->front++] = NULL;

	return pPROC;
}


int PRCS_QUE_EMPTY(PRCS_QUE* pQUE) {
	if (pQUE == NULL) {
		return -1;
	} else if (pQUE->queue == NULL) {
		return -2;
	}

	if (pQUE->front == -1 && pQUE->rear == -1) {
		return 1;
	}
	
	return 0;
}


void  PRCS_QUE_RELEASE(PRCS_QUE* *dpQUE) {
	if ((*dpQUE)->queue) {
		free((*dpQUE)->queue);
		(*dpQUE)->queue = NULL;
	}

	if (*dpQUE) {
		free(*dpQUE);
		*dpQUE = NULL;
	}
}
