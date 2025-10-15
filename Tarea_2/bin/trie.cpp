#include "../include/trie.hpp"

Trie::Trie(){

}

void Trie::insert(string w){
    TrieNode* node = &this->nodes.at(0);
    w = w + '$'; 

    if (node == NULL){
            // Crea la raíz en caso de no existir
            node = &TrieNode();
            node->value = ' ';
        }
    
    for (char i: w){
        int index = Sigma.find(i);
        TrieNode* hijo = node->next.at(index);

        if (hijo == NULL){
            // Si no existe el nodo se crea
            hijo = &TrieNode();
            hijo->parent = node;
        }
        node = hijo;
    }
    
}

TrieNode* Trie::descend(TrieNode* v,char c){
    int index = Sigma.find(c);
    // Por como se define next, si existe lo devuelve, y si no retorna un null

    return v->next.at(index);
}

TrieNode* autocomplete(Trie v);

void update_priority(TrieNode* v);