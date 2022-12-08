#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#define TRUE 1
#define FALSE 0
#define N 100 // buffer size
#define ITEMS 100000 // # of items
typedef sem_t semaphore;
semaphore mutex;
semaphore empty;
semaphore full;
#define down(A) sem_wait(A)
#define up(A) sem_post(A)
int produce_item();
void consume_item(int consumer_id, int item);
void insert_item(int item);
int remove_item();
void producer(void);
void* consumer(void *cid);
int buff[N];
int count = 0;
int rear = -1;
int front = -1;
int main()
{
 	int i;
 	pthread_t consumer_thr;
 	pthread_attr_t attr;
 	void *status;
 	int j;
 	// Initialize
 	sem_init(&mutex, 0, 1);
 	sem_init(&empty, 0, N);
 	sem_init(&full, 0, 0);
 	for (i = 0; i < N; i++)
 	buff[i] = 0;
 	count = 0;
 	pthread_attr_init(&attr);
 	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM); int cid = 0;
 	// Create a child thread for the consumer 스레드 스케줄링
 	pthread_create(&consumer_thr, &attr, consumer, &cid);//자식스레드 생성
 	producer();
 	pthread_join(consumer_thr, &status);
}
void producer(void)
{
 	int item;
 	static int nitem = 0;
 	while (nitem < ITEMS) {//10만번
 	item = produce_item();
 	down(&empty);
	// down(&mutex);
 	insert_item(item);
	// up(&mutex);
 	up(&full);
 	nitem++;
 	}
 	// Insert 'END' mark
 	down(&empty);
	// down(&mutex);
 	insert_item(-1); // -1 means 'END' 끝났다.
	// up(&mutex);
 	up(&full);
}
void* consumer(void *cid)
{
 	int consumer_id = *(int *) cid;
 	int item;
 	while (TRUE) {
 		down(&full);
		// down(&mutex);
 		item = remove_item();
		// up(&mutex);
		 up(&empty);
 		if (item == -1) // if 'END' mark 끝났다.
			 break;
		 consume_item(consumer_id, item);
 	}
 	pthread_exit(NULL);
}
int produce_item()
{
 	static int item = 0; return item++;
}
void consume_item(int consumer_id, int item)
{
 	// Just consume
 	printf("Consumer %d consumes: %d\n", consumer_id, item);
}
void insert_item(int item)
{
 	rear = (rear + 1) % N;
 	buff[rear] = item;
	// count++;
 	return;
}
int remove_item()
{
 	int item;
 	front = (front + 1) % N;
 	item = buff[front];
	// count--;
 	return item;
}

