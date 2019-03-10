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
    ~GenomeMatcherImpl();
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
    std::vector<const Genome*> genomes;
    Trie<pair<const Genome*,int>> trie;
    int min_searchLength;
    
    bool cmp_GenomeMatch(const GenomeMatch& g1, const GenomeMatch& g2)
    {
        return g1.percentMatch<g2.percentMatch;
    }
    
};





GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
:min_searchLength(minSearchLength)
{}

GenomeMatcherImpl::~GenomeMatcherImpl()
{
}

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
            
            //Number of insertion:
            //(gen_len-min_len+1)*(gen_len-min_len+2)/2
            
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
    
    //j is the length of currently searching fragment
    //run F times, F=length of fragment
    for(int j=minimumLength; j<=frag_len; j++)
    {
        result=trie.find(fragment.substr(0,j),exactMatchOnly);
        
        //IF NOT found, then no longer sequence with starting point at i can be found
        if(result.empty())
        {
            break;
        }
        
        //Traverse through the result
        for(vector<pair<const Genome*, int>>::iterator p=result.begin();p!=result.end();p++)
        {
            const Genome* currentGenome=p->first;
            string currentGenomeName=currentGenome->name();
            int currentGenomeStartPosition=p->second;
            //bool genome_found_at_current_length=false;
            bool no_this_genome_in_matches=true;
            
            vector<DNAMatch>::iterator matches_it=matches.begin();
            for(;matches_it!=matches.end();matches_it++ )
            {
                //If DNA sequence already inserted into vector<DNAMatch>& matches
                //Meaning that segments with same starting sequence appear >=twice
                
                //Every genome has at most 1 DNAMatch in matches
                if(matches_it->genomeName==currentGenomeName)
                {
                    no_this_genome_in_matches=false;
                    //If current Genome has length longer than previous DNAMatch
                    if(j>matches_it->length)
                    {
                        matches_it->length=j;
                        matches_it->position=currentGenomeStartPosition;
                    }
                    
                }
                
            }
            
            //If DNA sequence NOT inserted into vector<DNAMatch>& matches
            if(no_this_genome_in_matches)
            {
                DNAMatch d;
                d.genomeName=currentGenomeName;
                d.position=currentGenomeStartPosition;
                d.length=j;
                matches.push_back(d);
            }
            
        }
            
            
            
        
    }
    return !matches.empty();
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    if(fragmentMatchLength<min_searchLength || matchPercentThreshold<=0)
        return false;
    
    int query_length=query.length();
    
    int SEARCHES=query_length/fragmentMatchLength;
    double MATCH_PERCENTAGE=100/SEARCHES;
    
    //The index increases by fragmentMatchLength each loop
    int i;
    
    for(i=0;i<SEARCHES;i++)
    {
        //Initialize target query fragment to be searched
        vector<DNAMatch> matches;
        string queryFragment;
        query.extract(i*fragmentMatchLength,fragmentMatchLength,queryFragment);
        
        //query segment NOT FOUND in all genomes, Continue search of next query_segment
        if(!findGenomesWithThisDNA(queryFragment,min_searchLength,exactMatchOnly,matches))
        {
            continue;
        }
        for(vector<DNAMatch>::iterator matches_it=matches.begin();matches_it!=matches.end();matches_it++)
        {
            bool genome_exists_in_results = false;
            
            //If this genome exists in results, increase percent of match
            for(vector<GenomeMatch>::iterator results_it=results.begin();results_it!=results.end();results_it++)
            if(matches_it->genomeName==results_it->genomeName)
            {
                genome_exists_in_results=true;
                results_it->percentMatch+=MATCH_PERCENTAGE;
                break;
            }
            
            //If this genome does NOT exist in results, construct a new GenomeMatch for it
            if(!genome_exists_in_results)
            {
                GenomeMatch g;
                g.genomeName=matches_it->genomeName;
                g.percentMatch=MATCH_PERCENTAGE;
                results.push_back(g);
            }
        }
        
    }
            
    //Sort results in descending order
    //If percentage match is same, arrange in alphabetical order
    sort(results.begin(),results.end(),[this](GenomeMatch a, GenomeMatch b)
         {
             if(a.percentMatch==b.percentMatch)
                 return a.genomeName<b.genomeName;
             return a.percentMatch>b.percentMatch;
         });
    
    int SIZE=results.size();
    //Remove genome with percentage lower than threshold
    for(i=0; results[i].percentMatch>=matchPercentThreshold;i++);
    results.resize(i);
    
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
