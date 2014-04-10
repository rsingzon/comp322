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
	list<int> nodeList;			//List of nodes to return
	set<int> elementsToReturn;	//Keeps elements that should be returned
	queue<int> waitList;		//Keeps a list of nodes to be added to the set
	int count = 1;				//Number of nodes visited

	vector<list<Graph::Edge>> adj_list = get_adj_list();

	try{
		
		//Check if the start node is invalid
		if(start_node <= 0 || start_node > adj_list.size()){
			throw invalid_graph_id(start_node);
		}

		//Check if the number of nodes is less than or equal to zero
		if(number_nodes <= 0){
			throw invalid_param("The number of nodes used for the BFS is less than or equal to zero");
		}

		//Add the first to the queue and set of added nodes
		waitList.push(start_node);
		elementsToReturn.insert(start_node);
	
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
	list<int> spanningTree;			//Spanning tree to return
	set<int> elementsToReturn;		//IDs of nodes already in the spanning tree
	priority_queue<Edge> waitList;	//Priority queue holding nodes to be added to the tree
	int count = 1;					//Number of nodes in the spanning tree

	vector<list<Graph::Edge>> adj_list = get_adj_list();

	try{

		//Check that the start node is valid
		if(start_node <= 0 || start_node > adj_list.size()){
			throw invalid_graph_id(start_node);
		}

		//Check that the number of nodes is greater than zero
		if(number_nodes <= 0){
			throw invalid_param("The number of nodes for the spanning tree is less than or equal to zero");
		}

		//Add the neighbours of the start node to the priority queue
		for(Graph::Edge edge : adj_list.at(start_node)){
			waitList.push(edge);
		}

	elementsToReturn.insert(start_node);

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

	/* This destructor does not need to do anything because the WikiGraph class
	 * does not dynamically allocate memory onto the heap, and thus all of its 
	 * fields are deleted 
	 */
}

//Method to check user input
void isIntegerGreaterThanZero(int& x){
	bool valid = false;
    while(!valid){
    	
    	//Check if an integers was entered
        if(cin >> x){	
            if(x > 0){
            	valid = true;	
            } 
        }
        else {
        	cin.clear();
        }
		
		//Empty input stream
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if(!valid) cout << "Oops! " << x << " is not a valid number." << endl;
    }
}

//Displays messages and accepts commands on the command line
void displayInterface(string PATH){
    WikiGraph wg;
    bool valid = false;
    int relatedPages;
    int subgraphSize;
    int numWalks;
    int walkLength;
    string articleName;

    cout << "Welcome to the Wikipedia page traversal system!" << endl;

    while(true){
    	cout << "Please enter the number of related pages you would like to retrieve: " << endl;
	    isIntegerGreaterThanZero(relatedPages);

	    cout << "Please enter the size of the subgraph to be used for the BFS and spanning tree:" << endl;
	    isIntegerGreaterThanZero(subgraphSize);

	    cout << "Please enter the number of random walks to perform: " << endl;
	    isIntegerGreaterThanZero(numWalks);

	    cout << "Please enter the length of each walk: " << endl;
	    isIntegerGreaterThanZero(walkLength);

	    cout << "Please enter the name of a Wikipedia article: " << endl;
	    cin >> articleName;	

	    //Exit if the user types exit
	    transform(articleName.begin(), articleName.end(), articleName.begin(), ::tolower);
	    if(articleName.compare("exit") == 0){
	    	break;
	    }

		
		//Print the results from the search        	
    	try {
            string start_page(articleName);
            cout << "-------------------------------------" << endl;
            cout << "Results using full graph random walks" << endl;
            cout << "-------------------------------------" << endl;
            wg.print_related_pages(articleName, relatedPages);
            cout << "------------------------------------------" << endl;
            cout << "Results using random walks on BFS subgraph" << endl;
            cout << "------------------------------------------" << endl;
            wg.print_related_bfs(articleName, relatedPages, subgraphSize);
            cout << "------------------------------------------" << endl;
            cout << "Results using random walks on Spanning Tree subgraph" << endl;
            cout << "-------------------" << endl;
            wg.print_related_spanning_tree(articleName, relatedPages, subgraphSize);
            cout << "-------------------" << endl;
        }
     
        catch (invalid_graph_id& ex){
        	cout << "Sorry, the requested graph ID is invalid." << endl;
        	cout << "Stacktrace" << ex.get_error_message() << endl;
        }

        catch (invalid_page_title& ex){
        	cout << "Sorry, the page you requested is invalid." << endl;
        	cout << "Stacktrace" << ex.get_error_message() << endl;
        }

        catch (invalid_param& ex){
        	cout << "Sorry, a parameter entered is invalid." << endl;
        	cout << "Stacktrace" << ex.get_error_message() << endl;
        }

        catch (my_exception& ex) {
            cout << ex.get_error_message() << endl;
        }
    } 
}

