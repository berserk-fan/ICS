#define N 16
typedef int fix_array[N][N];

void fix_set_diagonal(fix_array A, int val)
{
  int *first;
  int *last;
  int int_size;

  first = A;
  last = &A[N][N];
  int_size = sizeof(int);
  
  while(first <= last)
  {
    *first = val;
    first += (N+1);
  }
}

int main(){
}
