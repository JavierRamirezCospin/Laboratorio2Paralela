# Laboratorio2Paralela
Repositorio para el codigo del Laboratorio 2, de Programacion Paralela y Distribuida 2023.

## Sequential Code

To compile sequential code (main.cpp) use command at root folder:
```
gcc main.cpp -fopenmp -o main -lstdc++ -lm
```

To run sequential code (main.cpp) use command at root folder:
```
./main <num_elements> <maxnum>
```

where:
 - *num_elements:* quantity of numbers to be generated
 - *maxnum:* max range to generate numbers (0 - maxnum)

## Parallel Code

To compile sequential code (main_par.cpp) use command at root folder:
```
gcc main_par.cpp -fopenmp -o main_par -lstdc++ -lm
```

To run sequential code (main_par) use command at root folder:
```
./main_par <num_elements> <maxnum> <num_threads>
```

where:
 - *num_elements:* quantity of numbers to be generated
 - *maxnum:* max range to generate numbers (0 - maxnum)
 - *num_threads:* quantity of threads to execute code
