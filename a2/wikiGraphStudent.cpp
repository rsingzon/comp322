/**
 * Singzon, Ryan
 * 260397455
 *
 * COMP 322 - Introduction to C++
 * Winter 2014
 * Assignment 2
 */

#include <iostream>
#include "graphSTL.h"


//This method takes as input a list<Edge> and prints the
//contents of the linked list
void print(list<Edge>& lst){
	
	list<int> list;

	list.push_front(2);
	list.push_front(4);


	for (int x : list){
		std::cout << x << std::endl;
	}

	for (Edge edge : lst){
		std::cout << "Origin: " << edge.origin << std::endl;
		std::cout << "Destination: " << edge.destination << std::endl;
		std::cout << "Weight: " << edge.weight << std::endl;
		std::cout << std::endl;
	}
}

/* 
 * This method goes through th list of edges and organizes it into 
 * separate lists for each vertex * 
 * 
 * Type definition for adjacencyList:
 * typedef vector<list<Edge> > adjacencyList;
 */
adjacencyList organizeList(list<Edge>& lst, int numberVertices){
	adjacencyList organizedList;

	//Iterate through the vertices and add adjacent vertices to each list
	for (int vertex = 0; vertex < numberVertices; vertex++){

		//Create a new list of edges for each vertex
		list<Edge> *edgeList = new list<Edge>;
		
		//If the origin or destination matches the current vertex, add edge to list
		for (Edge edge : lst){
			if (edge.origin == vertex || edge.destination == vertex){
				edgeList->push_back(edge);
			}
		}

		//Add the list to the adjacency list
		organizedList.push_back(*edgeList);
	}

	return organizedList;
}

/*
 * This method takes an adjacency list which has already been organized 
 * and prints the neighbours of each vertex in the graph
 */
void printOrganized(adjacencyList& lst, idToWikiMap page_ofID){

}

/*New methods*/
/* Type definition for idToWikiMap:
 * typedef map<int, WikiPage> idToWikiMap;
 */
idToWikiMap buildMap(list<WikiPage>& lst){
	idToWikiMap *map = new idToWikiMap;

	//Create an entry in the map for each wiki page
	for (WikiPage page : lst){
		map->insert(std::make_pair(page.ID, page));
	}

	return *map;
}

/*
 * Counts the number of times a particular string appears in 
 * an input file
 */
int countOccurences(ifstream& in_file, string string_to_count){
	int occurrences = 0;
	string word;

	//Read words from text file and check if it matches the input string
	while (in_file >> word){
		if (word.compare(string_to_count) == 0){
			occurrences++;
		}
	}

	return occurrences;
}

Edge createEdge(WikiPage& page_1, WikiPage& page_2){
	Edge newEdge;

	return newEdge;
}

list<Edge> createAllEdges(list<WikiPage>& pages){
	list<Edge> edges;

	return edges;
}

void saveGraphToFile(adjacencyList& graph_to_save, ofstream& out_file){

}

list<Edge> readGraphFromFile(ifstream& in_file){
	list<Edge> graph;

	return graph;
}