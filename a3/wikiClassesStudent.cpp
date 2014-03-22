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
		list<Edge> edgeList;

		//Create a list of the vertices which have already been added
		vector<int> existingVertices;

		//If the origin or destination matches the current vertex, add the edge to list
		for(Edge edge: lst){
			if(edge.origin == vertex || edge.destination == vertex){
				int vertexIndex = edge.origin == vertex ? edge.destination : edge.origin;
				bool vertexExists = false;

				//Check if the vertex has already been added to the list
				for (int x : existingVertices){
					if(x == vertexIndex) vertexExists = true;
				}

				//If the vertex does not exist, add it to list
				if(!vertexExists){
					existingVertices.push_back(vertexIndex);
					edgeList.push_back(edge);
				}
			}
		}

		//Add the list to the adjacency list in the graph
		adj_list.push_back(edgeList);

	}
		cout << endl << "Size of adj_list: " << adj_list.size() << endl;

		for (list<Edge> edgeList : adj_list){
			for (Edge edge : edgeList){
				
				cout << edge.origin << " " << edge.destination << " " << edge.weight << endl;
			}
			cout << endl;
		}
}

//Constructor for graph class using an input file
Graph::Graph(ifstream& in_file){
	
	//Create a list of edges for each vertex
	list<Edge> edgeList;

	//Create a list of vertices which have already been added
	vector<int> existingVertices;

	int index = 0;
	int numVertices = 0;
	int value;

	//Iterate through all values in the file
	while (in_file >> value){
		Edge newEdge;
		bool vertexExists = false;

		//Assign first value in line as origin
		if (index == 0){
			newEdge.origin = value;
			
			//Check if the vertex already exists in a list of vertices
			for (int x : existingVertices){
				if (x == newEdge.origin){
					vertexExists = true;
				}
			}

			//If the vertex does not exist, add it and increment number of vertices
			if (!vertexExists){
				existingVertices.push_back(newEdge.origin);
				numVertices++;
			}
		}

		//Assign second value as destination
		else if (index == 1){
			newEdge.destination = value;

			//Check if vertex already exists
			for (int x : existingVertices){
				if (x == newEdge.destination){
					vertexExists = true;
				}
			}

			//Add vertex if it does not exist
			if (!vertexExists){
				existingVertices.push_back(newEdge.destination);
				numVertices++;
			}
		}

		//Assign third value to weight and push edge into list
		else{
			newEdge.weight = value;
			edgeList.push_back(newEdge);
			index = -1;
		}
		index++;
	}

	for (Edge edge : edgeList){
		cout << edge.origin << " " << edge.destination << " " << edge.weight << endl ;
	}

	//Create a new graph using the list of edges and the number of vertices
	Graph tempGraph(edgeList, numVertices);
	adj_list = tempGraph.get_adj_list();

}

//Destructor for graph object
Graph::~Graph(){
	/**
	 * All of the memory for the graph object is allocated on the stack,
	 * therefore, it is automatically de-allocated once the the program returns
	 * from the function in which is was created.  Thus, there is no need to 
	 * de-allocate memory in the destructor
	 */ 
}

//Overload the << operator in order to print the contents of the graph
ostream& operator<< (ostream& o, Graph const& g){

	int index = 1;

	//Iterate through the lists of edges in the adjacency list
	for (list<Graph::Edge> edgeList : g.get_adj_list()){

		//Print the vertex
		o << "Vertex " << index << " -> ";

		bool first = true;

		//Iterate through the edges in the list of edges for each vertex
		for (Graph::Edge edge : edgeList){
			
			//Print a comma if the edge is not the first in the list
			if (!first)	o << ", ";
			first = false;

			//Print the origin or destination, depending on the current index
			int destination = index == edge.origin ? edge.destination : edge.origin;
			o << destination << ":" << edge.weight ;
		}

		o << endl;
		index++;
	}

	return o;
}

void Graph::save_to_output_file(ofstream& o_edges) const{
	//Keep track of which edges have already been saved in a 2D array
	list<Edge> existingEdges;
		
	//Iterate through the lists of edges for each vertex
	for (list<Edge> edgeList : adj_list){

		//Iterate through all of the edges for each of the lists
		for (Edge edge : edgeList){
            bool isEdgeSaved = false;

            //Check if the edge has already been saved
            for(Edge savedEdge : existingEdges){
            	if(savedEdge.origin == edge.origin && savedEdge.destination == edge.destination){
            		isEdgeSaved = true;
            	} 
            	else if(savedEdge.origin == edge.destination && savedEdge.destination == edge.origin){
            		isEdgeSaved = true;
            	}
            }
            
            //If the edge has not already been saved, save it
            if(!isEdgeSaved){
            	existingEdges.push_back(edge);
               
                o_edges << edge.origin << " " << edge.destination << " " << edge.weight << endl;
                cout << edge.origin << " " << edge.destination << " " << edge.weight << endl;
            }
		}
	}
}

