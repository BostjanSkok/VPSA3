#include <stdio.h>
#include <pthread.h>


const int M = 20;
const int N = 4;

int vekt1[M];
int vekt2[M];
int result[M];


void FillVektors(){}

void* DoWork(void * arg){


	return NULL;
}


int main() {

	pthread_barrier_t barrier;
	pthread_t threads[N];

	
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

