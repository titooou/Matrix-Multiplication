                                                                   
                                                      PARALLEL DENSE MATRIX-MATRIX MULTIPLICATION 
							   BY : Meghazzi Houcine & HAMMOU BOUTRIG Bakhti Amine	
								     24/01/2021
## Assignment
The multiplication of two matrices is to be implemented as 

* a sequential program 
* an explicitly threaded program (using the pthread standard) 

## Matrix multiplication
The aim is to multiply two matrices together.To multiply two matrices, the number of columns of the first matrix has to match the number of lines of the second matrix. The calculation of the matrix solution has independent steps, it is possible to parallelize the calculation.

## Makefile
CC=gcc
TUNE= -c

all:  app-hw1

app-hw1:  main.o initMatrix.o displayMatrix.o ThreadCalcul.o calcul.o
  $(CC) main.o initMatrix.o displayMatrix.o ThreadCalcul.o calcul.o -o mmm -lm -lpthread

main.o: main.c
  $(CC) $(TUNE) main.c -o main.o 

initMatrix.o: initMatrix.c
  $(CC) $(TUNE) initMatrix.c -o initMatrix.o
  
displayMatrix.o:  displayMatrix.c
  $(CC) $(TUNE) displayMatrix.c -o displayMatrix.o  

ThreadCalcul.o: ThreadCalcul.c
  $(CC) $(TUNE) ThreadCalcul.c -o ThreadCalcul.o
  
calcul.o: calcul.c
  $(CC) $(TUNE) calcul.c -o calcul.o

clean:
  rm -fr app-hw1 *.o


## Implementations

### Sequential

The sequential program is used to compare and correctness to the other implementations. The following is an excerpt from the source code. Here is computed the result matrix.

    for (int i = 0; i < result_matrix->rows; i++) {
        for (int j = 0; j < result_matrix->cols; j++) {
            for (int k = 0; k < m_1->cols; k++) {
                result_matrix->mat_data[i][j] += m_1->mat_data[i][k] *      
                m_2->mat_data[k][j];
            }
        }
    }

### Thread (POSIX Threads)
The `sysconf(_SC_NPROCESSORS_ONLN)` from `#include <unistd.h>` returns the number of processors, what is set as the thread number, to use the full capacity. The following excerpt shows the thread memory allocation.

    int number_of_proc = sysconf(_SC_NPROCESSORS_ONLN);
    ...
    // Allocate thread handles
    pthread_t *threads;
    threads = (pthread_t *) malloc(number_of_proc * sizeof(pthread_t));

### Execution:
               -a makefile compile all the file and create an executable mmm
               -  ./mmm [calculation mode p/P for parallel & s/S for sequential mode] [n matrix size] [number of threads threads]

### Performance Test                                
./mmm p 5 4
Matrix A: 
37  53  8 9 97  
75  23  66  77  7 
36  58  45  29  31  
0 92  52  93  94  
17  61  45  20  44  

Matrix B: 
89  63  44  20  34  
48  29  97  71  31  
59  68  37  3 9 
44  17  60  19  12  
66  75  62  94  46  

Parallel Execution Row : 
13107 11840 13619 13816 7543  
31046 23428 26054 10904 10206 
23930 19656 25074 16876 10402 
35560 29670 44512 34582 17520 
10880 9540  12258 9322  5138  

Execution time : 0.000134 



Parallel execution block : 
13107 11840 27238 13816 7543  
15523 11714 26054 5452  5103  
23930 19656 50148 16876 10402 
17780 14835 44512 17291 8760  
10880 9540  24516 9322  5138  

Execution Time  : 0.000266 

./mmm s 5 4
Matrix A: 
35  23  90  54  73  
11  14  61  50  11  
64  29  58  62  12  
53  95  52  58  42  
87  37  94  3 16  

Matrix B: 
65  2 94  25  67  
85  52  75  57  33  
53  51  4 70  25  
10  65  12  20  46  
43  8 68  88  63  

Sequential execution : 
12679 9950  10987 15990 12437 
6111  7199  3676  7311  5717  
10835 8720  9983  9609  10303 
16662 11804 15867 15236 13300 
14500 7215  12453 12332 10546 
Execution time : 0.000061
          
calculate...
    
    * * * * * * * 100x100 Matrix
    with sequential    0m0.032s
    with thread       0m0.032s


    * * * * * * * 1000x1000 Matrix
    with sequential    0m11.791s
    with thread       0m4.153s
    
    * * * * * * * 5000x5000 Matrix
    with sequential    26m52.342s

The output times are the `real times` from the unix `time` command. 
You can see the advantages of parallel computation in the last matrix calculation. The parallel calculation is about 5 times faster (for large matrices).