//Creates a new node in the existing graph and adds edges to the adjacency list
void Graph::push_node(list<Edge>& lst){

	//Add each edge to the destination vertex 
	for(Edge edge : lst){
		//Get the list of edges for the destination vertex
		list<Edge> vertexList = adj_list.at(edge.destination-1);
		
		//Add edge to list and update the adjacency list in the graph
		vertexList.push_back(edge);
		adj_list.at(edge.destination - 1) = vertexList;
	}

	//Add the new vertex to the adjacency list
	adj_list.push_back(lst);
}

/**
 * Implementation of the WikiGraph class
 */

// Destructor for the WikiGraph class
WikiGraph::~WikiGraph(){

}

//Creates a new WikiPage from HTML and text source files
WikiGraph::WikiPage& make_wiki_page(string path_to_html, string path_to_txt){
	
	//Dynamically allocate memory for the WikiPage.  Remember to DELETE!
	WikiGraph::WikiPage *page = new WikiGraph::WikiPage;
	WikiGraph::WikiPage &pageRef = *page;
	page->ID = 0;

	//Open a file stream to read the page title from the HTML source
	ifstream ffi;
	ffi.open(path_to_html);
	
	//Find the title of the page using the input stream
	page->title = getPageTitle(ffi);
	ffi.close();
	cout << "Title: " << page->title << endl;	

	//Set the file paths
	page->html_location = path_to_html;
	page->txt_location = path_to_txt;

	return pageRef;
}

//Adds a new WikiPage to the graph
void WikiGraph::push_page(WikiPage& wp){

	//Find the next available ID in the graph
	int id = adj_list.size() + 1;
	
	//If the node is still the default page, then replace it with the new page
	if(id == 2){
		if(node_to_wiki.back().title == ""){
			id = 1;
		}
	}

	//Set the ID of the WikiPage
	wp.ID = id;

	//Open the HTML file for the WikiPage
	ifstream ffi;
	ffi.open(wp.html_location);
	if (ffi.fail()) { 
		cout << "Failed to open file" << endl; 
	}

	//Obtain the set of linked pages
	set<string> linkedPages = allAssociations(ffi);
	
	//Build a list of edges that are related to the WikiPage
	list<Edge> edgeList;

	//For all links in the WikiPage, find the weight between the edge
	for(string page : linkedPages){
		int occurrences = countOccurrences(ffi, page);
		
	}

	
	ffi.close();	
}

// Comment
void WikiGraph::save_to_output_files(ofstream& out_edges, ofstream& out_title_to_node)const{

}

//Overload the << operator in order to print the contents of the wikiGraph
//Iterate through each of the adjacency lists and print out its edges
/*
	Output should take the form of:
	Page "Chicago" -> New York:2, Toronto:5, Montreal:14, Miami:4
	Page "Miami" -> Toronto:58, Chicago:4, Montreal:34
	Page "Montreal" -> Chicago:14, Miami:34
	Page "New York" -> Toronto:5, Chicago:2
	Page "Toronto" -> New York:5, Chicago:5, Miami:58
*/
ostream& operator<< (ostream& o, WikiGraph const& wikiGraph){
	
	//Initialize variables
	int index = 1;
	WikiGraph::WikiPage vertex;
	vector<list<Graph::Edge>> adj_list = wikiGraph.get_adj_list();

	//Iterate through the vertices in the adjacency list
	for (list<Graph::Edge> edgeList : adj_list){

		

	}

	//	vertex = page_ofID[index];

	//	//Check if the name of the vertex matches the alphabetized list
	//	if (vertex.title.compare(pageOrder.at(count)) == 0){

	//		std::cout << "Page \"" << vertex.title << "\" -> ";

	//		//Print each adjacent edge and its weight
	//		list<Edge> edgeList = lst.at(index);
	//		for (Edge edge : edgeList){
	//			//Check if the destination or the origin is the current vertex
	//			int adjacentVertexID = edge.origin == index ? edge.destination : edge.origin;
	//			WikiPage adjacentVertex = page_ofID[adjacentVertexID];

	//			std::cout << adjacentVertex.title << ":" << edge.weight << " ";
	//		}
	//		std::cout << std::endl;

	//		count++;
	//	}

	//	//Wrap around to first index of the adjacency list
	//	index++;
	//	if (index == size + 1){
	//		index = 1;
	//	}
	//}

	
	o << "Poopsalot\n";
	return o;
}