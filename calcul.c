#include <stdio.h>
#include <stdlib.h>

#include "calcul.h"
#include "displayMatrix.h"

void MultiplicationMatrix (int **M1,int **M2, int size){
	int **M3 = (int **)malloc(size * sizeof(int *));
	for (int i = 0; i < size; i++)
		M3[i] = (int *)malloc(size * sizeof(int));
	for(int i = 0; i < size; ++i)
		for(int j = 0; j < size; ++j){
			M3[i][j] = 0;
			for(int k = 0; k < size; ++k)
				M3[i][j] += M1[i][k] * M2[k][j];
		}
	printMatrix(M3,size);
}
