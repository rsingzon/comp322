#ifndef comp322_hw3_wikiClasses_h
#define comp322_hw3_wikiClasses_h


#include <vector>
#include <list>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <queue>
#include <cstdlib>
#include <random>
#include <cstdio>
#include <algorithm>
#include <functional>

using namespace std;


/********************************/
// Exceptions
/********************************/

/* simple exception that can be used to accumulate the stack trace*/
class my_exception {
private:
    string stack_trace;
public:
    void addToStack(string s) {
        stack_trace += "\n";
        stack_trace += s;
    }
    
    virtual string get_error_message() {
        return "Stack at failure:" + stack_trace;
    }
};

/* exception that gets thrown in the event of calling the function with
    parameters that could call access to inexistent graph nodes*/
class invalid_graph_id : public my_exception {
private:
    const int id_requested;
public:
    invalid_graph_id(int id_rq): id_requested(id_rq) {}
    string get_error_message() {
        return "Invalid page ID: " + to_string(id_requested) + "\n" + my_exception::get_error_message();
    }
    int get_invalid_id() { return id_requested; }
};

/* exception that gets thrown in the event of calling the function with
 parameters that could call access to inexistent Wikipedia pages*/
class invalid_page_title : public my_exception {
private:
    const string title_requested;
public:
    invalid_page_title(string title_rq): title_requested(title_rq) {}
    string get_error_message() {
        return "Invalid title: " + title_requested + "\n" + my_exception::get_error_message();
    }
};

/* exception that gets thrown in the event of calling the function with
 invalid parameters*/
class invalid_param: public my_exception {
private:
    const string param_fail;
public:
    invalid_param(string param_f): param_fail(param_f) {}
    string get_error_message() {
        return "Invalid Argument: " + param_fail + "\n" + my_exception::get_error_message();
    }
    string get_param_failure() { return param_fail; }
};


/********************************/
// Classes
/********************************/



/* Class that stores information about an abstract graph data structure */
class Graph {
public:
    /* public data */
    struct Edge {
        int origin;
        int destination;
        int weight;
        
        /*operator overload for comparing edges by weight*/
        bool operator<(const Edge& y) const{
            return weight < y.weight;
        }
    };
private:
    int rw_num_walks; // number of random walks to be performed
    int rw_walk_length; // length of each random walk
protected:
    /* private data */
    vector<list<Edge> > adj_list; //adjacency list
    vector<list<Edge> > get_adj_list() const {return adj_list;}
public:
    /*Constructors and Destructor*/
    Graph();  //default constructor
    Graph(Graph& other_graph) ; //copy constuctor
    Graph(list<Edge> lst, int num_vertices); // same as organize list from 2nd homework
    Graph(ifstream& in_file); // reads the list of edges from a file
    ~Graph(); //destructor @TODO
    
    /*public methods - implemented */
    unsigned int get_num_nodes() const { return static_cast<unsigned int>(adj_list.size() - 1);}
    
    /* push_node : adds an additional node to the graph: to do this,
     it adds the new list lst to the graph, as well as additional
     edges to corresponding lists (much like the constructor above) */
    void push_node(list<Edge>& lst);
    
    /* saveToFile : saves the edges of the graph, as explained in Homework 2*/
    void save_to_output_file(ofstream& o_edges) const;
    
    /* sub_graph : returns a new graph that contains only the nodes in the list nodes_to_keep. 
        NOTE: Their IDs in the new graph will be based on the order in which they were read 
                from the list nodes_to_keep */
    Graph sub_graph(list<int> nodes_to_keep) const;
    
    /* set/get random_walk_params */
    void set_num_walks(int p) {
        if(p <= 0) {
            invalid_param e("arg 0, int");
            e.addToStack("set_num_walks\n");
            throw e;
        }
        rw_num_walks = p; }
    void set_walk_length(int p) {
        if(p <= 0) {
            invalid_param e("arg 0, int");
            e.addToStack("set_walk_length\n");
            throw e;
        }
        rw_walk_length = p;
    }
    int get_num_walks() const { return rw_num_walks; }
    int get_walk_length() const { return rw_walk_length; }
    
    
    /*friends*/
    friend ostream& operator<< (ostream& o, Graph const& g);
    
    
    /* public methods - TODO */
    list<int> breadth_first_search(int start_node, int number_nodes) const;
    list<int> spanning_tree(int start_node, int number_nodes) const;
    map<int,int> random_walks(int start_node) const;
private:
    /* public methods - TODO */
    /* sampleEdge : samples an edge from the given list by using the weights of each edge as the
                    histogram for the distribution to be sampled */
    Edge sampleEdge(list<Edge> lst) const;
};



/* WikiGraph : class that implements a graph of Wikipedia pages. It extends Graph
 privately, and provides interface methods for adding and removing pages from the graph.*/
class WikiGraph : public Graph {
public:
    struct WikiPage {
        int ID;
        string title;
        string html_location;
        string txt_location;
    };
    
private:
    // this is a map that returns the node in the abstract graph associated with the wikipage
    // with input title
    map<string,int> title_to_node;
    // this is a map that returns a WikiPage associated with a node in the abstract graph
    vector<WikiPage> node_to_wiki;
public:
    
