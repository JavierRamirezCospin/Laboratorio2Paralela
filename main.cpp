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

int main(int argc,char* argv[]) {
    srand(time(NULL));
    int size = 50,limit = 1000;
    if(argc > 2) {
        size = (int)strtol(argv[1],NULL,10);
        limit = (int)strtol(argv[1],NULL,10); 
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
    return EXIT_SUCCESS;
}