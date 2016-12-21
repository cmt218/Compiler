function main() {
  var i, sum;
  i = 1;
  sum = 0;
  printf("  i   sum\n");
  while (i < 10) {
    i = add(i, 1);
    sum = add(sum, i);
    printf("  %ld   %ld\n", i, sum);
  }
  return 0;
}

function add(a, b) {
  var c;
  c = a + b;
  return c;
}

