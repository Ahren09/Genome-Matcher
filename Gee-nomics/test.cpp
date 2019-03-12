
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
void test_extract();
void test_GenomeMatcher1();
void test_GenomeMatcher2();
void test_GenomeMatcher3();

int main()
{
//    test_GenomeImpl();
//    test_GenomeImpl_Exceptions();

//    test1();
//    test_extract();
    test_GenomeMatcher1();
//    test_GenomeMatcher2();
//    test_GenomeMatcher3();
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
    
    ifstream inputFile5("/Users/jinyiqiao/Desktop/Git/Gee-nomics/test_files/test_genome_secondNameEmpty.txt");
    assert(!Genome::load(inputFile5, v));
    
}

void test_extract()
{
    Genome g("oryx", "GCTCGGNACACATCCGCCGCGGACGGGACGGGATTCGGGCTGTCGATTGTCTCACAGATCGTCGACGTACATGACTGGGA");
    string f1, f2, f3;
    bool result1 = g.extract(0, 5, f1); // result1 = true, f1 = “GCTCG”;
    bool result2 = g.extract(74, 6, f2); // result2 = true, f2 = “CTGGGA”;
    bool result3 = g.extract(74, 7, f3); // result3 = false, f3 is unchanged
    
    assert(g.length()==80 && g.name()=="oryx");
    assert(result1 && f1=="GCTCG");
    assert(result2 && f2=="CTGGGA");
    assert(!result3 && f3=="");
    
    
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


void test_GenomeMatcher1()
{
    Genome g1("Genome_1","CGGTGTACNACGACTGGGGATAGAATATCTTGACGTCGTACCGGTTGTAGTCGTTCGACCGAAGGGTTCCGCGCCAGTAC");
    Genome g2("Genome_2","TAACAGAGCGGTNATATTGTTACGAATCACGTGCGAGACTTAGAGCCAGAATATGAAGTAGTGATTCAGCAACCAAGCGG");
    Genome g3("Genome_3","TTTTGAGCCAGCGACGCGGCTTGCTTAACGAAGCGGAAGAGTAGGTTGGACACATTNGGCGGCACAGCGCTTTTGAGCCA");
    
    
    GenomeMatcher gm1(4);
    gm1.addGenome(g1);
    gm1.addGenome(g2);
    gm1.addGenome(g3);
    
    std::vector<DNAMatch> matches;
    bool result;
    result = gm1.findGenomesWithThisDNA("GAAG", 4, true, matches);
    assert(result);
    int SIZE=matches.size();
    for(int i=0;i<SIZE;i++)
    {
        cout<<matches[i].genomeName<<" of length "<<matches[i].length<<" at position "<<matches[i].position<<endl;
    }
    cout<<endl;
    
    
    std::vector<DNAMatch> matches_2;
    result = gm1.findGenomesWithThisDNA("GAATAC", 4, true, matches_2);
    SIZE=matches_2.size();
    for(int i=0;i<SIZE;i++)
    {
        cout<<matches_2[i].genomeName<<" of length "<<matches_2[i].length<<" at position "<<matches_2[i].position<<endl;
    }
    assert(result);
    
    std::vector<DNAMatch> matches_3;
    result = gm1.findGenomesWithThisDNA("GAATAC", 6, true, matches_3);
    assert(!result);
    cout<<endl;
    

    
    std::vector<DNAMatch> matches_4;
    result = gm1.findGenomesWithThisDNA("GAATAC", 6, false, matches_4);
    SIZE=matches_4.size();
    for(int i=0;i<SIZE;i++)
    {
        cout<<matches_4[i].genomeName<<" of length "<<matches_4[i].length<<" at position "<<matches_4[i].position<<endl;
    }
    cout<<endl;
    assert(result);
    
    cout<<"Test 5: GTATAT, 6, false, matches:"<<endl;
    std::vector<DNAMatch> matches_5;
    result = gm1.findGenomesWithThisDNA("GTATAT", 6, false, matches_5);
    SIZE=matches_5.size();
    for(int i=0;i<SIZE;i++)
    {
        cout<<matches_5[i].genomeName<<" of length "<<matches_5[i].length<<" at position "<<matches_5[i].position<<endl;
    }
    cout<<endl;
    assert(result);
    
    cout<<"Test 6: GAATACG, 6, false, matches:"<<endl;
    std::vector<DNAMatch> matches_6;
    result = gm1.findGenomesWithThisDNA("GAATACG", 6, false, matches_6);
    SIZE=matches_6.size();
    for(int i=0;i<SIZE;i++)
    {
        cout<<matches_6[i].genomeName<<" of length "<<matches_6[i].length<<" at position "<<matches_6[i].position<<endl;
    }
    cout<<endl;
    assert(result);
    
    result=false;
    cout<<"Test 7: GAAGGGTT, 5, false, matches:"<<endl;
    std::vector<DNAMatch> matches_7;
    result = gm1.findGenomesWithThisDNA("GAAGGGTT", 5, false, matches_7);
    SIZE=matches_7.size();
    for(int i=0;i<SIZE;i++)
    {
        cout<<matches_7[i].genomeName<<" of length "<<matches_7[i].length<<" at position "<<matches_7[i].position<<endl;
    }
    cout<<endl;
    assert(result);
    
    result=false;
    cout<<"Test 8: GAAGGGTT, 6, false, matches:"<<endl;
    std::vector<DNAMatch> matches_8;
    result = gm1.findGenomesWithThisDNA("GAAGGGTT", 6, false, matches_8);
    SIZE=matches_8.size();
    for(int i=0;i<SIZE;i++)
    {
        cout<<matches_8[i].genomeName<<" of length "<<matches_8[i].length<<" at position "<<matches_8[i].position<<endl;
    }
    cout<<endl;
    assert(result);
    
    result=false;
    cout<<"Test 9: ACGTGCGAGACTTAGAGCC, 12, false, matches:"<<endl;
    std::vector<DNAMatch> matches_9;
    result = gm1.findGenomesWithThisDNA("ACGTGCGAGACTTAGAGCC", 12, false, matches_9);
    SIZE=matches_9.size();
    for(int i=0;i<SIZE;i++)
    {
        cout<<matches_9[i].genomeName<<" of length "<<matches_9[i].length<<" at position "<<matches_9[i].position<<endl;
    }
    cout<<endl;
    assert(result);
    
    
    result=false;
    cout<<"Test 10: ACGTGCGAGACTTAGAGCG, 12, false, matches:"<<endl;
    std::vector<DNAMatch> matches_10;
    result = gm1.findGenomesWithThisDNA("ACGTGCGAGACTTAGAGCG", 12, false, matches_10);
    SIZE=matches_10.size();
    for(int i=0;i<SIZE;i++)
    {
        cout<<matches_10[i].genomeName<<" of length "<<matches_10[i].length<<" at position "<<matches_10[i].position<<endl;
    }
    cout<<endl;
    assert(result);
    
    std::vector<DNAMatch> matches_11;
    result = gm1.findGenomesWithThisDNA("GAAG", 3, true, matches_11);
    assert(!result);
    result=true;
    result = gm1.findGenomesWithThisDNA("GAAG", 5, true, matches_11);
    assert(!result);
}

void test_GenomeMatcher2()
{
    Genome g1("Genome_a","ACGTATACNACGAGCGGGGATAGAATTTTCTGACGTCGTACCGGTTGTAGTCGTTCGACCGAAGGGTTCCGCGCCAGTAC");
    Genome g2("Genome_b","TAACAGAGCGGTNATATTGTTACGAATCACGTGCGAGACTTAGAGCCAGAATATGAAGTAGTGATTCAGCAACCAAGCGG");
    Genome g3("Genome_c","TTTTGAGCCAGCGACGCGGCTTGCTTAACGAAGCGGAAGAGTAGGTTGGACACATTNGGCGGCACAGCGCTTTTGAGCCA");
    //                    *1   *6   *11  *16  *21  *26  *31  *36  *41  *46  *51  *56  *61  *66  *71  *76
    
    Genome g4("Genome_A","ACGTACNTNCAA");
    Genome g5("Genome_B","ACGAACGTACAAANAA");
    Genome g6("Genome_C","ACGTACGTACGT");
    
    Genome g7("Genome_C","ACGTACGTACGT");
    Genome g8("Genome_C","ACGT");
    Genome g9("Genome_C","ACGTACGTACGT");
    
    GenomeMatcher gm1(4);
    gm1.addGenome(g1);
    gm1.addGenome(g2);
    gm1.addGenome(g3);
    gm1.addGenome(g4);
    gm1.addGenome(g5);
    gm1.addGenome(g6);
    
    std::vector<GenomeMatch> results;
    Genome query("Query","ACGTACNTANTTTTC");
    gm1.findRelatedGenomes(query, 5, true, 30, results);
    
    int SIZE=results.size();
    cout.setf(ios::fixed);
    cout.precision(2);
    for(int i=0;i<SIZE;i++)
    {
        cout<<results[i].genomeName<<" has percentage: "<<results[i].percentMatch<<endl;
    }
}

void test_GenomeMatcher3()
{
    Genome g5("Genome_sameas_a_2","AAAAGTCCCCTTTTATCCCA");
    Genome g1("Genome_a","AAAAGTCCCCTTTTATCCCA");
    Genome g2("Genome_b","TCCCNGAAATTTTTGCCCCCNNNNN");
    Genome g3("Genome_c","TTTTGAGCCAGCGACGCGGCTTGCTTAACGAAGCGGAAGAGTAGGTTGGACACATTNGGCGGCACAGCGCTTTTGAGCCA");
    Genome g4("Genome_sameas_a","AAAAGTCCCCTTTTATCCCA");
    
    //                    *1   *6   *11  *16  *21  *26  *31  *36  *41  *46  *51  *56  *61  *66  *71  *76
    GenomeMatcher gm1(4);
    gm1.addGenome(g1);
    gm1.addGenome(g2);
    gm1.addGenome(g3);
    gm1.addGenome(g4);
    gm1.addGenome(g5);
//    gm1.addGenome(g6);
    
    std::vector<GenomeMatch> results;
    Genome query("Query","AAAAGCNNNATTTTGACGNTCCCNCATATATCCCAGAAATCCCCC");
    //                    *1   *6   *11  *16  *21  *26  *31  *36  *41
    
    gm1.findRelatedGenomes(query, 5, true, 11.11, results);
    int SIZE=results.size();
    cout.setf(ios::fixed);
    cout.precision(2);
    for(int i=0;i<SIZE;i++)
    {
        cout<<results[i].genomeName<<" has percentage: "<<results[i].percentMatch<<endl;
    }
}
