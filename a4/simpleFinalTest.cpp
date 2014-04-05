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
const string PATH = "/Users/gcoman/Documents/wiki_cs322";

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
            
        }else {
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
        
        int num_results = 10;
        int size_graph = 100;
        while (true) {
            try {
                cout << "Please enter a page to study (type exit to stop):";
                char input[100];
                cin.getline(input, 100);
                string start_page(input);
                if(start_page == "exit") break;
                cout << "-------------------" << endl;
                cout << "Results using full graph random walks" << endl;
                cout << "-------------------" << endl;
                wg.print_related_pages(start_page, num_results);
                cout << "-------------------" << endl;
                cout << "Results using random walks on BFS subgraph" << endl;
                cout << "-------------------" << endl;
                wg.print_related_bfs(start_page, num_results, size_graph);
                cout << "-------------------" << endl;
                cout << "Results using random walks on Spanning Tree subgraph" << endl;
                cout << "-------------------" << endl;
                wg.print_related_spanning_tree(start_page, num_results, size_graph);
                cout << "-------------------" << endl;
                break; // if search was succesful
            }catch (my_exception& ex) {
                cout << ex.get_error_message() << endl;
            }
        }
    } catch (my_exception& ex) {
        cout << ex.get_error_message() << endl;
    }
    
    return 0;
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

