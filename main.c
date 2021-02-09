#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "initMatrix.h"
#include "displayMatrix.h"
#include "calcul.h"
#include "ThreadCalcul.h"



/*Creation of blocksize structur*/
typedef struct blocksize{
						int n;
						int NbrThreads;
						int taille;
						int reste;
						int istart;
						int iend;
						int **A;
						int **B;
						int **C;
						}blocksize;

typedef struct blocksize_Matrix_Form{
									int n;
									int NbrThreads;
									int taille;
									int reste;
									int istart;
									int iend;
									int jstart;
									int jend;
									int **A;
									int **B;
									int **C;
								}blocksize_Matrix_Form;

void *Mult_vectorielle(void * args);
void *Mult_Matricielle(void * args);

int main(int argc, char **argv){
	
	//declaration of variables
	int       i,j;
	clock_t   debut,fin;
	double    temps;
	
	char *    Mode      =  argv[1]; 	// chosen mode  		
	int 	  n         =  atoi (argv[2]);  // Matrix size
	int       NBTHREAD =  atoi (argv[3]); //   number of thread for parallel mode  
	float sqrto;int sqrta;int NBRTHREAD;
	
	
	sqrto=sqrt((double)NBTHREAD);
    sqrta=sqrto;
    NBRTHREAD=sqrta*sqrta;
	/*Allocation et initialisation des matrices*/
	int **A = (int **)malloc(n * sizeof(int *));
	for (i = 0; i < n; i++)
		A[i] = (int *)malloc(n * sizeof(int));
	printf("Matrix A: \n");
	generateRandomMatrix (A, n, 0, 99); // fill Matrix  A Randomly
	printMatrix(A,n);		  // Display Matrix A 
	printf("\n");
	
	int **B = (int **)malloc(n * sizeof(int *));
	for (i = 0; i < n; i++)
		B[i] = (int *)malloc(n * sizeof(int));
	printf("Matrix B: \n");
	generateRandomMatrix(B, n, 1, 99); // Fill Matrix B Randomly
	printMatrix(B,n);		  // display Matrix B
	printf("\n");
	
	
	int **C = (int **)malloc(n * sizeof(int *));
	for (i = 0; i < n; i++)
		C[i] = (int *)malloc(n * sizeof(int));
		
	int **D = (int **)malloc(n * sizeof(int *));
	for (i = 0; i < n; i++)
		D[i] = (int *)malloc(n * sizeof(int));
	
	/*Argement pour passage des paramtres*/
	blocksize Arg;
	Arg.NbrThreads = NBRTHREAD ;
	Arg.n = n;
	Arg.taille = n/NBRTHREAD;
	Arg.reste = n%NBRTHREAD;
	Arg.istart = 0;
	Arg.iend = n-1;
	Arg.A = A;
	Arg.B = B;
	Arg.C = C;
	
	blocksize_Matrix_Form block;
	block.NbrThreads = NBRTHREAD ;
	block.n = n;
	block.taille = n/(int)sqrt((NBRTHREAD));
	block.reste = n%(int)sqrt((NBRTHREAD));
	block.A = A;
	block.B = B;
	block.C = D; 
	
	if((!strcmp(Mode,"S"))||(!strcmp(Mode,"s"))){
		printf("Sequential execution : \n");
		debut = clock();
		MultiplicationMatrix(A,B,n);
		fin = clock();
		temps = ((double)fin - debut) / CLOCKS_PER_SEC; //Calcul le temps d'execution pour le mode sequential
		printf("Execution time : %f\n",temps);
		return 0;
	} 
	else if ((!strcmp(Mode,"P"))||(!strcmp(Mode,"p"))){ 
		printf("Parallel Execution Row : \n");
		debut = clock();
		
		/*Creation des threads*/
		pthread_t *threads = (pthread_t*) malloc(sizeof(pthread_t) * NBRTHREAD);
		for(i=0; i<NBRTHREAD; i++){ 
			Arg.istart = i * Arg.taille;
			Arg.iend = Arg.istart + Arg.taille-1 + Arg.reste;
			debut = clock();
			if(pthread_create(&threads[i], NULL,Mult_vectorielle,&Arg)){ 
				fprintf(stderr, "inable to create thread \n");
				return 1;
			} 
			if(pthread_join(threads[i], NULL)){ 
				fprintf(stderr, "inable to join thread\n");
			return 2;
			}
			
		}
		
		printMatrix(Arg.C,Arg.n);
		fin = clock();
		temps = ((double)fin - debut) / CLOCKS_PER_SEC; // Calculatio of parralel execution time 
		printf("\nExecution time : %f \n",temps);
		printf("\n\n\n");
				
		printf("Parallel execution block : \n");
		debut = clock();
		pthread_t *threads2 = (pthread_t*) malloc(sizeof(pthread_t) * NBRTHREAD);
		for(i=0; i<NBRTHREAD; i++){ 
			block.istart = (i/(int)sqrt(NBRTHREAD)) * block.taille;
			block.iend   = block.istart  + block.taille -1 + block.reste;
			if(i < sqrt(NBRTHREAD)){
				block.jstart = i * block.taille;
				block.jend   = block.jstart + block.taille -1 + block.reste;
			}else{
				block.jstart = (i % (int)sqrt(NBRTHREAD)) * block.taille;
				block.jend   = block.jstart + block.taille -1 + block.reste;
			}
			//debut = clock();
			if(pthread_create(&threads2[i], NULL,Mult_Matricielle,&block)){ 
				fprintf(stderr, "inable to create thread\n");
				return 3;
			} 
			if(pthread_join(threads2[i], NULL)){ 
				fprintf(stderr, "inable to join thread \n");
			return 4;
			}
		} 		
	}
			printMatrix(block.C,block.n);
			fin = clock();
			temps = ((double)fin - debut) / CLOCKS_PER_SEC; //calcul execution time for block in parallel mode
			printf("\nExecution Time  : %f \n",temps);
			printf("\n\n\n");
			
	}
