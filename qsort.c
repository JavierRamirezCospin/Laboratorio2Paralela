#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define TREESHOLD 5000

int compare(const void * a, const void * b) {
    return (*(int*)a - *(int*)b);
}
void naive_qsort(int *data, int lo, int hi) //}, int (*compare)(const int *, const int*))
{
  if(lo > hi) return;
  int l = lo;
  int h = hi;
  int p = data[(hi + lo)/2];

  while(l <= h){
    while((data[l] - p) < 0) l++;
    while((data[h] - p) > 0) h--;
    if(l<=h){
      //swap
      int tmp = data[l];
      data[l] = data[h];
      data[h] = tmp;
      l++; h--;
    }
  }
  //recursive call
  naive_qsort(data, lo, h);
  naive_qsort(data, l, hi);
}

void par_qsort(int *data, int lo, int hi) {
    if (lo >= hi) return;

    int l = lo, h = hi, p = data[(hi + lo) / 2];
    while (l <= h) {
        while (data[l] < p) l++;
        while (data[h] > p) h--;
        if (l <= h) {
            int tmp = data[l];
            data[l] = data[h];
            data[h] = tmp;
            l++; h--;
        }
    }

    if ((h - lo) > TREESHOLD) {
        #pragma omp task shared(data) firstprivate(lo, h)
        par_qsort(data, lo, h);
    } else {
        naive_qsort(data, lo, h);
    }

    if ((hi - l) > TREESHOLD) {
        #pragma omp task shared(data) firstprivate(l, hi)
        par_qsort(data, l, hi);
    } else {
        naive_qsort(data, l, hi);
    }

    #pragma omp taskwait
}

int main() {
    int n = 600000;
    int *data = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        data[i] = rand() % 100;
    }

    double start, end;
    start = omp_get_wtime();

    #pragma omp parallel shared(data) num_threads(8)
    {
        #pragma omp single
        par_qsort(data, 0, n - 1);
    }

    end = omp_get_wtime();

    printf("Time: %f\n", end - start);

    // for (int i = 0; i < n; i++) {
    //     printf("%d ", data[i]);
    // }
    

    free(data);
    return 0;
}
