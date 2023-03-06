#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <time.h>
#include <omp.h>

#define INFILE "numbers.csv"
#define OUTFILE "primes.csv"

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
    int num_thds = 1;
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
    #pragma omp parallel num_threads(size)
    WritePrimes(size,nums);

    delete []nums;
    return EXIT_SUCCESS;
}