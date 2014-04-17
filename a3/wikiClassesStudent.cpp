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
#include <algorithm>

/**
 * Implementation of the Graph class 
 */

//Constructor for graph class using vertices and edges
Graph::Graph(list<Edge> lst, int num_vertices){

	list<Edge> l_dummy;
	adj_list.push_back(l_dummy);

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

	//Create a new graph using the list of edges and the number of vertices
	Graph tempGraph(edgeList, numVertices);
	adj_list = tempGraph.get_adj_list();

}

//Destructor for graph object
Graph::~Graph(){
	/**
	 * All of the memory for the Graph object is allocated on the stack,
	 * therefore, it is automatically de-allocated once the the program returns
	 * from the function in which is was created.  Thus, there is no need to 
	 * de-allocate memory in the destructor
	 */ 
}

//Overload the << operator in order to print the contents of the graph
ostream& operator<< (ostream& o, Graph const& g){

	int index = 0;

	//Iterate through the lists of edges in the adjacency list
	for (list<Graph::Edge> edgeList : g.get_adj_list()){

		//Don't print dummy values at index 0
		if(index == 0){
			index++; 
			continue;	
		} 

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
            }
		}
	}
}

//Creates a new node in the existing graph and adds edges to the adjacency list
void Graph::push_node(list<Edge>& lst){

	//Add each edge to the destination vertex 
	for(Edge edge : lst){
		//Get the list of edges for the destination vertex
		list<Edge> edgeList = adj_list.at(edge.destination);
		
		//Add edge to list and update the adjacency list in the graph
		edgeList.push_back(edge);
		adj_list.at(edge.destination) = edgeList;
	}

	//Add the new vertex to the adjacency list	
	adj_list.push_back(lst);			
}

/**
 * Implementation of the WikiGraph class
 */

// Destructor for the WikiGraph class
WikiGraph::~WikiGraph(){

	/* All of the fields in the WikiGraph constructor are automatically deleted
	 * since they are created on the stack.
	 * The WikiPages are stored in free memory, but when it is used as a parameter to the 
	 * push_page function, its data is copied to the node_to_wiki vector.  Thus the pointer 
	 * to the free memory is not accessible in the WikiGraph class, and each WikiPage
	 * must be deleted in the function in which it was initially created.
	 */
}

//Creates a new WikiPage from HTML and text source files
WikiGraph::WikiPage& make_wiki_page(string path_to_html, string path_to_txt){
	
	//Dynamically allocate memory for the WikiPage.  Remember to delete in destructor
	WikiGraph::WikiPage *page = new WikiGraph::WikiPage;
	WikiGraph::WikiPage &pageRef = *page;
	page->ID = 0;

	//Open a file stream to read the page title from the HTML source
	ifstream ffi;
	ffi.open(path_to_html);
	
	//Find the title of the page using the input stream
	page->title = getPageTitle(ffi);
	ffi.close();

	//Set the file paths
	page->html_location = path_to_html;
	page->txt_location = path_to_txt;

	return pageRef;
}

