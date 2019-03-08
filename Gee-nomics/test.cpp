
#include <stdio.h>
#include <cassert>
#include <iostream>
#include "Trie.h"

using namespace std;

void test1();

int main()
{
    test1();
    std::cout<<"Pass all tests!"<<std::endl;
}


void test1()
{
    Trie<int> trie;
    trie.insert("GATTACA",42); //GATTACA {42}
    trie.insert("GATTACA",17); //GATTACA {42,17}
    trie.insert("GATTACA", 42); // GATTACA {42, 17, 42}
    trie.insert("GCTTACA",30); //GCTTACA {30}
    vector<int> v=trie.find("GATTACA",true);
    for(vector<int>::iterator it=v.begin(); it!=v.end(); it++)
    {
        cout<<(*it)<<endl;
    }
    
    
}
