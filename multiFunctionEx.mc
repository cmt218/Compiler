function getOneHundred() {
  return 100;
}

function multTen(x) {
  return x * 10;
}

function main() {
  var a;
  a = 0;

  while(a<1000) {
    if(a < 100) {

    }
    else {
      a = multTen(a);
    }
  }
}