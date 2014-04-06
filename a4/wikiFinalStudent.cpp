/*
* COMP 322 - Introduction to C++
* Assignment 4
* Winter 2014
*
* Singzon, Ryan
* 260397455
*/

#include "wikiFinal.h"


/* Samples an edge from the given list by using the 
 * weights of each edge as the histogram for the 
 * distribution to be sampled */
Graph::Edge sampleEdge(list<Graph::Edge> lst){
	Graph::Edge sampleEdge;
	int totalWeight;
	int index = 0;
	vector<int> cumul;
	

	//Find the total weight from the sum of weights of each edge
	for (list<Graph::Edge>::iterator it = lst.begin(); it != lst.end(); ++it){
		
		//Check if any edges have weight <= 0
		try{
			if(it->weight <= 0){
				throw index;
			} 
			else{
				//Update the current weight and add the current weight to the vector
				totalWeight += it->weight;
				cumul.push_back(totalWeight);		
				index++;
			}
		}
		catch(int index){
			cerr << "Index %d of edge list has weight less than or equal to zero." << endl;
		}
	}

	//Obtain a random number between 0 and totalWeight-1
	int randNum = (rand() % totalWeight-1);
	
	//Find the index of k such that cumul[k-1] < randNum <= cumul[k]
	for(int k = 1; k < cumul.size(); k++){
		if(randNum < cumul.at(k) && randNum > cumul.at(k-1)){
			
			list<Graph::Edge>::iterator it = lst.begin();
    		advance(it, k);
			sampleEdge = *it;
		}
	}

	return sampleEdge;
}

map<int, int> Graph::random_walks(int start_node) const{
	map<int, int> walk;

	

	return walk;
}

list<int> Graph::breadth_first_search(int start_node, int number_nodes) const{
	list<int> nodeList;

	return nodeList;
}

list<int> Graph::spanning_tree(int start_node, int number_nodes) const{
	list<int> spanningTree;

	return spanningTree;
}

//Destructor for the Graph class
Graph::~Graph(){

}

//Destructor for the WikiGraph class
WikiGraph::~WikiGraph(){

}