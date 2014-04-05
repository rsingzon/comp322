#include "wikiFinal.h"


/***************************************************************/
/**************** Graph ****************************************/
/***************************************************************/

Graph::Graph() : rw_num_walks(100), rw_walk_length(10) { //default constructor
    list<Edge> l_dummy;
    adj_list.push_back(l_dummy);
}


Graph::Graph(Graph& other_graph) : rw_num_walks(other_graph.rw_num_walks), rw_walk_length(other_graph.rw_walk_length){ //copy constuctor
    adj_list.resize(other_graph.adj_list.size());
    for (int i = 1; i < adj_list.size(); i++) {
        for(auto z : other_graph.adj_list[i]) {
            adj_list[i].push_back(z);
        }
    }
    
    
}

Graph::Graph(list<Edge> lst, int num_vertices) : rw_num_walks(100), rw_walk_length(10){ // same as organize list from 2nd homework
    if (num_vertices <= 0) { //there are no vertices
        return ;
    }
    //allocate memory for every vertex, indexed from 1 to numberVertices
    adj_list.resize(num_vertices+1);
    
    for (auto& e : lst) { // investigate every edge in lst
        
        if(e.origin > num_vertices || e.origin < 1 ) {
            invalid_graph_id ex(e.origin);
            ex.addToStack("Graph(list<Edge>, int)");
            throw ex;
        }if(e.destination > num_vertices || e.destination < 1) {
            invalid_graph_id ex(e.destination);
            ex.addToStack("Graph(list<Edge>, int)");
            throw ex;
        }
        adj_list[e.origin].push_back(e); // push the forward edge
        Edge erev {e.destination, e.origin, e.weight}; // create reverse edge
        adj_list[e.destination].push_back(erev); // push the reverse edge
    }
}

Graph::Graph(ifstream& in_file) try : rw_num_walks(100), rw_walk_length(10) { // reads the list of edges from a file
    int largest_n = 0; //need need to create a new graph
    
    list<Edge> toAdd; //list of edges to add
    
    int origin, destination, weight; // used for input
    in_file >> origin >> destination >> weight;
    while (!in_file.eof()) {
        toAdd.push_back(Edge {origin, destination, weight} );
        in_file >> origin >> destination >> weight;
        
        //update largest
        largest_n = (origin > largest_n) ? origin : largest_n;
        largest_n = (destination > largest_n) ? destination : largest_n;
        
    }
    //create new graph, and extract adjacency list
    Graph g(toAdd, largest_n);
    this->adj_list = g.adj_list;
}  catch (my_exception& ex) {
    ex.addToStack("Graph(ifstream&)");
    throw;
} catch (ifstream::failure e) {
    std::cerr << "Exception opening/reading/closing file\n";
    throw;
}



void Graph::push_node(list<Edge>& lst) {
    for (Edge& e : lst) {
        if(e.origin != adj_list.size() ) {
            invalid_graph_id ex(e.origin);
            ex.addToStack("push_node");
            throw ex;
        }
        if(e.destination < 1 || e.destination >= adj_list.size() ) {
            invalid_graph_id ex(e.destination);
            ex.addToStack("push_node");
            throw ex;
        }
    }
    list<Edge> new_list = lst; // new list to add at the end of the graph
    
    // add reverse edges
    for (Edge& e : lst) {
        adj_list[e.destination].push_back(Edge{e.destination, e.origin, e.weight});
    }
    // add all edges
    adj_list.push_back(new_list);
}

/* saveToFile : saves the edges of the graph, as explained in Homework 2*/
void Graph::save_to_output_file(ofstream& o_edges) const try {
    for (list<Edge> lst : adj_list) {
        for(Edge& e :lst) {
            if(e.origin < e.destination) {
                o_edges << e.origin << " " << e.destination;
                o_edges << " " << e.weight << endl;
            }
        }
    }
} catch (ofstream::failure e) {
    std::cerr << "Exception opening/reading/closing file\n";
    throw;
}



