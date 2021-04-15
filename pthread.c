#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define T 2
#define N 4

int A[N][N];
int B[N][N];
int C[N][N] = {{0}};

void *matrix_add(void *arg) {
	int id = *(int*)arg;
	int i, j;

	for (i = id; i < N; i += T)
		for (j = 0; j < N; j++)
			C[i][j] = A[i][j] + B[i][j];

	pthread_exit(NULL);
}

int main()
{
    pthread_t thread[T];
    int tid[T];
	int i, j;

    // Generating random values in mat_A and mat_B
	for (i = 0; i < N; i++) {

		for (j = 0; j < N; j++) {

			A[i][j] = rand() % 10;
			B[i][j] = rand() % 10;

		}

	}
    clock_t begin = clock();
	for (i = 0; i < T; i++) {
		tid[i] = i;
		pthread_create(&thread[i], NULL, matrix_add, &tid[i]);
	}
    
	for (i = 0; i < T; i++)
		pthread_join(thread[i], NULL);

    clock_t end = clock();
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			printf((j < N-1) ? "%d " : "%d\n", C[i][j]);

    
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n",time_spent);
	return 0;
}
