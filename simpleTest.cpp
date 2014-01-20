#include <iostream>
#include "edge.h"
#include "graph.h"

using namespace std;

int main() {
  cout << "Adding all elements" << endl;
  Edge* e = append(NULL, 1,4,5);
  e = append(e,1,2,2);
  e = append(e,4,2,5);
  e = append(e,4,5,58);
  e = append(e,2,3,14);
  e = append(e,5,2,4);
  e = append(e,5,3,34);
  print(e);
  Edge** z = organizeList(e,5);
  cout << "Printing the adjacency list" << endl;
  printOrganized(z,5);
  cout << "Checking that the list was not compromized" << endl;
  print(e);
  cout << "Removing the first element" << endl;
  e = deleteFirst(e);  
  print(e);
  cout << "Removing all remaining elements" << endl;
  deleteList(e);
  cout << "DONE";
}
