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

// void blocking(__uint64_t A[][SIZE], __uint64_t tB[][SIZE], int block_size)
// {
// 	int idx, block_a, block_b, row_a, row_b;
//
// 	for (block_a = 0; block_a < SIZE; block_a += block_size) {
// 		for (block_b = 0; block_b < SIZE; block_b += block_size) {
// 			for (idx = 0; idx < SIZE; idx++) {
// 				for (row_a = block_a; row_a < min(block_a + block_size, SIZE); row_a++) {
// 					for (row_b = block_b; row_b < min(block_b + block_size, SIZE); row_b++) {
// 						E[idx][row_a] += A[idx][row_b] * tB[row_a][row_b];
// 					}
// 				}
// 			}
// 		}
// 	}
// }

void dgemm_block(__uint64_t A[][SIZE], __uint64_t tB[][SIZE], int block_size) {
	int si, sj, sk, i, j, k;
	__uint64_t sum;
	for (sj = 0; sj < SIZE; sj += block_size)
		for(si = 0; si < SIZE; si += block_size )
			for( sk = 0; sk < SIZE; sk += block_size)
				for( i = si; i < si+block_size; ++i)
					for( j = sj; j < sj + block_size; ++j)
						// sum = E[i][j];
						for(k = sk; k < sk + block_size; k++)
							E[i][j] += A[i][k] * tB[j][k];
						// E[i][j] = sum;

}

int main(int argc, char *argv[])
{

	clock_t t;
	clock_t t1;
	clock_t t2;
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

	t = clock();
	matmul(A, B);
	t = clock() - t;
	time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

	printf("Matmul took %f seconds to execute \n", time_taken);

	t1 = clock();
	transpose_matmul(A, tB);
	t1 = clock() - t1;
	verified = verify(C, D);
	printf("verified %d \n", verified);
	time_transposed_taken = ((double)t1)/CLOCKS_PER_SEC; // in seconds
	printf("Transposed took %f seconds to execute \n", time_transposed_taken);


	for(int i = 1; i <= SIZE; i*=2) {
		// print_matrix(C);
		// printf("i: %d\n", i);
		t2 = clock();
		// blocking(A, tB, i);
		dgemm_block(A, tB, i);
		t2 = clock() - t2;

		time_transposed_taken = ((double)t2)/CLOCKS_PER_SEC; // in seconds
		verified = verify(C, E);
		printf("verified %d \n", verified);
		printf("blocking took %f seconds to execute. Block size: %d \n", time_transposed_taken, i);
		memset(E, 0, sizeof(__uint64_t) * SIZE * SIZE);
	}




}
