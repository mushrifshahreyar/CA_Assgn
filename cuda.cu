#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cuda.h>

#define row 10000
#define col 10000

int matrixA[row][col], matrixB[row][col], matrixC[row][col], matrixD[row][col];

__global__ void add_matrix(int matrixA[row][col], int matrixB[row][col], int matrixC[row][col])
{
	int i = blockDim.x*blockIdx.x + threadIdx.x;
	int j = blockDim.y*blockIdx.y + threadIdx.y;

	if (i < row && j < col)
	{
		matrixC[i][j] = matrixA[i][j] + matrixB[i][j];
	}
}

int main()
{
	
	int(*deviceA)[col];
	int(*deviceB)[col];
	int(*deviceC)[col];
	int i, j;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			matrixA[i][j] = rand() % 100;
			matrixB[i][j] = rand() % 100;
		}
	}
	/*for (i = 0; i < row; i++)
	{
	for (j = 0; j < col; j++)
	{
	printf("%d ", matrixA[i][j]);
	}
	printf("\n");
	}*/
	cudaEvent_t start_time, stop_time;
	float elapsedTime;
	

	//clock_t start_time = clock();
	cudaMalloc((void **)&deviceA, row * col * sizeof(int));
	cudaMalloc((void **)&deviceB, row * col * sizeof(int));
	cudaMalloc((void **)&deviceC, row * col * sizeof(int));

	cudaMemcpy(deviceA, matrixA, row * col * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(deviceB, matrixB, row * col * sizeof(int), cudaMemcpyHostToDevice);

	dim3 threadsPerBlock(32, 32);
	dim3 numOfBlocks(ceil(row / 32.0), ceil(col / 32.0));
	cudaEventCreate(&start_time);
	cudaEventRecord(start_time, 0);

	add_matrix <<<numOfBlocks, threadsPerBlock >>> (deviceA, deviceB, deviceC);
	
	cudaEventCreate(&stop_time);
	cudaEventRecord(stop_time, 0);
	cudaEventSynchronize(stop_time);

	cudaEventElapsedTime(&elapsedTime, start_time, stop_time);
	cudaMemcpy(matrixC, deviceC, row*col * sizeof(int), cudaMemcpyDeviceToHost);

	//clock_t stop_time = clock();
	
	printf(" Parallely Elapsed Time: %f ms\n", elapsedTime);

	
	clock_t start_time_nonparallely, stop_time_nonparallely;
	start_time_nonparallely = clock();

	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			matrixD[i][j] = matrixA[i][j] + matrixB[i][j];
		}
	}
	stop_time_nonparallely = clock();
	printf("Non-parallely Elapsed Time: %f ms\n", (float)((stop_time_nonparallely) - (start_time_nonparallely)));
	
	cudaFree(deviceA);
	cudaFree(deviceB);
	cudaFree(deviceC);
}