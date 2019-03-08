
#include <stdio.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include "Trie.h"
#include "provided.h"

using namespace std;

void test1();
void test_ifstream();
void test_GenomeImpl();
void test_GenomeImpl_Exceptions();

int main()
{
    test_GenomeImpl();
    test_GenomeImpl_Exceptions();
    
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

void test_GenomeImpl()
{
    vector<Genome> v;
    ifstream inputFile("/Users/jinyiqiao/Desktop/Git/Gee-nomics/test_files/test_genome_1.txt");
    if(!inputFile)
    {
        cerr<<"Input file not found!"<<endl;
        exit(1);
    }
    Genome::load(inputFile, v);
    
    string fragment;
    
    
    for(int i=0;i<4;i++)
    {
        bool isSuccessful=v[i].extract(i, 5, fragment);
        if(isSuccessful)
            cout<<"Extract failure!";
        else cout<<"Genome "<<i<<" is:"<<endl<<fragment<<endl;
    }
    
}

void test_GenomeImpl_Exceptions()
{
    vector<Genome> v;
    ifstream inputFile("/Users/jinyiqiao/Desktop/Git/Gee-nomics/test_files/test_genome_empty.txt");
    assert(!Genome::load(inputFile, v));
    
    ifstream inputFile2("/Users/jinyiqiao/Desktop/Git/Gee-nomics/test_files/test_genome_empty_genome.txt");
    assert(!Genome::load(inputFile2, v));
    
    ifstream inputFile3("/Users/jinyiqiao/Desktop/Git/Gee-nomics/test_files/test_genome_empty_name.txt");
    assert(!Genome::load(inputFile3, v));
    
    ifstream inputFile4("/Users/jinyiqiao/Desktop/Git/Gee-nomics/test_files/test_genome_empty_Extraneous.txt");
    assert(!Genome::load(inputFile4, v));
    assert(v.empty());
    
}

void test_ifstream()
{
    ifstream input("/Users/jinyiqiao/Desktop/Git/Gee-nomics/test_files/test.txt");
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

