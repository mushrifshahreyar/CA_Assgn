#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include<time.h>
#define MAX 10
int first [MAX][MAX], second[MAX][MAX], sum[MAX][MAX];
int rows, columns;
void print_matrix(int arr[MAX][MAX]) {
  int i,j;
  for (i=0;i<rows;i++) {
    for (j=0;j<columns;j++)  {
      printf("%d ", arr[i][j]);
    }
    printf("\n");
  }
}
void input(int arr[10][10], int rows, int columns) {
  int i,j;
  for (i=0; i<rows; i++) {
    for (j=0; j<columns; j++) {
      scanf("%d", &arr[i][j]);
    }
  }
}
void generate_input (int arr[MAX][MAX], int rows, int columns) {
  int i,j;
  for (i=0; i<rows; i++) {
    for (j=0; j<columns; j++) {
      arr[i][j] = rand()%MAX;
    }
  }
}
void calc_sum (int arr1[MAX][MAX], int arr2[MAX][MAX]) {
  int i,j,tid;
  #pragma omp parallel private (tid) shared (i,j)
  #pragma omp for
  for (i=0;i<rows;i++){
    for (j=0;j<columns;j++){
      tid = omp_get_thread_num();
      printf("From thread %d \n", tid);
      sum[i][j] = first[i][j]+second[i][j];
    }
  printf("no of threads %d   \n ", omp_get_num_threads());
  }
  tid = omp_get_thread_num();
  printf("From thread %d\n", tid);
  print_matrix(sum);
}

int main () {
  printf ("\nEnter the number of rows and columns for the matrix ");
  scanf ("%d %d",&rows,&columns);
  printf ("\nEnter the elements of the first matrix");
  generate_input(first, rows,columns);
  print_matrix(first);
  printf ("\nEnter the elements of the second matrix ");
  generate_input(second, rows, columns);
  print_matrix(second);
  printf ("\nThe sum of the matrices is \n");
  clock_t begin = clock();
  calc_sum(first,second);
  clock_t end = clock();
  double time_spent = (double)(end - begin);
  printf("%f\n",time_spent);

}