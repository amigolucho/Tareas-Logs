#include "bin/trie.cpp"
#include <algorithm>


int main(){

    cout << "Testeamos la insercion" << endl;
    
    Trie Trie;

    vector<string> palabras = {"hola", "mundo", "retumbar", "hola", "felipe"};
    for(string s : palabras){
        Trie.insert(s);
    }
    cout << "Se insertaron con exito las palabras. Deberia tener 27 nodos: nodos finales son " 
         << Trie.nodes_count << endl;
    
    cout << "Testeamos descender" << endl;
    TrieNode* node2 = Trie.nodes.at(8);
    node2 = Trie.descend(node2, 's');
    if(node2==nullptr){
        cout << "No existe este descenso" << endl;
    }else {
        cout << "Se descendio correctamente" << endl;
    }
    
    return 0;
};