#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Trie.h"
using namespace std;

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
    std::vector<const Genome*> genomes;
    Trie<pair<const Genome*,int>> trie;
    int min_searchLength;
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
:min_searchLength(0)
{}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
    
    //Num of computation is (GENOME_LENGTH-SEARCH_LENGTH)*SEARCH_LENGTH
    genomes.push_back(&genome);
    int len=genome.length();
    
    //If Genome is shorter than mininum length
    if(len<min_searchLength)
        return;
    
    //Extract genome to be inserted from certain point
    string fragment;
    genome.extract(0, len, fragment);
    
    //i: starting position of DNA sequence at corresponding genome
    for(int i=0;i+min_searchLength<=len;i++)
    {
        
        //j:length of segment to be inserted with start position at i
        for(int j=min_searchLength;j+i<=len;j++)
        {
            //TODO: what if previously already inserted?
            //Construct the pair to be inserted into the trie
            
            //first: vector of Genome* with correspoinding starting node
            //second: index of start position
            pair<const Genome*,int> p;
            p.first=&genome;
            p.second=i;
            trie.insert(fragment.substr(i,j), p);
        }
    }
}

int GenomeMatcherImpl::minimumSearchLength() const
{
    return min_searchLength;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    int frag_len=fragment.size();
    
    if(minimumLength<min_searchLength || frag_len<minimumLength)
        return false;
    
    vector<pair<const Genome*, int>> result;
    
    //i is the starting position of fragment to search
    for(int i=0;i+minimumLength<=frag_len;i++)
    {
        //j is the length of currently searching fragment
        for(int j=min_searchLength; j+i<=frag_len; j++)
        {
            result=trie.find(fragment.substr(i,j),exactMatchOnly);
            
            //IF NOT found, then no longer sequence with starting point at i can be found
            if(result.empty())
            {
                break;
            }
            
            //traverse through the result
            for(vector<pair<const Genome*, int>>::iterator p=result.begin();p!=result.end();p++)
            {
                const Genome* currentGenome=p->first;
                string currentGenomeName=currentGenome->name();
                int currentGenomeStartPosition=p->second;
                
                
                vector<DNAMatch>::iterator matches_it=matches.begin();
                for(;matches_it!=matches.end();matches_it++ )
                {
                    //If DNA sequence already inserted into vector<DNAMatch>& matches
                    //Meaning that segments with same starting sequence appear >=twice
                    if(matches_it->genomeName==currentGenomeName)
                    {
                        //If current Genome has length longer than previous DNAMatch
                        if(j>matches_it->length)
                        {
                            matches_it->length=j;
                            matches_it->position=currentGenomeStartPosition;
                        }
                        
                    }
                    
                }
                
                //If DNA sequence NOT inserted into vector<DNAMatch>& matches
                if(matches_it==matches.end())
                {
                    DNAMatch d;
                    d.genomeName=currentGenomeName;
                    d.position=currentGenomeStartPosition;
                    d.length=j;
                }
                
                
                
                
            }
            
            
            
        }
    }
    
    
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    
    return false;  // This compiles, but may not be correct
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
