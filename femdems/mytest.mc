function main() {
  var i, sum;
  i = 1;
  sum = 0;
  printf("  i   sum \n");
  while (i < 10) {
    i = i + 1;
    sum = sum + i;
    printf("  %ld   %ld \n", i, sum);
  }
  return 0;
}