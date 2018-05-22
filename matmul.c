#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 1024

 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

__uint64_t A[SIZE][SIZE];
__uint64_t B[SIZE][SIZE];
__uint64_t C[SIZE][SIZE];
__uint64_t D[SIZE][SIZE];
__uint64_t E[SIZE][SIZE];
__uint64_t tB[SIZE][SIZE];

void transpose(__uint64_t B[][SIZE], __uint64_t tB[][SIZE])
{
	int r, c;
	for (c = 0; c < SIZE; c++) {
		for (r = 0; r < SIZE; r++) {
			tB[r][c] = B[c][r];
		}
	}
}

// initialize A[] and B[] with random integers
void init(__uint64_t A[][SIZE], __uint64_t B[][SIZE])
{
	int r, c;
	for (c = 0; c < SIZE; c++) {
		for (r = 0; r < SIZE; r++) {
			A[r][c] = rand() % (10 + 1 - 0) + 0;
			B[r][c] = rand() % (10 + 1 - 0) + 0;
		}
	}
}

// function to print matrix
void print_matrix(__uint64_t A[][SIZE]) 
{
	int r, c;
	for( c = 0; c < SIZE; c++) {
		for (r = 0; r < SIZE; r++) {
			printf("%llu", A[r][c]);
			printf("|");
		}
	printf("\n");
	}
	printf("\n");
}

// verify 2 arrays C[] and D[] are the same
int verify(__uint64_t C[][SIZE], __uint64_t D[][SIZE])
{
	int r, c;

	for (c = 0; c < SIZE; c++) {
		for (r = 0; r < SIZE; r++) {
			if (C[r][c] != D [r][c]) {
				printf("error!\n");
				goto out;
			}

		}
	}
	return 0;

out:
	return -1;
}

// multiply matrix A[] with B[] using row and column wise
void matmul(__uint64_t A[][SIZE], __uint64_t B[][SIZE])
{
	int rowA, colB, idx;
	for (rowA = 0; rowA < SIZE; rowA++) {
		for (colB = 0; colB < SIZE; colB++) {
			for (idx = 0; idx < SIZE; idx++) {
				C[rowA][colB] += A[rowA][idx] * B[idx][colB];
			}
		}
	}
}

void transpose_matmul(__uint64_t A[][SIZE], __uint64_t tB[][SIZE])
{
	int rowA, rowB, idx;
	for (rowA = 0; rowA < SIZE; rowA++) {
		for(rowB = 0; rowB < SIZE; rowB++ ){
			for (idx = 0; idx < SIZE; idx++) {
				D[rowA][rowB] += A[rowA][idx] * tB[rowB][idx];
			}	
		}
	}
}

void blocking(__uint64_t A[][SIZE], __uint64_t tB[][SIZE], int block_size) 
{
	int i, k, j, jj, kk;
	for (k = 0; k < SIZE; k += block_size)
		for (j = 0; j < SIZE; j += block_size)
			for (i = 0; i < SIZE; ++i)
				for (jj = j; jj < min(j + block_size, SIZE); ++jj)
					for (kk = k; kk < min(k + block_size, SIZE); ++kk)
						E[i][jj] += A[i][kk] * tB[jj][kk];
}
int main(int argc, char *argv[])
{
	int block_size;
	// printf("%d ", argc);
	// printf("%s ", argv[0]); 
	// printf("%s ", argv[1]);

	if(argc == 2) {
		printf("argv[1] %s", argv[1]);
		block_size = (int) argv[1];
	}
	clock_t t;
	clock_t t1;
	double time_taken;
	double time_transposed_taken;
	int verified;

	init(A, B);
	transpose(B, tB);
	// print_matrix(B);
	// print_matrix(tB);
	memset(C, 0, sizeof(__uint64_t) * SIZE * SIZE);
	memset(D, 0, sizeof(__uint64_t) * SIZE * SIZE);
	memset(E, 0, sizeof(__uint64_t) * SIZE * SIZE);
	// printf("A: \n");
	// print_matrix(A);
	// printf("B: \n");
	// print_matrix(B);
	// printf("tB: \n");
	// print_matrix(tB);
	// t = clock();
	// printf("A*B: \n");
	// matmul(A, B);
	// t = clock() - t;
	// time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
	// print_matrix(C);
	// printf("Matmul took %f seconds to execute \n", time_taken);
	// printf("A*tB: \n");
	// transpose_matmul(A, tB);
	// print_matrix(D);

	t1 = clock();
	printf("%d \n", block_size);
	blocking(A, tB, 64);
	t1 = clock() - t1;
	time_transposed_taken = ((double)t1)/CLOCKS_PER_SEC; // in seconds

	// blocking(A, tB);

	// verified = verify(C, E);

	// printf("verified %d \n", verified);

	printf("blocking took %f seconds to execute \n", time_transposed_taken);
}
