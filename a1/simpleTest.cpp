#include <iostream>
#include "edge.h"
#include "graph.h"

using namespace std;

int main() {
  Edge* e = append(NULL, 1,4,5);
  e = append(e,1,2,2);
  e = append(e,4,2,5);
  e = append(e,4,5,58);
  e = append(e,2,3,14);
  e = append(e,5,2,4);
  e = append(e,5,3,34);
  print(e);
  
  Edge** z = organizeList(e,5);

  printOrganized(z,5);
  cout << "Checking that the list was not compromized" << endl;
  print(e);
  cout << "Removing the first element" << endl;
  e = deleteFirst(e);  
  print(e);
  cout << "Removing all remaining elements" << endl;
  deleteList(e);
  print(e);
  cout << "Removed all remaining elements" << endl;
  cout << "DONE";
/*
  const& Edge p = append(NULL, 2,4, 10);
  const& Edge b = append(NULL, 4,2, 10);
  Edge *c = append(NULL, 1,2, 10);
  Edge *d = append(NULL, 2,2, 10);
  Edge *g = append(NULL, 2,3, 10);
  Edge *f = append(NULL, 2,4, 10);

  int u = compareTo(p, b);
  int y = compareTo(c, d);
  int x = compareTo(g, f);
  int w = compareTo(b, d);
  std::cout<< u <<std::endl;
  std::cout<< y<<std::endl;
  std::cout<< x<<std::endl;
  std::cout<< w<<std::endl;

(2,4) == (4,2) (Because graph is undirected)

(1,2) < (2,2)

(2,3) < (2,4)

(4,2) > (2,2)

(2,1) > (1,1)

(2,1) < (3,5)

(3,5) > (2,1)
  */
}
