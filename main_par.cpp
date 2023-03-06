#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <time.h>
#include <omp.h>

#define INFILE "numbers.csv"
#define OUTFILE "primes.csv"

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


void GenerateNumbers(int maxnum,int limit) {
    std::ofstream file;
    #pragma omp master
    {
        file.open(INFILE,std::ios::out);
    }
    #pragma omp barrier
    if(file.bad()) {
        printf("Failed to create %s\n",INFILE);
        exit(EXIT_FAILURE);
    }
    #pragma omp parallel
    {
        #pragma omp for ordered
        for(int x = 0;x<maxnum-1;x++)
            #pragma omp ordered
            file<<std::to_string(rand() % limit + 1)<<",";
    }
    file<<std::to_string(rand() % limit + 1)<<std::endl;
    file.close();
};

void ReadNumbers(int size,int* nums) {
    std::ifstream file;
    file.open(INFILE);
    if(file.bad()) {
        printf("Could not read/open '%s'",INFILE);
        exit(EXIT_FAILURE);
    }
    int i = 0;
    std::string num;
    while(std::getline(file,num,',')) {
        nums[i] = stoi(num);
        i += 1;
    }
};

void WritePrimes(int size,int* nums) {
    std::ofstream file;
    #pragma omp master
    {
        file.open(OUTFILE,std::ios::out);
    }
    #pragma omp barrier
    if(file.bad()) {
        printf("Failed to create %s\n",INFILE);
        exit(EXIT_FAILURE);
    }
    #pragma omp parallel
    {
        int x;
        #pragma omp for ordered private(x)
        for(x = 0;x<size-1;x++)
            #pragma omp ordered
            file<<std::to_string(nums[x])<<",";
    }
    file<<std::to_string(nums[size-1])<<std::endl;
    file.close();
}

int main(int argc,char* argv[]) {
    srand(time(NULL));
    int size = 50,limit = 1000;
    int num_thds = 8;
    if(argc > 3) {
        size = (int)strtol(argv[1],NULL,10);
        limit = (int)strtol(argv[2],NULL,10);
        num_thds = (int)strtol(argv[3],NULL,10);
    }
    int* nums = new int[size];

    #pragma omp parallel num_threads(num_thds)
    GenerateNumbers(size,limit);

    ReadNumbers(size,nums);
    /* 
    ***
        QUICKSORT
    ***
    */
   #pragma omp parallel shared(nums) num_threads(num_thds)
    {
        #pragma omp single
        par_qsort(nums, 0, size - 1);
    }
    #pragma omp parallel num_threads(size)
    WritePrimes(size,nums);

    delete []nums;
    return EXIT_SUCCESS;
}