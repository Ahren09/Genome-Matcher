#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>

template<typename ValueType>
class Trie
{
public:
    Trie();
    ~Trie();
    void reset();
    void insert(const std::string& key, const ValueType& value);
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

      // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
    struct TrieNode
    {
        TrieNode():children(4,nullptr){}
        vector<ValueType> val;
        vector<TrieNode*> children；
    }
    
    TrieNode* root;
    
    int toIndex(const char c) const
    {
        switch(c)
        {
            case 'A':
                return 0;
                break;
            case 'C':
                return 1;
                break;
            case 'G':
                return 2;
                break;
            case 'T':
                return 3;
                break;
            default:
                return -1;
                break;
        }
        
    }
    
    char toBase(const int i) const
    {
        {
            switch(i)
            {
                case 0:
                    return 'A';
                    break;
                case 1:
                    return 'C';
                    break;
                case 2:
                    return 'G';
                    break;
                case 3:
                    return 'T';
                    break;
                default:
                    return 'X';
                    break;
            }
        }
    }
    
    std::vector<ValueType> searchNode(TrieNode* p, const std::string& key,int i, bool exactMatchOnly)
    {
        std::vector<ValueType> v;
        int KEY_SIZE=key.size();
        
        //If p is nullptr
        if(!p) return v;
        
        //Exact Match
        //If current node is the last node, concatonate ValueType vector to v
        if(KEY_SIZE==i+1)
        {
            v.insert(v.end(),p->val.begin(),p->val.end());
            return v;
        }
        
        char c=key[i];
        int index=toIndex(c);
        
        vector<ValueType>* tmp;
        if(p->children[index])
        {
            tmp=&searchNode(p->children[index], key, i+1, exactMatchOnly);
            v.insert(v.end(),tmp->begin(),tmp->end());
        }
        
        //If we do NOT require exact match
        if(!exactMatchOnly)
        {
            for(int j=0;j<4;i++)
            {
                //If children TrieNode is empty
                if(j==index)
                    continue;
                tmp=&searchNode(p->children[j],key, i+1, true);
                v.insert(v.end(),tmp->begin(),tmp->end());
            }
        }
        return v;
    }
    
};


Trie::Trie()
:root(new TrieNode())
{}

Trie::~Trie()
{
    if(root)
        for(int i=0;i<4;i++)
        {
            delete root->children[i];
        }
    delete root;
}

void Trie::reset()
{
    ~Trie();
    root=new TrieNode();
}

template <typename ValueType>
void Trie::insert(const std::string& key, const ValueType& value)
{
    //Empty key
    if(key.size()<=0)
        return;
    
    TrieNode* p=root;
    int i;
    for(char c:key)
    {
        i=toBase(c);
        
        //If key contains bases other than A,C,G,T
        if(i==-1)
            return;
        
        if(!p->children[i])
            p->children[i]=new TrieNode();
        p=p->children[i];
    }
    p->val.push_back(value);
    
}

template <typename ValueType>
std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const
{
    vector<ValueType> v; //Empty vector of ValueType. Return when Errors detected
    
    int KEY_SIZE=key.size();
    if(KEY_SIZE<=0)
        return v;
    
    TrieNode* p=root;
    //Now key contains at least one element
    int index=toIndex(key[0]);
    if(index==-1 || children[index]==nullptr)
        return v;
    else
        p=children[index];
    
    //If key is only 1 char long
    //This function works fine if value is empty
    if(KEY_SIZE==1)
    {
        v.insert(v.end(),p->val.begin(),p->val.end());
    }
    
    
    v=searchNode(p,key,1,exactMatchOnly);
    return v;
    
    
    
}


#endif // TRIE_INCLUDED
