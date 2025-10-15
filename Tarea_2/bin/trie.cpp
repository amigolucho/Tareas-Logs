#include "../include/trie.hpp"

Trie::Trie(){

}

void Trie::insert(string w){
    cout << this->nodes.size() << endl;
    TrieNode* node = &this->nodes.at(0);
    w = w + '$'; 

    if (node == NULL){
            // Crea la raíz en caso de no existir
            TrieNode* Trie;
            node = Trie;
            node->value = ' ';
        }
    
    for (char i: w){
        int index = Sigma.find(i);
        TrieNode* hijo = node->next.at(index);

        if (hijo == NULL){
            // Si no existe el nodo se crea
            TrieNode* Trie;
            hijo = Trie;
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

TrieNode* Trie::autocomplete(TrieNode* v){
    TrieNode* terminal = v->best_terminal;
    
    return terminal;
}

void Trie::update_priority(TrieNode* v){
    // Frecuencia
    //v->priority += 1;
    // reciente
    this->timestamp += 1;
    v->priority = this->timestamp;
    
    TrieUtils::update_info(v);
}


namespace TrieUtils {
    void update_info(TrieNode* v){
        TrieNode* padre = v->parent;
        if (padre == NULL){
            return ;
        }

        if (padre->best_priority < v->priority){
            // Si es menor hay que actualizar
            padre->best_priority = v->priority;
            padre->best_terminal = v;
            update_info(padre);
        }
    }
}