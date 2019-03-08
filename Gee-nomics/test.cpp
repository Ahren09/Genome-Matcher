
#include <stdio.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include "Trie.h"

using namespace std;

void test1();
void test_ifstream();

int main()
{
    test_ifstream();
    
//    test1();
//    std::cout<<"Pass all tests!"<<std::endl;
}


void test1()
{
    Trie<int> trie;
    trie.insert("GATTACA",42); //GATTACA {42}
    trie.insert("GATTACA",17); //GATTACA {42,17}
    trie.insert("GATTACA", 42); // GATTACA {42, 17, 42}
    trie.insert("GATTACC", 88); // GATTACC {88}
    trie.insert("GCTTACA", 30); //GCTTACA {30}
    trie.insert("ACTTACA", 60); //ACTTACA {60}
    
    vector<int> v=trie.find("GATTACA",false);
    
    vector<int> tester1={42,17,42,88,30};
    assert(tester1==v);
//    for(vector<int>::iterator it=v.begin(); it!=v.end(); it++)
//    {
//        cout<<(*it)<<endl;
//    }
    
    
}

void test_GenomeImpl
{
    Genome
    
}

void test_ifstream()
{
    ifstream input("/Users/jinyiqiao/Desktop/Git/Gee-nomics/test.txt");
    if(!input)
    {
        cerr<<"Cannot open input file!"<<endl;
        return;
    }
    string content;
    int i=0;
    string name;
    getline(input,name);
    char c;
    string tmp;
    if(name[0]=='>')
    {
        name=name.substr(1);
        cout<<"NAME IS:"<<name<<endl;
//        while(getline(input,tmp)){
//            if(tmp[0]=='>')
//            {
//                name=tmp.substr(1);
//                cout<<content<<endl;
//                content="";
//            }
//            else content+=tmp;
//
//        }
        
//        while(getline(input,content))
//        {
//            input.get(c);
//            if(c=='>')
//            {
//                getline(input,name);
//                cout<<content<<endl;
//                cout<<"NAME IS:"<<name<<endl;
//                content="";
//            }
//            else{
//                content+=c;
//            }
//
//        }
        
        while(1)
        {
            input>>content;
            input.ignore('>');
        }
        
    }
    //cout<<output;
}
