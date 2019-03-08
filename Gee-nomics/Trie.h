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
        TrieNode():children(128,nullptr){}
        std::vector<ValueType> val;
        std::vector<TrieNode*> children;
    };
    
    TrieNode* root;
    
//    int toIndex(const char c) const
//    {
//        switch(c)
//        {
//            case 'A':
//                return 0;
//                break;
//            case 'C':
//                return 1;
//                break;
//            case 'G':
//                return 2;
//                break;
//            case 'T':
//                return 3;
//                break;
//            default:
//                return -1;
//                break;
//        }
//
//    }
//
//    char toBase(const int i) const
//    {
//        {
//            switch(i)
//            {
//                case 0:
//                    return 'A';
//                    break;
//                case 1:
//                    return 'C';
//                    break;
//                case 2:
//                    return 'G';
//                    break;
//                case 3:
//                    return 'T';
//                    break;
//                default:
//                    return 'X';
//                    break;
//            }
//        }
//    }
    
    void searchNode(const TrieNode* p, const std::string& key,int i, bool exactMatchOnly, std::vector<ValueType>& v) const
    {
        int KEY_SIZE=key.size();
        
        //Search ends during midway
        if(!p) return;
        
        //If current node is the last node, concatonate ValueType vector to v
        if(KEY_SIZE==i)
        {
            v.insert(v.end(),p->val.begin(),p->val.end());
            return;
        }
        
        char c=key[i];
        
        std::vector<ValueType> tmp;
        if(p->children[c])
        {
            searchNode(p->children[c], key, i+1, exactMatchOnly,v);
        }
        
        //Not requiring exact match, search for other nodes
        if(!exactMatchOnly)
        {
            for(int j=0;j<128;j++)
            {
                //If children TrieNode is empty
                if(j==c)
                    continue;
                searchNode(p->children[j],key, i+1, true, v);
            }
        }
    }
    
};

template <typename ValueType>
Trie<ValueType>::Trie()
:root(new TrieNode())
{}

template <typename ValueType>
Trie<ValueType>::~Trie()
{
    if(root)
        for(int i=0;i<128;i++)
        {
            delete root->children[i];
        }
    delete root;
}

template <typename ValueType>
void Trie<ValueType>::reset()
{
    ~Trie();
    root=new TrieNode();
}

template <typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value)
{
    //Empty key
    if(key.size()<=0)
        return;
    
    TrieNode* p=root;
    for(char c:key)
    {
        
        if(!p->children[c])
            p->children[c]=new TrieNode();
        p=p->children[c];
    }
    p->val.push_back(value);
    
}

template <typename ValueType>
std::vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const
{
    std::vector<ValueType> v; //Empty vector of ValueType. Return when Errors detected
    
    int KEY_SIZE=key.size();
    if(KEY_SIZE<=0)
        return v;
    
    TrieNode* p=root;
    
    //Now key contains at least one element long
    int index=key[0];
    if(index==-1 || p->children[index]==nullptr)
        return v;
    else
        p=p->children[index];
    
    //If key is only 1 char long
    //This function works fine if value is empty
    if(KEY_SIZE==1)
    {
        v.insert(v.end(),p->val.begin(),p->val.end());
    }
    
    
    searchNode(p,key,1,exactMatchOnly, v);
    return v;
}


#endif // TRIE_INCLUDED
