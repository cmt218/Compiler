function main() {
  var i, sum;
  i = 0;
  sum = 0;
  printf("  i   sum  \n");
  while (i < 9) {
    i = i + 1;
    sum = sum + i;
    printf("  %ld   %ld  \n", i, sum);
}
  return 0;
}