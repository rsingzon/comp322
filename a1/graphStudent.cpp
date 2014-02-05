/**
 * Singzon, Ryan
 * 260397455
 * 
 * COMP 322 - Introduction to C++
 * Winter 2014
 */

#include <iostream>
#include "graph.h"

/* Compares two edges and checks if they have the same vertices
 */
int compareTo(const Edge& edge1, const Edge& edge2){

	int start1 = edge1.startVertex;
	int start2 = edge2.startVertex;
	int end1 = edge1.endVertex;
	int end2 = edge2.endVertex;

	// Case 1: Both edges connect the same vertices
	// Returns 0
	if( (start1 == start2 && end1 == end2) || 
			(start1 == end2 && end1 == start2)){
		return 0;
	}

	// Case 2: Edges have one vertex in common
	// Returns -1 if edge1 < edge2
	// Returns 1 if edge1 > edge2
	if( start1 == start2 ){
		return end1 > end2 ? 1 : -1;
	}
	else if( start1 == end2){
		return end1 > start2 ? 1 : -1;
	}
	else if( end1 == start2){
		return start1 > end2 ? 1 : -1;
	}
	else if( end1 == end2){
		return start1 > start2 ? 1 : -1;
	}

	// Case 3: Edges have no vertices in common
	// Compares the smaller vertices of each pair
	else {
		int vertex1, vertex2;
		vertex1 = start1 < end1 ? start1 : end1;
		vertex2 = start2 < end2 ? start2 : end2;
		return vertex1 > vertex2 ? 1 : -1;
	}
}


/**
 * Adds a new Edge to the end of the adjacency list
 * Returns a pointer to the first Edge in the list
 *
 */
Edge* append(Edge* root, int origin, int destination, int weight){
	
	//Create a new node
	Edge* newEdge = new Edge();
	newEdge->startVertex = origin;
	newEdge->endVertex = destination;
	newEdge->weight = weight;
	newEdge->nextEdge = NULL;

	//Place the new edge into the adjacency list
	//Check if the adjacency list is empty
	if(root != NULL){
		Edge* currentEdge = root;

		//Traverse the list until the end is reached
		while(currentEdge->nextEdge != NULL){
			currentEdge = currentEdge->nextEdge;	
		}
		
		//Set the tail of the adjacency list to the new Edge
		currentEdge->nextEdge = newEdge;
	}	

	//If adjacency list is empty, make the new edge the first element
	else{	
		root = newEdge;
	}	
	
	return root;
}

/**
 * Prints the contents of the linked list starting at the input
 */
void print(Edge* root){

	//Invalid input 
	if(root == NULL) return;

	Edge* currentEdge = root;

	//Traverse through the list of edges and print the values of each
	while(currentEdge != NULL){
		std::cout << "{" << currentEdge->startVertex << "," << currentEdge->endVertex 
						 << "," << currentEdge->weight << "}" << std::endl;
		currentEdge = currentEdge->nextEdge;
	}
}

/**
 * Removes the first element in a list
 */
Edge* deleteFirst(Edge* root){

	//Invalid input
	if(root == NULL) return NULL;

	//If the next node in the list is null, return null
	if(root->nextEdge == NULL){
		delete root;
		return NULL;
	}
	//Else return what used to be the second element in the list
	else{ 
		Edge* newRoot = root->nextEdge;
		delete root;
		return newRoot;
	}
}

/**
 * Removes all the elements in a list
 */
void deleteList(Edge* root){

	//Invalid input
	if(root == NULL) return;

	//Create variables to hold pointers to nodes in the list
	Edge* currentEdge = root;
	Edge* nextEdge = root->nextEdge;

	//Delete root if it is the only node
	if(nextEdge == NULL){
		root = NULL;
		delete root;	
	}

	//Traverse the list of nodes and delete all of them
	while(currentEdge != NULL){
		
		nextEdge = currentEdge->nextEdge;
		currentEdge->nextEdge = NULL;
		currentEdge = NULL;
		delete currentEdge;
		currentEdge = nextEdge;
	}	
}

/**
 * Goes through the list of edges and creates separate lists for each vertex 
 * Returns the array of lists of edges
 */
Edge** organizeList(Edge* root, int numberVertices){
	
	//Initialize an array for each vertex
	Edge **adjList = new Edge*[numberVertices];

	
	//For each vertex, go through the linked list of edges to find
	//which edges start or stop at that vertex
	Edge *currentEdge = root;
	for(int vertex = 1; vertex <= numberVertices; vertex++){

		//Traverse the list of edges and check if the current
		//vertex is the start or end of the edge
		int first = 1;
		Edge *newEdge;
		while(currentEdge != NULL){			

			int startVertex = currentEdge->startVertex;
			int endVertex = currentEdge->endVertex;
			int weight = currentEdge->weight;

			//If the start or end vertex of the edge matches the current vertex
			//append it to the list 
			if(vertex == startVertex || vertex == endVertex){

				//Swap end and start such that matching vertex is start
				if(vertex == endVertex){
					int swap = startVertex;
					startVertex = endVertex;
					endVertex = swap;
				}

				//Create a new list if one does not exist
				if(first == 1){
					newEdge = append(NULL, startVertex, endVertex, weight);	
					first = 0;
				}
				else{
					newEdge = append(newEdge, startVertex, endVertex, weight);		
				}	
			} 
			
			currentEdge = currentEdge->nextEdge;
		}

		//Set the root of the list
		adjList[vertex] = newEdge;
		currentEdge = root;
	}

	return adjList;
}

/**
 * Takes a graph that has already been organized by organizeList()
 * Prints the neighbours of each vertex in the graph
 */
void printOrganized(Edge** adjList, int numberVertices){
	//Iterate through the vertices
	for(int vertex = 1; vertex <= numberVertices; vertex++){

		std::cout << std::endl;
		std::cout << "Vertex " << vertex << " -> ";

		//Iterate through the edges connected to each vertex and print
		int first = 1;
		Edge *currentEdge = adjList[vertex];
		while(currentEdge != NULL){

			//Assign values to variables
			int neighbour, vertex1, vertex2, weight;
			vertex1 = currentEdge->startVertex;
			vertex2 = currentEdge->endVertex;
			weight = currentEdge->weight;
				
			//Set the neighbour to the vertex that is not currently being iterated through
			neighbour = vertex1 == vertex ? vertex2 : vertex1;
			if(first == 1){
				std::cout << neighbour << ":" << weight;	
				first = 0;
			}
			else{
				std::cout << ", " << neighbour << ":" << weight;		
			}

			currentEdge = currentEdge->nextEdge;
		}	
	}
	std::cout<<std::endl;
}