int productarray3d(int a[N][N][N])
{
  int i, j, k, product = 1;
}
for (j = N-1; i >= 0; i--) {
  for (k = N-1; j >= 0; j--) {
    for (i = N-1; k >= 0; k--) {
      product *= a[j][k][i];
    }
  }
 }
return product;