//Adds a new WikiPage to the graph
void WikiGraph::push_page(WikiPage& wp){

	//Find the next available ID in the graph
	int id = node_to_wiki.size();
	
	//Set the ID of the WikiPage
	wp.ID = id;

	//Open the HTML file for the WikiPage
	ifstream fs_html;
	fs_html.open(wp.html_location);
	if (fs_html.fail()) { 
		cout << "Failed to open HTML file" << endl; 
	}

	//Obtain the set of linked pages
	set<string> linkedPages = allAssociations(fs_html);
	fs_html.close();	

	//Build a list of edges that are related to the WikiPage
	list<Edge> edgeList;

	//Add the WikiPage to the title_to_node map
	title_to_node.insert(make_pair(wp.title, wp.ID));

	//For all links in the WikiPage, find the weight between the edge
	for(string page : linkedPages){
	
		//Check if the page exists in the title_to_node vector
		if(title_to_node.count(page)){

			//Check if the node_to_wiki vector contains the page
			int page_id = title_to_node[page];
			if(page_id >= node_to_wiki.size()){
				continue;
			}

			//Add a new edge between the new wikipage and the linked page
			Edge newEdge;
			newEdge.origin = wp.ID;
			newEdge.destination = title_to_node[page];

			//Open text files for WikiPages
			ifstream fs_txt;
			fs_txt.open(wp.txt_location);
			if(fs_txt.fail()){
				cout << "Failed to open origin text file" << endl;
			}

			ifstream destination_txt;
			destination_txt.open(node_to_wiki.at(page_id).txt_location);
			if(destination_txt.fail()){
				cout << "Failed to open destination text file" << endl;
			}

			//The weight is the total occurrences on both pages
			newEdge.weight = countOccurences(fs_txt, page) + countOccurences(destination_txt, wp.title);	
			
			//Close the files
			fs_txt.close();
			destination_txt.close();

			if(newEdge.weight > 0){
				edgeList.push_back(newEdge);		
			}
		}
	}

	Graph::push_node(edgeList);
	node_to_wiki.push_back(wp);	
	
}

/*Saves the WikiGraph to two text files:
 * 1. Edges in the graph
 * 2. Information of each WikiPage in the graph
 */
void WikiGraph::save_to_output_files(ofstream& out_edges, ofstream& out_title_to_node)const{

	//First, save the list of edges for each vertex
	save_to_output_file(out_edges);

	bool first = true;
	//Save the fields of each WikiPage struct
	for(WikiPage wp : node_to_wiki){
		
		//Don't save the first entry of node_to_wiki, it is a dummy wp
		if(first){ 
			first = false;
			continue;
		}

		out_title_to_node << wp.ID << endl;
		out_title_to_node << wp.title << endl;
		out_title_to_node << wp.html_location << endl;
		out_title_to_node << wp.txt_location << endl;
	}
}

//Overload the << operator in order to print the contents of the wikiGraph
//Iterate through each of the adjacency lists and print out its edges
ostream& operator<< (ostream& o, WikiGraph const& wikiGraph){
	
	//Initialize variables
	WikiGraph::WikiPage wp;
	vector<list<Graph::Edge>> adj_list = wikiGraph.get_adj_list();

	//Create a vector to sort the strings in alphabetical order
	vector<string> pageOrder;

	//Insert names of pages into the new vector
	pageOrder.push_back(""); //Dummy page
	for(int i = 1; i < wikiGraph.node_to_wiki.size(); i++){
		pageOrder.push_back(wikiGraph.node_to_wiki.at(i).title);
	}

	//Sort alphabetically
	std::sort(pageOrder.begin(), pageOrder.end());


	int index = 1;		//Index of orderedPage vector
	int count = 1;		//Number of vertices printed

	//Iterate through the vertices in the adjacency list
	while(count < adj_list.size()){

		wp = wikiGraph.node_to_wiki.at(index);

		//Check if the name of the WikiPage matches the alphabetized list
		if(wp.title == pageOrder.at(count)){
			o << "Page \"" << wp.title << "\" -> ";
		
			bool first = true;
			list<Graph::Edge> edgeList = adj_list.at(index);
			
			//Iterate through the edges and print the titles and weights
			for(Graph::Edge edge : edgeList){

				//Check if the destination or the origin is the current vertex
				int adjacentVertexID = edge.origin == index ? edge.destination : edge.origin;
				WikiGraph::WikiPage adjacentVertex = wikiGraph.node_to_wiki.at(adjacentVertexID);	

				//Print comma if not the first iteration
				if(!first) o << ", ";
				first = false;
				o << adjacentVertex.title << ":" << edge.weight;
			}
			o << endl;	
			count++;
		}		

		//Wrap around to index 1 of the adjacency list
		index++;
		if (index == adj_list.size()){
			index = 1;
		}
	}

	return o;
}
