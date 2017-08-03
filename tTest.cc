#include <iostream>
#include "node.h"
#include "link.h"
#include "tri.h"

using namespace std;

int main(int argc, char** argv){

  /* Tri was my second attempt at making the trie class which ended up
     working so thats why mine is not named trie */

  //Create trie object
  Tri t;

  //Put a word in the trie and get the value
  t.put("racecar", "five");
  cout << t.get("racecar") << endl;

  t.put("x", "x$1");
  cout << t.get("x") << endl;


  /*
  //Put a contained word in the trie and get the value
  t.put("race",15);
  cout << t.get("race") << endl;

  //Put a branched word in the trie and get the value
  t.put("racetrack",9);
  cout << t.get("racetrack") << endl;

  //Replace the value of a key already in the trie
  t.put("race",7);
  cout << t.get("race") << endl;

  //Try getting a value for a key not in the trie
  cout << t.get("cats") << endl;
  */
  return 0;

}


