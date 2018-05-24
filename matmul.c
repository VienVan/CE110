/********************************************************
*Homework Group: Group_57														    *
*Student 1: Lawrence E Lawson	                          *
*Student 2: Vien Van																	  *
*Program: Matrix multiplication 												*
*********************************************************/
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

/********************************************************
*Function to transpose a matrix                         *
*Input: 2 matrices                                      *
*********************************************************/
void transpose(__uint64_t B[][SIZE], __uint64_t tB[][SIZE])
{
	int r, c;
	for (c = 0; c < SIZE; c++) {
		for (r = 0; r < SIZE; r++) {
			tB[r][c] = B[c][r];
		}
	}
}

/********************************************************
*Initializes 2 empty matrices with random numbers       *
*Input: 2 matrices                                      *
*********************************************************/
void init(__uint64_t A[][SIZE], __uint64_t B[][SIZE])
{
	int r, c;
	for (c = 0; c < SIZE; c++) {
		for (r = 0; r < SIZE; r++) {
			A[r][c] = rand();
			B[r][c] = rand();
		}
	}
}

/********************************************************
*Function to print matrix for debugging purpose         *
*                                                       *
*********************************************************/
// void print_matrix(__uint64_t A[][SIZE])
// {
// 	int r, c;
// 	for( c = 0; c < SIZE; c++) {
// 		for (r = 0; r < SIZE; r++) {
// 			printf("%llu", A[r][c]);
// 			printf("|");
// 		}
// 	printf("\n");
// 	}
// 	printf("\n");
// }

/********************************************************
*Function to verify if two matrices are the same        *
*Input: 2 matrices                                      *
*********************************************************/
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

/********************************************************
*Function to multiply matrix without optimization       *
*Input: 2 matrices                                      *
*********************************************************/
void matmul(__uint64_t A[][SIZE], __uint64_t B[][SIZE])
{
	int rowA, colB, idx;
	for (rowA = 0; rowA < SIZE; rowA++)
		for (colB = 0; colB < SIZE; colB++)
			for (idx = 0; idx < SIZE; idx++)
				C[rowA][colB] += A[rowA][idx] * B[idx][colB];
}

/********************************************************
*Function to multiply a matrix with a transposed matrix *
*Input: 2 matrices                                      *
*********************************************************/
void transposed_matmul(__uint64_t A[][SIZE], __uint64_t tB[][SIZE])
{
	int rowA, rowB, idx;
	for (rowA = 0; rowA < SIZE; rowA++)
		for(rowB = 0; rowB < SIZE; rowB++ )
			for (idx = 0; idx < SIZE; idx++)
				D[rowA][rowB] += A[rowA][idx] * tB[rowB][idx];
}


/********************************************************
*Function to use blocking to multiply 2 matrices        *
*This function uses transposed multiplication           *
*Input: 2 matrices and block size                       *
*********************************************************/
void transposed_block(__uint64_t A[][SIZE], __uint64_t tB[][SIZE], int block_size) {
	int si, sj, sk, i, j, k;
	__uint64_t sum;
	for (sj = 0; sj < SIZE; sj += block_size)
		for(si = 0; si < SIZE; si += block_size )
			for( sk = 0; sk < SIZE; sk += block_size)
				for( i = si; i < si+block_size; ++i)
					for( j = sj; j < sj + block_size; ++j)
						for(k = sk; k < sk + block_size; k++)
							E[i][j] += A[i][k] * tB[j][k];
}

/********************************************************
*Function to use blocking to multiply 2 matrices        *
*This function uses transposed multiplication           *
*Input: 2 matrices and block size                       *
*********************************************************/
void regular_block(__uint64_t A[][SIZE], __uint64_t tB[][SIZE], int block_size) {
	int si, sj, sk, i, j, k;
	__uint64_t sum;
	for (sj = 0; sj < SIZE; sj += block_size)
		for(si = 0; si < SIZE; si += block_size )
			for( sk = 0; sk < SIZE; sk += block_size)
				for( i = si; i < si+block_size; ++i)
					for( j = sj; j < sj + block_size; ++j)
						for(k = sk; k < sk + block_size; k++)
							E[i][j] += A[i][k] * tB[k][j];
}


int main(int argc, char *argv[])
{

		clock_t t;
		clock_t t1;
		clock_t t2;
		double time_taken;
		double time_transposed_taken;
		int verified;

		// initialize matrices with random variables
		init(A, B);
		// transpose matrix B
		transpose(B, tB);

		// initialize product matrices with 0s
		memset(C, 0, sizeof(__uint64_t) * SIZE * SIZE);
		memset(D, 0, sizeof(__uint64_t) * SIZE * SIZE);
		memset(E, 0, sizeof(__uint64_t) * SIZE * SIZE);

		// report time for regular matrix mult
		t = clock();
		matmul(A, B);
		t = clock() - t;
		time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
		printf("Matmul took %f seconds to execute \n", time_taken);

		// report time for transposed matrix mult
		t1 = clock();
		transposed_matmul(A, tB);
		t1 = clock() - t1;
		time_transposed_taken = ((double)t1)/CLOCKS_PER_SEC; // in seconds
		verified = verify(C, D);
		if(verified == 0 ){
				printf("Verified. Transposed took %f seconds to execute \n", time_transposed_taken);
		} else {
			printf("Not verified");
		}

		// report time for transposed matrix mult with blocking
		for(int i = 1; i <= SIZE; i*=2) {
			t2 = clock();
			// blocking(A, tB, i);
			transposed_block(A, tB, i);
			t2 = clock() - t2;

			time_transposed_taken = ((double)t2)/CLOCKS_PER_SEC; // in seconds
			verified = verify(C, E);
			if(verified == 0) {
					printf("Verified. Blocking with transposed took %f seconds to execute. Block size: %d \n", time_transposed_taken, i);
			} else {
				printf("Not verified. \n");
			}
			memset(E, 0, sizeof(__uint64_t) * SIZE * SIZE);
		}

		// report time for regular matrix mult with blocking
		for(int i = 1; i <= SIZE; i*=2) {
			t2 = clock();
			// blocking(A, tB, i);
			regular_block(A, B, i);
			t2 = clock() - t2;

			time_transposed_taken = ((double)t2)/CLOCKS_PER_SEC; // in seconds
			verified = verify(C, E);
			if(verified == 0) {
					printf("Verified. Blocking without transposed took %f seconds to execute. Block size: %d \n", time_transposed_taken, i);
			} else {
				printf("Not verified. \n");
			}
			memset(E, 0, sizeof(__uint64_t) * SIZE * SIZE);
		}


}
