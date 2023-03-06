// Ejemplo de QuickSort
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

void par_qsort(int *data, int lo, int hi)
{
  if (lo >= hi) return; // base case
  int l = lo;
  int h = hi;
  int p = data[(hi + lo)/2];
  while(l <= h){
    while(data[l] < p) l++;
    while(data[h] > p) h--;
    if(l <= h){
      //swap
      int tmp = data[l];
      data[l] = data[h];
      data[h] = tmp;
      l++; h--;
    }
  }
  //recursive call
  # pragma omp task shared(data)
  par_qsort(data, lo, h);
  # pragma omp task shared(data)
  par_qsort(data, l, hi);

  # pragma omp taskwait
}

int main() {
    int n = 10000000;
    int *data = malloc(n * sizeof(int));
    if (data == NULL) {
        printf("Failed to allocate memory for data\n");
        return 1;
    }
    for (int i = 0; i < n; i++) {
      data[i] = rand() % 100;
    }
    double start, end;
    start = omp_get_wtime();
    # pragma omp parallel shared(data) num_threads(1)
    {
      # pragma omp single
      par_qsort(data, 0, n-1);
    }
    end = omp_get_wtime();
    printf("Time: %f \n", end - start);
    free(data);
    return 0;
}