ostream& operator<< (ostream& o, Graph const& graph) try {
    // ignore index 0, as vertices are indexed from 1, not from 0
    for(int i=1; i < graph.adj_list.size(); i++) {
        o << "Vertex " << i << " -> ";
        for (auto& e : graph.adj_list[i]) {
            o << e.destination << ":" << e.weight << "; ";
        }
        o <<endl;
    }
    return o;
} catch (ofstream::failure e) {
    std::cerr << "Exception opening/reading/closing file\n";
    throw;
}


/***************************************************************/
/**************** Wiki Graph ****************************************/
/***************************************************************/


WikiGraph::WikiGraph(WikiGraph& other_graph) : Graph(other_graph) {
    // create title_to_node
    for (auto& x : other_graph.title_to_node) {
        title_to_node[x.first] = x.second;
    }
    // create node_to_wiki
    node_to_wiki.resize(other_graph.node_to_wiki.size());
    for (int i=1; i < node_to_wiki.size(); i++) {
        node_to_wiki[i] = other_graph.node_to_wiki[i];
    }
} // copy constructor


WikiGraph::WikiGraph(ifstream& in_file_edges, ifstream& in_file_wiki_info) try : Graph(in_file_edges) {
    int largest_index = 0; // need this to create node_to_wiki
    string ss; // used for reading
    list<WikiPage> lst; // create all wikipages, before adding to node_to_wiki
    
    while (!in_file_wiki_info.eof()) {
        getline(in_file_wiki_info, ss);
        if (ss.length() == 0) break;
        WikiPage new_page;
        new_page.ID = stoi(ss);
        getline(in_file_wiki_info, new_page.title);
        getline(in_file_wiki_info, new_page.html_location);
        getline(in_file_wiki_info, new_page.txt_location);
        lst.push_back(new_page);
        largest_index = (largest_index < new_page.ID) ? new_page.ID : largest_index;
    }
    // create data strucutes
    node_to_wiki.resize(largest_index+1);
    for(auto& x : lst) {
        title_to_node[x.title] = x.ID;
        node_to_wiki[x.ID] = x;
    }
} catch (ifstream::failure e) {
    std::cerr << "Exception opening/reading/closing file\n";
}



void WikiGraph::push_page(WikiPage& wp) try {
    //update the ID of the new page
    wp.ID = get_num_nodes() + 1;
    // update WikiGraph data
    title_to_node[wp.title] = wp.ID;
    node_to_wiki.push_back(wp);
    
    // create list of Edges to be added to the graph
    list<Edge> p_neigh;
    if (title_to_node.size() == 1) { // if empty graph
        Graph::push_node(p_neigh);
        return;
    }
    
    ifstream f(wp.html_location);
    //get all possible articles (i.e. linked to the input wiki page in html)
    set<string> allPossibleArticles = allAssociations(f);
    for(auto& s : allPossibleArticles) {
        if (title_to_node.find(s) == title_to_node.end()) {  //check this
            continue;
        }
        if(s == wp.title) continue;
        if(s.length() == 0) continue;
        
        // compute weight
        ifstream page_1_in (wp.txt_location.c_str());
        ifstream page_2_in (node_to_wiki[title_to_node[s]].txt_location.c_str());
        if(!page_1_in.is_open()) { cout << "Cannot open: " << wp.txt_location << endl; continue;}
        if(!page_2_in.is_open()) { cout << "Cannot open: " << node_to_wiki[title_to_node[s]].txt_location << endl; continue;}
        int weight = countOccurences(page_1_in, s) + countOccurences(page_2_in, wp.title);
        page_1_in.close();
        page_2_in.close();
        if( weight > 0) {
            Edge e { title_to_node[wp.title], title_to_node[s], weight};
            p_neigh.push_back(e);
        }
    }
    Graph::push_node(p_neigh);
} catch (my_exception& ex) {
    ex.addToStack("push_page");
    throw;
}



