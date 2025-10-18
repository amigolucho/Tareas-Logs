#include "bin/trie.cpp"
#include <algorithm>


int main(){
    int * a = nullptr;
    Trie Trie;
    TrieNode node = TrieUtils::create_node(' ');
    Trie.nodes.push_back(&node);
    cout << Trie.nodes.at(0).value()->key << endl;

    Trie.insert("holaaaa");
    Trie.insert("holo");
    Trie.insert("pene");

    cout << Trie.nodes.size() << endl;//deveria ser 10


    return 0;
};