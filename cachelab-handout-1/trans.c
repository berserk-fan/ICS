#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
  int i,j,r,c;
  int tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7,tmp8;

  if(M == 32 && N == 32){  
    for(i = 0; i < 4; ++i) {
      for(r = 0; r < 32; r++){
	c = i*8;
	tmp1 = A[r][c];
	tmp2 = A[r][c+1];
	tmp3 = A[r][c+2];
	tmp4 = A[r][c+3];
	tmp5 = A[r][c+4];
	tmp6 = A[r][c+5];
	tmp7 = A[r][c+6];
	tmp8 = A[r][c+7];

	B[c][r] = tmp1;
	B[c+1][r] = tmp2;
	B[c+2][r] = tmp3;
	B[c+3][r] = tmp4;
	B[c+4][r] = tmp5;
	B[c+5][r] = tmp6;
	B[c+6][r] = tmp7;
	B[c+7][r] = tmp8;
      }
    } 
  }
  if(M == 64 && N == 64){
    for(i = 0; i < 64; i+=8) {
      for(j = 0; j < 64; j+=16) {
        for(r = j; r < j + 8; ++r) { 
	    for(c = i; c < i + 4; ++c){
	      B[c][r] = A[r][c];
	    }
	}
	for(r = j; r < j + 8; ++r){
	  for(c = i + 4; c < i + 8; ++c){
	    B[c][r+8] = A[r][c];
	  }
	}
	
	for(r = j + 8; r < j + 16; ++r) { 
	    for(c = i; c < i + 4; ++c){
	      B[c][r] = A[r][c];
	    }
	}
	
	for(r = j + 8; r < j + 16; ++r){
	  for(c = i + 4; c < i + 8; ++c){
	    B[c][r-8] = A[r][c];
	  }
	}
	
	for(tmp8 = i+4; tmp8 < i+8; ++tmp8){
	  for(c = j; c < j + 8; ++c){
	    tmp1 = B[tmp8][c+8];
	    B[tmp8][c+8] = B[tmp8][c];
	    B[tmp8][c] = tmp1;
	  }
	}
      }
    }

  }
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}


/* 
 * initMatrix - Initialize the given matrix 
 */
void initMatrix(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;
    srand(time(NULL));
    for (i = 0; i < N; i++){
        for (j = 0; j < M; j++){
            // A[i][j] = i+j;  /* The matrix created this way is symmetric */
            A[i][j]=rand();
            B[j][i]=rand();
        }
    }
}

int main(){
  int A[64][64];
  int B[64][64];
  
  initMatrix(64,64,A,B);
  transpose_submit(64,64,A,B);
  printf("%d", is_transpose(64,64,A,B));
}
