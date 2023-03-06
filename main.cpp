#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <time.h>

#define INFILE "numbers.csv"
#define OUTFILE "primes.csv"

bool GenerateNumbers(int maxnum,int limit) {
    std::ofstream file(INFILE,std::ios::out);
    if(file.bad()) {
        printf("Failed to create %s\n",INFILE);
        return 0;
    }
    for(int x = 0;x<maxnum;x++) {
        std::string num = std::to_string(rand() % limit + 1);
        file<<num<<",";
    }
    file<<maxnum<<std::endl;
    file.close();
    return 1;
};

int* ReadNumbers(int size) {
    std::ifstream file;
    file.open(INFILE);
    if(file.bad()) {
        printf("Could not read/open '%s'",INFILE);
        return nullptr;
    }
    int* nums = new int[size];
    int i = 0;
    std::string num;
    while(std::getline(file,num,',')) {
        nums[i] = stoi(num);
        i += 1;
    }
    return nums;
};

bool WritePrimes(int size, int* nums) {
    std::ofstream file(OUTFILE, std::ios::out);
    if (file.bad()) {
        printf("Failed to create %s\n", OUTFILE);
        return 0;
    }
    for (int x = 0; x < size; x++) {
        std::string num = std::to_string(nums[x]);
        file << num << ",";
    }
    file << std::to_string(nums[size]) << std::endl;
    file.close();
    return 1;
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

int main(int argc,char* argv[]) {
    srand(time(NULL));
    int size = 50,limit = 1000;
    if(argc > 2) {
        size = (int)strtol(argv[1],NULL,10);
        limit = (int)strtol(argv[2],NULL,10); 
    }
    bool flag = GenerateNumbers(size,limit);
    if(!flag) {
        return EXIT_FAILURE;
    }
    int* nums = ReadNumbers(size);
    if(nums == NULL) {
        return EXIT_FAILURE;
    }
    /* 
    ***
        QUICKSORT
    ***
    */
    // naive_qsort(nums,0,size-1);

    bool flag2 = WritePrimes(size,nums);
    if(!flag2) {
        return EXIT_FAILURE;
    }
    delete []nums;
    return EXIT_SUCCESS;
}