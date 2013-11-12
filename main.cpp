#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>


const int M = 16;
const int N = 2;
pthread_mutex_t locks[N];
pthread_barrier_t barrier;

int vekt1[M];
int vekt2[M];
int result[N];



void FillVektors(){
	for (int i = 0; i < M; i++)
	{
		//vekt1[i] = rand()%10;
		//vekt2[i] = rand()%10;
		vekt1[i] = 1;
		vekt2[i] = 2;
	}
}

void printVector()
{
	for (int i = 0; i < N; i++)
	{
		printf("%i;", result[i]);
	}
	printf("\n");
}



void* DoWork2(void * arg){
	int threadId = (int)arg;
	int start = threadId*(M / N);
	int sum = 0;
	int stepSize = 2;
	for (int i = start; i < (threadId + 1) * (M / N); i++)
	{
		sum += vekt1[i] * vekt2[i];
	}

	result[threadId] = sum;
	
	pthread_barrier_wait(&barrier);
	//if (threadId == 0){ printVector(); }
	//pthread_barrier_wait(&barrier);

	while (stepSize <= N)
	{

		if (threadId % stepSize == 0 && threadId + (stepSize / 2) < N){
			//	printf("thread %i", threadId);
			result[threadId] += result[threadId + (stepSize / 2)];
			//printf(" value = %i\n", result[threadId]);
		}
		stepSize *= 2;
		pthread_barrier_wait(&barrier);
	
		//if (threadId == 0){ printVector(); }
		//pthread_barrier_wait(&barrier);
	}



	return NULL;
}

void* DoWork(void * arg){
	int threadId = (int)arg;
	int start = threadId*(M / N);
	int sum = 0;
	int stepSize = 2;
	for (int i = start; i < (threadId + 1) * (M / N); i++)
	{
		sum += vekt1[i] * vekt2[i];
	}

	result[threadId] = sum;


	//if (threadId == 0){ printVector(); }
	//pthread_barrier_wait(&barrier);

	while (stepSize <= N)
	{

		if (threadId % stepSize == 0 && threadId + (stepSize / 2) < N){
			//	printf("thread %i", threadId);
			pthread_mutex_lock(&locks[threadId + (stepSize / 2)]);
			
			result[threadId] += result[threadId + (stepSize / 2)];

			pthread_mutex_unlock(&locks[threadId + (stepSize / 2)]);

			//printf(" value = %i\n", result[threadId]);
		}
		else
		{
			pthread_mutex_unlock(&locks[threadId]);
		}
		stepSize *= 2;

		//if (threadId == 0){ printVector(); }
		//pthread_barrier_wait(&barrier);
	}



	return NULL;
}
int main() {

	pthread_t threads[N];
	srand(time(NULL));
	

	FillVektors();

	for (int i = 0; i < N; i++){
		pthread_mutex_init(&locks[i], NULL);
	}

	pthread_barrier_init(&barrier, NULL, N);
	for (int i = 0; i < N; i++){
		pthread_mutex_lock(&locks[i]);
		pthread_create(&threads[i], NULL, DoWork, (void*)i);
	}

	for (int i = 0; i < N; i++){
		pthread_join(threads[i], NULL);
	}

	printf("%i \n", result[0]);
	getchar();
	pthread_barrier_destroy(&barrier);



}








