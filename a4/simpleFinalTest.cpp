//
//  main.cpp
//  hw4
//
//  Created by Gheorghe Comanici on 2014-03-18.
//  Copyright (c) 2014 Comanici Gheorghe. All rights reserved.
//

#include "wikiFinal.h"


using namespace std;

// change PATH to the location of the folder where you have saved
// the resources for this homework
const string PATH = "/home/singzon/Development/comp322/a4";

string file_input_graph_edges = PATH + "/wg_edges.txt";
string file_input_graph_wikis = PATH + "/wg_wikis.txt";

void generate_all_pages();

int main2() {
    cout  << "Read list of articles from file" << endl;
    ifstream ifedge; ifedge.open(file_input_graph_edges);
    ifstream ifwiki; ifwiki.open(file_input_graph_wikis);
    WikiGraph wg(ifedge,ifwiki);
    
    wg.remove_rest();
    return 0;
}

void displayInterface(string PATH);
int main() {
   
    
   try {
        WikiGraph wg;
        // if you want to read the graph using your own code,
        // change the boolean to false.
        bool read_graph_file = true;
        

        
        if(read_graph_file) {
            cout  << "Read list of articles from file" << endl;
            ifstream ifedge; ifedge.open(file_input_graph_edges);
            ifstream ifwiki; ifwiki.open(file_input_graph_wikis);
            WikiGraph new_g(ifedge,ifwiki);
            wg = new_g;
        }
        
        else {
            cout  << "Creating list of articles" << endl;
            generate_all_pages();
            
            cout  << "Generate graph of articles" << endl;
            
            ifstream ff; ff.open(PATH + "/wpcd/all_pages.txt");
            
            map<string,string> html_txt;
            while (!ff.eof()) {
                string str1, str2;
                getline(ff, str1);
                getline(ff, str2);
                html_txt[str1] = str2;
            }
            
            int max_pages_to_read = 10000;
            for (auto x : html_txt) {
                if (x.first.size() == 0) continue;
                wg.push_page(make_wiki_page(PATH + "/" + x.first,
                                            PATH + "/" + x.second));
                if(--max_pages_to_read <= 0) break;
            }
            ofstream ofedge; ofedge.open(PATH + "/wg_edges.txt");
            ofstream ofwiki; ofwiki.open(PATH + "/wg_wikis.txt");
            wg.save_to_output_files(ofedge, ofwiki);
        }
    }

    catch (my_exception& ex) {
        cout << ex.get_error_message() << endl;
    }

    displayInterface(PATH);    
    return 0;

/*
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

        Graph::Edge sampledEdge = testGraph.sampleEdge(edgeList);*/

    
}




void generate_all_pages()
{
    vector<pair<string, string> > all_ins;
    string path_to_txt = "wpcd/plaintext_articles";
    ifstream ff; ff.open(PATH + "/wpcd/all_txt.txt");
    
    set<string> all_texts;
    
    while (!ff.eof()) {
        string new_path_html;
        getline(ff, new_path_html);
        string s = new_path_html;
        s = s.substr(s.find_last_of("/") + 1, s.length());
        s = s.substr(0, s.find_last_of("."));
        all_texts.insert(s);
    }
    ff.close();
    ff.open( PATH + "/wpcd/all_htm.txt");
    ofstream fout; fout.open( PATH + "/wpcd/all_pages.txt");
    
    while (!ff.eof()) {
        string new_path_html;
        getline(ff, new_path_html);
        if(new_path_html.length() == 0) continue;
        string s = new_path_html;
        s = s.substr(s.find_last_of("/") + 1, s.length());
        s = s.substr(0, s.find_last_of("."));
        if(all_texts.find(s) != all_texts.end()) {
            fout << "wpcd/" + new_path_html << endl;    
            fout << path_to_txt << "/" << s << ".txt" << endl;
        }
    }
    ff.close();
    fout.close();
}