void WikiGraph::save_to_output_files(ofstream& out_edges, ofstream& out_file_wiki) const try {
    save_to_output_file(out_edges);
    for (int i=1; i < node_to_wiki.size(); i++) {
        out_file_wiki << i << endl;
        out_file_wiki << node_to_wiki[i].title << endl;
        out_file_wiki << node_to_wiki[i].html_location << endl;
        out_file_wiki << node_to_wiki[i].txt_location << endl;
    }
} catch (std::ofstream::failure e) {
    std::cerr << "Exception opening/reading/closing file\n";
}


void WikiGraph::print_related_pages(string title, int number_pages) const try {
    if(title_to_node.find(title) == title_to_node.end()) {
        invalid_page_title ex(title);
        throw ex;
    }
    int start_node = title_to_node.at(title); // get start node
    
    // get the counts for the random walk
    map<int,int> rw_counts = random_walks(start_node);
    // sort pages by the visit count, in descending order
    vector<pair<int,int>> counts_and_nodes;
    for(pair<int,int> a :rw_counts) {
        counts_and_nodes.push_back(make_pair(a.second, a.first));
    }
    sort(counts_and_nodes.begin(), counts_and_nodes.end(), greater<pair<int,int> >());
    
    // print pages in descending order of number of visits
    for(pair<int, int> a : counts_and_nodes) {
        cout << node_to_wiki[a.second].title << ": " << a.first << " visits" << endl;
        if (--number_pages <= 0) break;
    }
} catch (my_exception& ex) {
   ex.addToStack("print_related_pages");
   throw;
}


void WikiGraph::print_from_list(list<int> nodes_to_analyse, int start_node, int number_pages) const try {
    Graph g;
    g = sub_graph(nodes_to_analyse);
    g.set_num_walks(get_num_walks());
    g.set_walk_length(get_walk_length());
    
    map<int,int> old_ids;
    int i=1;
    int new_start_node = 1;
    for(const auto& a : nodes_to_analyse) {
        if(a == start_node) new_start_node = i;
        old_ids[i++] = a;
    }
    
    // get the counts for the random walk
    map<int,int> rw_counts = g.random_walks(new_start_node);
    // sort pages by the visit count, in descending order
    vector<pair<int,int>> counts_and_nodes;
    for(pair<int,int> a :rw_counts) {
        counts_and_nodes.push_back(make_pair(a.second, a.first));
    }
    sort(counts_and_nodes.begin(), counts_and_nodes.end(), greater<pair<int,int> >());
    
    // print pages in descending order of number of visits
    for(pair<int, int> a : counts_and_nodes) {
        cout << node_to_wiki[old_ids[a.second]].title << ": " << a.first << " visits" << endl;
        if(--number_pages <= 0) break;
    }
} catch (my_exception& ex) {
    ex.addToStack("print_from_list");
    throw;
}


bool sortByArticleName(WikiGraph::WikiPage w1, WikiGraph::WikiPage w2) {
    return w1.title < w2.title;
}


ostream& operator<< (ostream& o, WikiGraph const& g) {
    if (g.get_num_nodes() == 0) return o ; //nothing to print
    
    // we will have to order all indeces from 1 to lst.size()
    vector<WikiGraph::WikiPage> printOrder(g.node_to_wiki.size()-1);
    for (int i=0; i < printOrder.size(); i++) {
        printOrder[i] = g.node_to_wiki[i+1];
    }
    sort(printOrder.begin(), printOrder.end(), sortByArticleName);
    
    //visit the adjacency lists in the order that was determined by the sort function
    for(auto& wp : printOrder) {
        cout << "Page \"" << wp.title << "\" -> ";
        auto lst = g.get_adj_list();
        for (auto& e : lst[wp.ID]) {
            cout << g.node_to_wiki[e.destination].title << ":" << e.weight << ", ";
        }
        cout << endl;
    }
    return o;
}


