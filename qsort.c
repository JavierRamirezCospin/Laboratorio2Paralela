#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define TREESHOLD 10000

int compare(const void * a, const void * b) {
    return (*(int*)a - *(int*)b);
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
        qsort(&data[lo], h - lo + 1, sizeof(int), compare);
    }

    if ((hi - l) > TREESHOLD) {
        #pragma omp task shared(data) firstprivate(l, hi)
        par_qsort(data, l, hi);
    } else {
        qsort(&data[l], hi - l + 1, sizeof(int), compare);
    }

    #pragma omp taskwait
}

int main() {
    int n = 60;
    int *data = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        data[i] = rand() % 100;
    }

    double start, end;
    start = omp_get_wtime();

    #pragma omp parallel shared(data) num_threads(4)
    {
        #pragma omp single
        par_qsort(data, 0, n - 1);
    }

    end = omp_get_wtime();

    printf("Time: %f\n", end - start);

    for (int i = 0; i < n; i++) {
        printf("%d ", data[i]);
    }
    

    free(data);
    return 0;
}
