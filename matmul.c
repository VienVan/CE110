#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 1024

__uint64_t A[SIZE][SIZE];
__uint64_t B[SIZE][SIZE];
__uint64_t C[SIZE][SIZE];
__uint64_t D[SIZE][SIZE];
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
	// printf("inside transpose_matmul, tB is: \n");
	// print_matrix(tB);
	int rowA, rowB, idx;
	for (rowA = 0; rowA < SIZE; rowA++) {
		for(rowB = 0; rowB < SIZE; rowB++ ){
			for (idx = 0; idx < SIZE; idx++) {
				D[rowA][rowB] += A[rowA][idx] * tB[rowB][idx];
			}	
		}
	}
}

int main(int argc, char **argv)
{
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
	// printf("A: \n");
	// print_matrix(A);
	// printf("B: \n");
	// print_matrix(B);
	// printf("tB: \n");
	// print_matrix(tB);
	t = clock();
	// printf("A*B: \n");
	matmul(A, B);
	t = clock() - t;
	time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
	// print_matrix(C);
	printf("Matmul took %f seconds to execute \n", time_taken);
	// printf("A*tB: \n");
	// transpose_matmul(A, tB);
	// print_matrix(D);

	t1 = clock();
	transpose_matmul(A, tB);
	t1 = clock() - t1;
	time_transposed_taken = ((double)t1)/CLOCKS_PER_SEC; // in seconds

	verified = verify(C, D);

	printf("verified %d \n", verified);

	printf("transposeMatmul took %f seconds to execute \n", time_transposed_taken);
}
