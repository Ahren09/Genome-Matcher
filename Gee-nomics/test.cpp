
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