    /*Constructors and Destructor*/
    WikiGraph() { WikiPage wp_dummy; node_to_wiki.push_back(wp_dummy);} // default
    /* the constructor below reads a WikiGraph that was saved
     in two separate external files */
    WikiGraph(WikiGraph& other_graph); // copy constructor
    WikiGraph(ifstream& in_file_edges, ifstream& in_file_wiki_info); // constructor from files
    ~WikiGraph(); // destructor TODO
    
    /* public methods : implemented */
    
    /* push_page : adds the WikiPage the the graph*/
    void push_page(WikiPage& wp);
    
    /* override push_node to make sure only WikiPages are added to the graph*/
    void push_node(list<Edge>& lst) { cout << "Can only push WikiPages in this graph" << endl;};
    
    /* saves the wiki graph info to two separate files: in one file we save the edges in the graph, like by line. The second output file contains the wikiPages, with all fields of the struct WikiPage on a separate line. Please see the definition of the WikiGraph(ifstream& in_edges, ifstream& in_title_to_node); constructor (BELOW) for more details */
    void save_to_output_files(ofstream& out_edges, ofstream& out_file_wiki) const;
    
    /* friends */
    
    /* operator<< : same as print organized for Wiki Graph */
    friend ostream& operator<< (ostream& o, WikiGraph const& wikiGraph);
    
    /* print_related_pages : prints pages related to "title", based on a set of simple
            random walk over the entire graph
     */
    void print_related_pages(string title, int number_pages) const;
    
    /* print_related_bfs : prints pages related to "title", based on a set of simple
     random walk over the subgraph obtained by running a breadth first search that
     starts at the node represented by title.
     */
    void print_related_bfs(string title, int number_pages, int size_graph) const {
        if (title_to_node.find(title) == title_to_node.end()) {
            invalid_param e("arg 0, string");
            e.addToStack("print_related_bfs\n");
            throw e;
        }
        if (number_pages <= 0) {
            invalid_param e("arg 1, int");
            e.addToStack("print_related_bfs\n");
            throw e;
        }
        if (size_graph <= 0) {
            invalid_param e("arg 2, int");
            e.addToStack("print_related_bfs\n");
            throw e;
        }
        int start_node = title_to_node.at(title);
        list<int> nodes_to_analyse = breadth_first_search(start_node, size_graph);
        print_from_list(nodes_to_analyse, start_node, number_pages);
        
    }
    
    /* print_related_spanning_tree : prints pages related to "title", based on a set of simple
     random walk over the subgraph obtained by computing a spanning tree starting at 
     the node represented by title.
     */
    void print_related_spanning_tree(string title, int number_pages, int size_graph) const {
        if (title_to_node.find(title) == title_to_node.end()) {
            invalid_param e("arg 0, string");
            e.addToStack("print_related_spanning_tree\n");
            throw e;
        }
        if (number_pages <= 0) {
            invalid_param e("arg 1, int");
            e.addToStack("print_related_spanning_tree\n");
            throw e;
        }
        if (size_graph <= 0) {
            invalid_param e("arg 2, int");
            e.addToStack("print_related_spanning_tree\n");
            throw e;
        }int start_node = title_to_node.at(title);
        list<int> nodes_to_analyse = spanning_tree(start_node, size_graph);
        print_from_list(nodes_to_analyse, start_node, number_pages);
    }
    
    void remove_rest() {
        list<int> tmp = spanning_tree(title_to_node["Montreal"], 800);
        set<int> to_keep;
        for( auto x : tmp) to_keep.insert(x);
        
        for(auto x : node_to_wiki) {
            if( to_keep.find(x.ID) == to_keep.end()) {
                string s = x.html_location;
                if( remove(s.c_str()) != 0 )
                    cout << "Error deleting file" << x.txt_location << endl;
                else
                    cout << "File successfully deleted" << endl;
                s = x.txt_location;
                if( remove(s.c_str()) != 0 )
                    cout << "Error deleting file" << x.txt_location << endl;
                else
                    cout << "File successfully deleted" << endl;
            }
        }
    }
private:
    
    /* prints the search results based on the subgraph determined by the input list of nodes*/
    void print_from_list(list<int> nodes_to_analyse, int start_node, int number_pages) const;
    
    
    
};








/********************************/
// Provided methods - facilies for working with the html and text files associated with wikipages
/********************************/


/* getPageTitle : extracts the title of wikipedia page from the HTML source */
string getPageTitle(ifstream& html_file);

/* allAssociations : returns all the values that are associated with the given 'field' in the string 'in_str' */
set<string> allAssociations(ifstream& f);

/* countOccurences : solution to the method in the previous homework */
int countOccurences(ifstream& in_file, string string_to_count);



/* make_wiki_page : reads one wikipage that has html source in path_to_html, and text source in path_to_txt. If one of HTML or TXT sources is missing, then the page should be ignored (return an empty WikiPage. */
WikiGraph::WikiPage& make_wiki_page(string path_to_html, string path_to_txt);



#endif