Graph Graph::sub_graph(list<int> nodes_to_keep) const {
    for (int x : nodes_to_keep) {
        if (x < 1 || x >= adj_list.size()) {
            invalid_graph_id ex(x);
            ex.addToStack("sub_graph");
            throw ex;
        }
    }
    Graph g;
    map<int,int> new_ids; //have to save the ids that each node will have in the new graph
    
    int id_new = 1;
    for(int x: nodes_to_keep) {
        new_ids[x] = id_new++; //save id
        list<Edge> new_adj; // will have to create a new adjacency list (based on selection)
        for(Edge e : adj_list[x]){
            if(new_ids.find(e.destination) != new_ids.end()) { //if connected to something already existing
                g.adj_list[new_ids[e.destination]].push_back(Edge{new_ids[e.destination],new_ids[e.origin],e.weight}); // add backward edge
                new_adj.push_back(Edge{new_ids[e.origin],new_ids[e.destination],e.weight}); //add the crt edge to the new list
            }
        }
        g.adj_list.push_back(new_adj); // add new list to the adj list
    }
    return g;
}




//Standards used by the source html files
const string LINKED_PAGE_IN_HTML = "title=";
const string ARTICLE_NAME_IN_HTML = "wgTitle";


/* findAndExtractAssociatedValue : will search the string, but instead of extracting the position where 'field' is found, it actually saves the data associated with this field in 'result' and returns the pointer after this result, for other searches*/
size_t findAndExtractAssociatedValue(const string& inputString, size_t start_pos, const string& field, string&result) {
    size_t pos = inputString.find(field, start_pos);
    if( pos != string::npos) {
        size_t p1 = inputString.find("\"", pos + field.length());
        size_t p2 = inputString.find("\"", p1 + 1);
        result = inputString.substr(p1+1,p2-p1-1);
        return p2+1;
    }else return string::npos;
}

/* getPageTitle : extracts the title of wikipedia page from the HTML source */
string getPageTitle(ifstream& html_file) {
    if(!html_file.is_open()){
        cout << "getPageTitle : the file was not properly opened" << endl;
        return "";
    }
    string result;
    while (!html_file.eof()) {
        string line;
        getline(html_file, line);
        size_t pos = findAndExtractAssociatedValue(line, 0, ARTICLE_NAME_IN_HTML, result);
        if(pos != string::npos)
            break;
    }
    return result;
}

/* allAssociations : returns all the title of all pages which are linked (through a hyperlink) in the input file */
set<string> allAssociations(ifstream& f) {
    string field = LINKED_PAGE_IN_HTML;
    set<string> toRet;
    if(!f.is_open()) return toRet;
    while (!f.eof()) {
        string line;
        getline(f, line);
        string result;
        for(size_t pos = findAndExtractAssociatedValue(line, 0, field, result);
            pos != string::npos;
            pos = findAndExtractAssociatedValue(line, pos, field, result)) {
            toRet.insert(result);
        }
    }
    return toRet;
}

/* countOccurences : solution to the method in the previous homework */
int countOccurences(ifstream& in_file, string string_to_count) {
    int toRet = 0;
    string all_file;
    while (!in_file.eof()) {
        string line;
        getline(in_file, line);
        all_file += line;
        all_file += " ";
    }
    for (size_t pos = all_file.find(string_to_count);
         pos != string::npos;
         pos = all_file.find(string_to_count,pos + string_to_count.length())){
        toRet++;
    }
    return toRet;
}



WikiGraph::WikiPage& make_wiki_page(string path_to_html, string path_to_txt) {
    auto toRet = new WikiGraph::WikiPage;
    WikiGraph::WikiPage& new_page = *toRet; //get reference to the newly created object
    
    ifstream html_file(path_to_html.c_str()); //open html file
    new_page.title = getPageTitle(html_file); // use already existing method to get title
    html_file.close();
    new_page.txt_location = path_to_txt; //save the path to the txt file
    new_page.html_location = path_to_html; //save the path to the html file
    return new_page;
}