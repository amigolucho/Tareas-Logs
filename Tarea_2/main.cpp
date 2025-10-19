#include "bin/trie.cpp"
#include <algorithm>


int main(){
    Trie Trie;
    TrieNode root = TrieUtils::create_node(' ');

    Trie.nodes.push_back(&root);


    Trie.insert("hola");
    Trie.insert("holo");

    //node2 = Trie.descend(&node, 'h');


    cout << Trie.nodes.size() << endl;//deveria ser 15
    return 0;
};