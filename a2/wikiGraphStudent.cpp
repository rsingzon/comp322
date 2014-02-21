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
	string word, twoWord, threeWord;
	string oldWord = "";
	string olderWord = "";
	

	//Read words from text file and check if it matches the input string
	while (in_file >> word){
		
		twoWord = oldWord + " " + word;
		threeWord = olderWord + " " + oldWord + " " + word;

		//Check one, two and three length strings
		if ((word.find(string_to_count) != string::npos) ||
			(twoWord.find(string_to_count) != string::npos) ||
			(threeWord.find(string_to_count) != string::npos)){

			oldWord = "";
			olderWord = "";

			occurrences++;
		}
		else{
			oldWord = word;
			olderWord = oldWord;
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

	//If the second page never appears in the first page, do not add an edge
/*	if (weight_1 + weight_2 == 0){
		return;
	}
*/
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
			edges.push_back(createEdge(firstPage, secondPage));
		}
	}

	return edges;
}

void saveGraphToFile(adjacencyList& graph_to_save, ofstream& out_file){

}

list<Edge> readGraphFromFile(ifstream& in_file){
	list<Edge> graph;

	return graph;
}