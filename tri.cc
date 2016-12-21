/*
  CSE109
  Cole Tomlinson
  cmt218
  Trie data structure program
  Program #3
*/

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include "tri.h"
#include "node.h"
#include "link.h"

//Constructor
Tri::Tri() {
  sNode = new Node;
}

//Destructor
Tri::~Tri() {
}

//Put method
void Tri::put(string key, string val){

  //Variables for string placement
  Node* curNode = sNode;
  int keyLength = key.length();
  bool match = false;

  //Iterates through the length of the key
  for(int x = 0;x < keyLength;x++){

    //Iterates through the links going from the current node
    for(int y = 0;y < (*curNode).numLinks;y++){
      //Handles case where string characters already have links
      if((*(*curNode).links[y]).letter == key[x]){
	curNode = (*(*curNode).links[y]).next;
	match = true;

	//Places the key's value if it is at the last character
	if(x == keyLength - 1){
	  (*curNode).nodeVal = val;
	}
	break;
      }      
    }

    //Handles case where new nodes and links have to be created
    if((*curNode).numLinks < 10 && match == false){

      Link* add = new Link;
      
      (*add).setLet(key[x]);
      (*curNode).links[(*curNode).numLinks] = add;
      (*curNode).numLinks++;
      curNode = new Node;
      (*add).next = curNode;

      //Places the key's value if it is at the last character
      if(x == keyLength - 1){
	(*curNode).nodeVal = val;
      }
    }
    match = false;
  }
}

//Method for getting the value associated with a key
string Tri::get(string key){
  //Variables for trie navigation
  Node* curNode = sNode;
  int keyLength = key.length();
  string keyVal = "";
  bool found = false;
  
  //Iterates through the length of the key
  for(int a = 0;a < keyLength+1;a++){
    //Iterates through the links in a node
    for(int b = 0;b < (*curNode).numLinks;b++){
      if((*(*curNode).links[b]).letter == key[a]){
	curNode = (*(*curNode).links[b]).next;
	found = true;
	break;
      }
    }
    if(found == false){
      keyVal = "Not Found";
      return keyVal;
    }
  }
  

  keyVal = (*curNode).nodeVal;
 
  //Return the value associated with the key
  return keyVal;
}

bool Tri::contains(string key){


  Node* curNode = sNode;
  int keyLength = key.length();
  bool found = false;

   
  for(int x = 0; x <= keyLength-1; x++){
    found = false;

    for(int y = 0; y < curNode->numLinks; y++){

      if(curNode->links[y]->letter == key[x]){
	curNode = curNode->links[y]->next;
	found = true;
	break;
      }
    }
    if(found == false){
      return false;
    }
  }
  return true;


}
