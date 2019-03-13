#include "provided.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "Trie.h"
#include <algorithm>

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
    int nGenomes;
    int min_searchLength;
    std::vector<Genome> genomes;
    Trie<pair<int,int>> trie;
    
    bool cmp_GenomeMatch(const GenomeMatch& g1, const GenomeMatch& g2)
    {
        return g1.percentMatch<g2.percentMatch;
    }
};


GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
:nGenomes(0),min_searchLength(minSearchLength)
{}

//Time: O(L* N)
//L : GenomeMatcher's minSearchLength
//N : length of the added Genome's DNA sequence
void GenomeMatcherImpl::addGenome(const Genome& genome)
{
    genomes.push_back(genome);
    int len=genome.length();
    
    //If Genome is shorter than mininum length
    if(len<min_searchLength || len==0)
        return;
    
    //i: starting position of DNA sequence at corresponding genome
    for(int i=0;i+min_searchLength<=len;i++)
    {
        //Construct the pair to be inserted into the trie
        //first: ID of Genome in library
        //second: index of start position
        string fragment;
        if(!genome.extract(i, min_searchLength, fragment))
            break;
        pair<int,int> p;
        p.first=nGenomes;
        p.second=i;
        trie.insert(fragment, p);
    }
    nGenomes++;
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
    
    unordered_map<const Genome*,pair<int,int>> tmp;
    vector<pair<const Genome*, int>> result;

    string searchFragment=fragment.substr(0,min_searchLength);
    
    //Initial match has the same beginning as final matches
    vector<pair<int,int> > InitialMatch= trie.find(searchFragment, exactMatchOnly);
    int MAX_MISMATCH=exactMatchOnly ? 0:1;
    
    //Processing initial search result
    //Each loop run F times, F=length of fragment
    for(vector<pair<int,int> >::iterator it=InitialMatch.begin(); it!=InitialMatch.end(); it++)
    {
        string potentialMatch;
        int ID=it->first;
        int startPosition=it->second;
        int misMatch=0; //Number of mismatches
        int sizeOfPotentialMatch;
        const Genome* g=&genomes[ID];
        
        //True: extract fragment has length equal to fragment length
        //False: fragment has shorter size
        if(!g->extract(startPosition, frag_len, potentialMatch))
        {
            if(g->length()==0)
                continue;
            else g->extract(startPosition, g->length()-startPosition, potentialMatch);
        }
            
        
        sizeOfPotentialMatch=potentialMatch.size();
        if(potentialMatch.size()<minimumLength)
            continue;
        
        //i is length, OR index of last mismatch
        int i;
        for(i=0;i<sizeOfPotentialMatch;i++)
        {
            if(potentialMatch[i]!=fragment[i])
            {
                misMatch++;
            }
            if(misMatch>MAX_MISMATCH) break;
        }
        
        //Search terminates if number of mismatches goes above threshold
        if(misMatch>MAX_MISMATCH && i<minimumLength)
            continue;
        
        else if(i>=minimumLength)
        {
            if(tmp.find(g)==tmp.end())
            {
                pair<int, int> positionAndLength;
                positionAndLength.first=startPosition;
                positionAndLength.second=i;
                tmp[g]=positionAndLength;
            }
            else
            {
                if(i>tmp[g].second)
                {
                    tmp[g].first=startPosition;
                    tmp[g].second=i;
                }
            }
        }
    }
    
    for(unordered_map<const Genome*,pair<int,int>>::iterator it=tmp.begin();it!=tmp.end();it++)
    {
        DNAMatch d;
        d.genomeName=it->first->name();
        d.position=it->second.first;
        d.length=it->second.second;
        matches.push_back(d);
    }
    
    sort(matches.begin(),matches.end(),[this](DNAMatch a, DNAMatch b)
         {
             if(a.length==b.length)
                 return a.genomeName<b.genomeName;
             return a.length>b.length;
             
         });
    
    return !matches.empty();
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    
    if(fragmentMatchLength<min_searchLength || matchPercentThreshold<=0)
        return false;
    
    int query_length=query.length();
    int SEARCHES=query_length/fragmentMatchLength;
    
    //The index increases by fragmentMatchLength each loop
    int i;
    
    //string: stores Genome's name
    //int: store number of searches with successful match
    unordered_map<string, int> results_map;
    
    for(i=0;i<SEARCHES;i++)
    {
        //Initialize target query fragment to be searched
        vector<DNAMatch> matches;
        string queryFragment;
        query.extract(i*fragmentMatchLength,fragmentMatchLength,queryFragment);
        
        //query segment NOT FOUND in all genomes, Continue search of next query_segment
        if(!findGenomesWithThisDNA(queryFragment,fragmentMatchLength,exactMatchOnly,matches))
        {   continue;   }
        for(vector<DNAMatch>::iterator matches_it=matches.begin();matches_it!=matches.end();matches_it++)
        {
            //If this genome does NOT exist in results, insert genome's name and set appearance to 1;
            if(results_map.find(matches_it->genomeName)==results_map.end())
            {
                results_map[matches_it->genomeName]=1;
            }
            
            //If this genome exists in results, increase number of appearance
            else results_map[matches_it->genomeName]++;
            
        }
    }
    
    //int nMatchThreshold=matchPercentThreshold / 100 * SEARCHES;
    results.clear();
    
    //Remove genome with percentage lower than threshold
    for(unordered_map<string, int>::iterator it=results_map.begin();it!=results_map.end(); it++)
    {
        GenomeMatch g;
        g.genomeName=it->first;
        g.percentMatch=static_cast<double>(it->second)*100/SEARCHES;
        if(g.percentMatch<matchPercentThreshold)
            continue;
        results.push_back(g);
    }
    
    //Sort results in descending order
    //If percentage match is same, arrange in alphabetical order
        sort(results.begin(),results.end(),[this](GenomeMatch a, GenomeMatch b)
             {
                 if(a.percentMatch==b.percentMatch)
                     return a.genomeName<b.genomeName;
                 return a.percentMatch>b.percentMatch;
             });
    
    return results.size()>0;
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
