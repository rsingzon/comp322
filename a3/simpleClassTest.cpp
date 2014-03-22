#include "wikiClasses.h"

//change path if your wiki files are in a separate location
const string path = "./";

int main() {
    
	//Test creating an abstract graph
	list<Graph::Edge> edgeList;
	Graph::Edge e1 = { 1, 4, 5 };
	Graph::Edge e2 = { 1, 2, 2 };
	Graph::Edge e3 = { 4, 2, 5 };
	Graph::Edge e4 = { 4, 5, 58 };
	Graph::Edge e5 = { 2, 3, 14 };
	Graph::Edge e6 = { 5, 2, 4 };
	Graph::Edge e7 = { 5, 3, 34 };

	edgeList.push_back(e1);
	edgeList.push_back(e2);
	edgeList.push_back(e3);
	edgeList.push_back(e4);
	edgeList.push_back(e5);
	edgeList.push_back(e6);
	edgeList.push_back(e7);

	Graph testGraph(edgeList, 5);
	
	cout << "Contents of graph" << endl;
	cout << testGraph;

/*	//Test output file
	cout << endl << "Saving graph..." << endl;
	ofstream ffo; ffo.open("test_graph.txt");
	if (ffo.fail()) { cout << "File could not be opened" << endl; return 1; }
	testGraph.save_to_output_file(ffo);
	ffo.close();

	//Open saved file
	cout << endl << "Loading graph..." << endl;
	ifstream ffi;
	ffi.open("test_graph.txt");
	if (ffi.fail()) { cout << "File could not be opened" << endl; return 1; }
	Graph fileGraph(ffi);

	cout << endl << "Input file graph" << endl;
	cout << fileGraph;*/

    //Test push_node
    list<Graph::Edge> newEdgeList;
    Graph::Edge newE1 = { 6, 4, 5 };
    Graph::Edge newE2 = { 6, 2, 2 };
    Graph::Edge newE3 = { 6, 2, 5 };
    Graph::Edge newE4 = { 6, 5, 58 };
    Graph::Edge newE5 = { 6, 3, 14 };
    Graph::Edge newE6 = { 6, 2, 4 };
    Graph::Edge newE7 = { 6, 3, 34 };

    newEdgeList.push_back(newE1);
    newEdgeList.push_back(newE2);
    newEdgeList.push_back(newE3);
    newEdgeList.push_back(newE4);
    newEdgeList.push_back(newE5);
    newEdgeList.push_back(newE6);
    newEdgeList.push_back(newE7);

    testGraph.push_node(newEdgeList);

    cout << endl << "Adjacency list with new node" << endl;
    cout << testGraph << endl;


	
    cout  << "Creating list of articles" << endl;
    
    WikiGraph wg;
	
    wg.push_page(make_wiki_page(path + "wiki/wp/n/New_York_City.htm",
                                path + "wiki/text/New_York_City.txt"));
    cout << endl;
    wg.push_page(make_wiki_page(path + "wiki/wp/c/Chicago.htm",
                                path + "wiki/text/Chicago.txt"));
    cout << endl;
    wg.push_page(make_wiki_page(path + "wiki/wp/t/Toronto.htm",
                                path + "wiki/text/Toronto.txt"));
    cout << endl;
    wg.push_page(make_wiki_page(path + "wiki/wp/m/Montreal.htm",
                                path + "wiki/text/Montreal.txt"));
    cout << endl;
    wg.push_page(make_wiki_page(path + "wiki/wp/m/Miami%2C_Florida.htm",
                                path + "wiki/text/Miami%2C_Florida.txt"));
    cout << endl;
    
    cout << "Testing the management of the graph" << endl;
    cout << "Here is the graph as organizing wikipedia pages" << endl;
    cout << "===============================================" << endl;
    cout << wg;
    cout << "===============================================" << endl;
	/*
	cout << "Here is the underlying abstract graph" << endl;
    cout << "=====================================" << endl;
    Graph g = wg;
    cout << g;
    cout << "=====================================" << endl;
    
    
    const string out_edge_file = "/Users/gcoman/out_graph.txt";
    const string out_wiki_file = "/Users/gcoman/out_wiki.txt";
    cout << endl << "Testing the file input and output:" << endl;
    ofstream ffo_edge; ffo_edge.open(out_edge_file);
    if(ffo_edge.fail()) { cout << "File " << out_edge_file << "could not be opened" << endl; return 1;}
    ofstream ffo_wiki; ffo_wiki.open(out_wiki_file);
    if(ffo_wiki.fail()) { cout << "File " << out_wiki_file << " could not be opened" << endl; return 1;}
    wg.save_to_output_files(ffo_edge, ffo_wiki);
    ffo_wiki.close();ffo_edge.close();
    
    ifstream ffi_edge; ffi_edge.open(out_edge_file);
    if(ffi_edge.fail()) { cout << "File " << out_edge_file << "could not be opened" << endl; return 1;}
    ifstream ffi_wiki; ffi_wiki.open(out_wiki_file);
    if(ffi_wiki.fail()) { cout << "File " << out_wiki_file << " could not be opened" << endl; return 1;}
    WikiGraph wg2(ffi_edge, ffi_wiki);
    ffi_edge.close();ffi_wiki.close();
    
    
    
    cout << "Here is the graph as organizing wikipedia pages" << endl;
    cout << "===============================================" << endl;
    cout << wg2;
    cout << "===============================================" << endl;
    cout << "Here is the underlying abstract graph" << endl;
    cout << "=====================================" << endl;
    Graph g2 = wg2;
    cout << g2;
    cout << "=====================================" << endl;*/
}
