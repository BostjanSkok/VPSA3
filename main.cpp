#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>


const int M = 20;
const int N = 4;

pthread_barrier_t barrier;

int vekt1[M];
int vekt2[M];
int result[N];

int r;

void FillVektors(){
	for (int i = 0; i < M; i++)
	{
		vekt1[i] = rand()%10;
		vekt2[i] = rand()%10;
	}
}

void* DoWork(void * arg){
	int threadId = (int)arg;
	int start = threadId*(M / N);
	int sum = 0;
	int counter = 2;
	for (int i =start; i < threadId + (M / N); i++)
	{
		sum += vekt1[i] + vekt2[i];
	}

	result[threadId]=sum;
	pthread_barrier_wait(&barrier);
	while (counter<N)
	{
		if (threadId % counter == 1){
			result[threadId] += result[threadId + 1];
		}
		counter * 2;
		pthread_barrier_wait(&barrier);
	}


	


	return NULL;
}


int main() {

	pthread_t threads[N];
	srand(time(NULL));
	
	FillVektors();

	pthread_barrier_init(&barrier, NULL, N);
	for (int i=0; i < N; i++){
		pthread_create(&threads[i],NULL,DoWork,(void*) i);
	}

	for (int i=0; i < N; i++){
		pthread_join(threads[i],NULL);
	}

	pthread_barrier_destroy(&barrier);




}

