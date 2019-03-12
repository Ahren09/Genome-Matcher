#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
    string seq;
    string m_name;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
:m_name(nm),seq(sequence)
{}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) 
{
    //Report open failure
    if(!genomeSource)
    {
        return false;
    }
    
    std::string name;
    std::string gene;
    char tmp='X';
    
    //Get the first Genome's name
    //Return false if name is empty
    if(getline(genomeSource,name))
    {
        //Now name contains at least 1 char
        if(name[0]=='>')
        {
            name=name.substr(1);
            if(name=="")
            {
                cerr<<"Error: Empty name"<<endl;
                return false;
            }
        }
        //Not starting with a properly-formatted name line
        else
        {
            cerr<<"Error: Not starting with '>'"<<endl;
            return false;
        }
    }
    //Empty file
    else
    {
        cerr<<"Error: Empty file!"<<endl;
        return false;
    }
    
    while(genomeSource.get(tmp))
    {
        if(tmp=='>')
        {
            //Create new Genome using previous contents
            Genome g(name,gene);
            genomes.push_back(g);
            if(!getline(genomeSource,name)) //This will skip the '>' before each name of Genome
            {
                cerr<<"Error: Empty name"<<endl;
                return false;
            }
            gene="";
            
        }
        else if(tmp=='\n')
            continue;
        
        else
        {
            toupper(tmp);
            if(tmp != 'A' && tmp != 'C' && tmp != 'G' && tmp != 'T' && tmp != 'N')
            {
                cerr<<"Error: Containing chars other than ACGTN"<<endl;
                return false;
            }
            gene+=tmp;
        }
        
    }
    
    //If genome sequence is empty
    if(gene=="")
    {
        cerr<<"Error: Empty sequence!"<<endl;
        return false;
    }
    
    
    //Create the last genome
    Genome g(name,gene);
    genomes.push_back(g);
    
    return true;
    
}

int GenomeImpl::length() const
{
    return seq.length();
}

string GenomeImpl::name() const
{
    return m_name;
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
    int len=this->length();
    if(position<0 || len<0)
        return false;
    
    //If extract reaches end of fragment, return fragment after position
    if(position+length>=len)
    {
        fragment=seq.substr(position);
        return false;
    }
    fragment=seq.substr(position,length);
    return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes) 
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}
