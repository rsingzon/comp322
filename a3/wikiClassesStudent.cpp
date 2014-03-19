/**
* COMP 322 - Introduction to C++
* Assignment 3
* Winter 2014
*
* Singzon, Ryan
* 260397455
*
*/

#include "wikiClasses.h"


/**
 * Implementation of the Graph class 
 */

//Constructor for graph class using vertices and edges
Graph::Graph(list<Edge> lst, int num_vertices){

}

//Constructor for graph class using an input file
Graph::Graph(ifstream& in_file){

}

//Destructor for graph object
Graph::~Graph(){

}

void Graph::save_to_output_file(ofstream& o_edges) const{

}

void Graph::push_node(list<Edge>& lst){

}

/**
 * Implementation of the WikiGraph class
 */

// Destructor for the WikiGraph class
WikiGraph::~WikiGraph(){

}


void WikiGraph::push_page(WikiPage& wp){

}

// Comment
void WikiGraph::save_to_output_files(ofstream& out_edges, ofstream& out_title_to_node)const{

}

/*
 * Other methods
 */

ostream& operator<< (ostream& o, Graph const& g){

}

ostream& operator<< (ostream& o, WikiGraph const& wikiGraph){

}

WikiGraph::WikiPage& make_wiki_page(string path_to_html, string path_to_txt){

}
