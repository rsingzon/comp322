#include <iostream>
#include "graphSTL.h"

using namespace std;

WikiPage* wikiPageInit(int id, string name, string html, string txt);


int main() {
  cout  << "Creating list of articles" << endl;
  list<WikiPage> all_pages;

  all_pages.push_back(*wikiPageInit(1, "New York",
				   "wiki/wp/n/New_York_City.htm",
				   "wiki/text/New_York_City.txt"));
  all_pages.push_back(*wikiPageInit(2, "Chicago",
				   "wiki/wp/c/Chicago.htm",
				   "wiki/text/Chicago.txt"));
  all_pages.push_back(*wikiPageInit(3, "Montreal",
				   "wiki/wp/m/Montreal.htm",
				   "wiki/text/Montreal.txt"));
  all_pages.push_back(*wikiPageInit(4, "Toronto",
				   "wiki/wp/t/Toronto.htm",
				   "wiki/text/Toronto.txt"));
  all_pages.push_back(*wikiPageInit(5, "Miami",
				   "wiki/wp/m/Miami%2C_Florida.htm",
				   "wiki/text/Miami%2C_Florida.txt"));
  
  list<Edge> edges = createAllEdges(all_pages);
  cout << "Here is the list of edges created:" << endl;
  print(edges);


  adjacencyList graphAL = organizeList(edges, 5);
  idToWikiMap page_ofID = buildMap(all_pages);
  cout << "Here is the graph associated with the list of edges:" << endl;
  printOrganized(graphAL, page_ofID);


  cout << "Testing the file input and output:" << endl;
  ofstream ffo; ffo.open("out_graph.txt");
  if(ffo.fail()) { cout << "File could not be opened" << endl; return 1;}
  saveGraphToFile(graphAL, ffo);
  ffo.close();
  ifstream ffi; ffi.open("out_graph.txt");
  if(ffi.fail()) { cout << "File could not be opened" << endl; return 1;}
  edges = readGraphFromFile(ffi);
  ffi.close();
  print(edges);

  return 0;
}


WikiPage* wikiPageInit(int id, string name, string html, string txt) {
  WikiPage* tmp_page = new WikiPage;
  tmp_page->ID = id;
  tmp_page->title = name;
  tmp_page->html_location = html;
  tmp_page->txt_location = txt;
  return tmp_page;
}
