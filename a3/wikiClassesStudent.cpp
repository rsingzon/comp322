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

	//Iterate through vertices and add adjacent vertices to each list
	for(int vertex = 1; vertex <= num_vertices; vertex++){

		//Create a list of edges for each vertex
		list<Edge> *edgeList = new list<Edge>;

		//Create a list of the vertices which have already been added
		vector<int> *existingVertices = new vector<int>;

		//If the origin or destination matches the current vertex, add the edge to list
		for(Edge edge: lst){
			if(edge.origin == vertex || edge.destination == vertex){
				int vertexIndex = edge.origin == vertex ? edge.destination : edge.origin;
				bool vertexExists = false;

				//Check if the vertex has already been added to the list
				for (int x : *existingVertices){
					if(x == vertexIndex) vertexExists = true;
				}

				//If the vertex does not exist, add it to list
				if(!vertexExists){
					existingVertices->push_back(vertexIndex);
					edgeList->push_back(edge);
				}
			}
		}

		//Add the list to the adjacency list in the graph
		adj_list.push_back(*edgeList);
	}
}

//Constructor for graph class using an input file
/***
** FIX THIS
**/
Graph::Graph(ifstream& in_file){
	
	list<Edge> graph;

	int index = 0;
	int value;
	Edge *newEdge;

	//Iterate through all values in the file
	while (in_file >> value){

		//Assign first value in line as origin
		if (index == 0){
			newEdge = new Edge;
			newEdge->origin = value;
		}

		//Assign second value as destination
		else if (index == 1){
			newEdge->destination = value;
		}

		//Assign third value to weight and push edge into list
		else{
			newEdge->weight = value;
			graph.push_back(*newEdge);
			index = -1;
		}

		index++;
	}

	//The adjacency list stores lists of edges that include the
	//current index of the list
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
