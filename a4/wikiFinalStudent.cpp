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
Graph::Edge Graph::sampleEdge(list<Graph::Edge> lst) const{
	Graph::Edge sampleEdge;
	int totalWeight;
	int index = 0;
	vector<int> cumul;
	
	//Find the total weight from the sum of weights of each edge
	for (list<Graph::Edge>::iterator it = lst.begin(); it != lst.end(); ++it){
		
		//Check if any edges have weight <= 0
		try{
			if(it->weight <= 0){
				throw invalid_param("Edge weight less than or equal to zero");
			} 
			else{
				//Update the current weight and add the current weight to the vector
				totalWeight += it->weight;
				cumul.push_back(totalWeight);		
				index++;
			}
		}
		catch(const invalid_param& e){
			//cerr << "Index " << index << " of edge list has weight less than or equal to zero." << endl;
		}
	}

	//Obtain a random number between 0 and totalWeight-1
	int randNum = (rand() % totalWeight-1);

	//cout << "Random integer: " << randNum << endl;
	
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


//Traverse the graph rw_num_walks times with walks of length rw_walk_length
map<int, int> Graph::random_walks(int start_node) const{
	map<int, int> walk;
 	vector<list<Edge>> adj_list = get_adj_list();

 	int walkCount = 0;
 	int lengthCount = 0;

 	//Perform rw_num_walk walks
 	for(walkCount = 0; walkCount < get_num_walks(); walkCount++){

 		//Visit rw_walk_length nodes
 		for(lengthCount = 0; lengthCount < get_walk_length(); lengthCount++){

 			list<Edge> edgeList = adj_list.at(start_node);
 			Edge newEdge = sampleEdge(edgeList);

 			try{
 				//Check if the start node is invalid
 				if(newEdge.origin != start_node){
	 				throw invalid_graph_id(newEdge.origin);
 				}

 				//Increment the number of times the node has been visited
 				if(walk.count(start_node)){
 					int timesVisited = walk[start_node];	
 					timesVisited++;
 					walk[start_node] = timesVisited;
 				}
 				else{
 					walk[start_node] = 1;
 				}

 			}
 			catch(const invalid_graph_id& e){

 			}
  		}
 	}
	return walk;
}

//Performs a breadth first search to find nodes related to start_node
list<int> Graph::breadth_first_search(int start_node, int number_nodes) const{
	vector<list<Graph::Edge>> adj_list = get_adj_list();
	list<int> nodeList;
	set<int> elementsToReturn;	//Keeps elements that should be returned
	queue<int> waitList;		//Keeps a list of nodes to be added to the set
	int count = 1;				//Number of nodes visited
	
	//Add the first to the queue and set of added nodes
	waitList.push(start_node);
	elementsToReturn.insert(start_node);

	try{
		
		//Check if the start node is invalid
		if(start_node <= 0 || start_node > adj_list.size()){
			throw invalid_graph_id(start_node);
		}

		//Check if the number of nodes is less than or equal to zero
		if(number_nodes <= 0){
			throw invalid_param("The number of nodes used for the BFS is less than or equal to zero");
		}
	
		while(!waitList.empty() && count >= number_nodes){

			//Get the next node in the queue
			int currentNode = waitList.front();
			waitList.pop();

			//Add current node to list
			nodeList.push_back(currentNode);

			//Increment the number of nodes added to the list
			count++;

			//Iterate through each of the neighbours of the current node
			int neighbour;
			list<Graph::Edge> edgeList = adj_list.at(currentNode);

			for(Graph::Edge edge : edgeList){

				//Get the ID of the neighbouring node
				neighbour = edge.origin == currentNode ? edge.destination : edge.origin;

				//Add neighbours to queue if they have not already been added
				if(elementsToReturn.count(neighbour) == 0){
					waitList.push(neighbour);
					elementsToReturn.insert(neighbour);
				}
			}
		}	
	}
	/////*****TODO: Why are the references to the exceptions constants?
	catch(const invalid_graph_id& e){

	}
	catch(const invalid_param& e){

	}

	return nodeList;
}

list<int> Graph::spanning_tree(int start_node, int number_nodes) const{
	list<int> spanningTree;
	set<int> elementsToReturn;
	priority_queue<Edge> waitList;

	try{

	}
	catch(const invalid_graph_id& e){

	}
	catch(const invalid_param& e){

	}

	return spanningTree;
}

//Destructor for the Graph class
Graph::~Graph(){

}

//Destructor for the WikiGraph class
WikiGraph::~WikiGraph(){

}