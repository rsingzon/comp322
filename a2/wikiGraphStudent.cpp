/**
 * Singzon, Ryan
 * 260397455
 *
 * COMP 322 - Introduction to C++
 * Winter 2014
 * Assignment 2
 */

#include <iostream>
#include <algorithm>
#include "graphSTL.h"


//This method takes as input a list<Edge> and prints the
//contents of the linked list
void print(list<Edge>& lst){

	for (Edge edge : lst){
		std::cout << "{" << edge.origin << ", " << edge.destination << ", " << edge.weight << "}" << std::endl;
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
	for (int vertex = 1; vertex <= numberVertices; vertex++){

		//Create a new list of edges for each vertex
		list<Edge> *edgeList = new list<Edge>;
		
		//Create a list of the vertices which have already been added
		vector<int> *existingVertices = new vector<int>;


		//If the origin or destination matches the current vertex, add edge to list
		for (Edge edge : lst){
			if (edge.origin == vertex || edge.destination == vertex){
				
				int vertexIndex = edge.origin == vertex ? edge.destination : edge.origin;
				bool vertexExists = false;

				//Check if the vertex has already been added to the list
				for (int x : *existingVertices){
					if (x == vertexIndex) vertexExists = true;
				}
				
				//Vertex does not exist, add it to list
				if(!vertexExists){
					existingVertices->push_back(vertexIndex);
					edgeList->push_back(edge);
				}
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

	WikiPage vertex;
	int index = 1;
	
	

	//Iterate through the vertices in the adjacency list
	for (list<Edge> edgeList : lst){
		vertex = page_ofID[index];
		std::cout << "Page \"" << vertex.title << "\" -> ";

		//Print each adjacent edge and its weight
		for (Edge edge : edgeList){

			//Check if the destination or the origin is the current vertex
			int adjacentVertexID = edge.origin == index ? edge.destination : edge.origin;
			WikiPage adjacentVertex = page_ofID[adjacentVertexID];

			std::cout << adjacentVertex.title << ":" << edge.weight << " ";
		}

		std::cout << std::endl;
		index++;
	}

	
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
	unsigned int stringLength = string_to_count.length();

	string nextLine;

	//Read lines from text file
	while (getline(in_file, nextLine)){
	
		string substring = nextLine;
		size_t position = substring.find(string_to_count);

		//Check if the line contains any instances of the desired string
		while (position != std::string::npos){
			
			//Remove the first part of the line which contains the found word
			substring = substring.substr(position);
			if (substring.length() > stringLength){
				substring = substring.substr(stringLength);
			}

			//Check if the word exists in the rest of the line
			position = substring.find(string_to_count);
			occurrences++;
		}
	}
	return occurrences;
}

/*
 * Takes as input two wiki pages and creates an edge connecting them
 */
Edge createEdge(WikiPage& page_1, WikiPage& page_2){
	int weight_1 = 0;
	int weight_2 = 0;
	string title_1, title_2;
	ifstream in_file_1, in_file_2;
	Edge newEdge;

	//Set the first page as origin, second as destination
	newEdge.origin = page_1.ID;
	newEdge.destination = page_2.ID;

	//Count the number of times the title of the second page appears
	//in the text file of the first page
	
	title_1 = page_1.title;
	title_2 = page_2.title;

	//Open an input stream to read file and count occurrences
	in_file_1.open(page_1.txt_location);
	in_file_2.open(page_2.txt_location);

	weight_1 = countOccurences(in_file_1, title_2);
	std::cout << "Occurrences of " << title_2 << " in " << title_1 << ": " << weight_1 << std::endl;
	weight_2 = countOccurences(in_file_2, title_1);
	std::cout << "Occurrences of " << title_1 << " in " << title_2 << ": " << weight_2 << std::endl;

	in_file_1.close();
	in_file_2.close();

	newEdge.weight = weight_1 + weight_2;

	return newEdge;
}

/*
 * Takes all the wiki pages and creates edges between each page
 */
list<Edge> createAllEdges(list<WikiPage>& pages){
	list<Edge> edges;

	//Iterate through the list of pages
	for (WikiPage firstPage : pages){
		int idFirst = firstPage.ID;

		//Find an adjacent page for each page
		for (WikiPage secondPage : pages){
			int idSecond = secondPage.ID;

			//If the first page matches the second page, skip
			if (idFirst == idSecond) continue;

			//Else, create an edge between the pages
			Edge newEdge = createEdge(firstPage, secondPage);

			//Only add the edge if its weight > 0
			if (newEdge.weight != 0){
				edges.push_back(newEdge);
			}
		}
	}

	return edges;
}

/*
 * Takes as input an adjacency list and saves it to a file
 * typedef vector<list<Edge> > adjacencyList;
 */
void saveGraphToFile(adjacencyList& graph_to_save, ofstream& out_file){

	//Iterate through the lists of edges for each vertex and
	for (list<Edge> edgeList : graph_to_save){

		//Iterate through all of the edges for each of the lists
		for (Edge edge : edgeList){

			//Avoid duplicating edges by only adding edges whose origins are less than their destinations
			if (edge.origin < edge.destination){
				out_file << edge.origin << " " << edge.destination << " " << edge.weight << std::endl;
			}
		}
	}
}

/*
 * Takes as input an input file stream and returns a list of edges 
 */
list<Edge> readGraphFromFile(ifstream& in_file){
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

	return graph;
}