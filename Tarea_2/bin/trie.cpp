#include "../include/trie.hpp"

void Trie::insert(string w){
    TrieNode* node = this->nodes.at(0);// Partimos insertando en la raiz
    w += '$'; 

    TrieNode* hijo;
    
    for (char i: w){
        int index = Sigma.find(i); 

        if (node->next.at(index) == nullptr){
            // Si no existe el nodo se crea
            TrieNode* new_node = new TrieNode();
            nodes_count++;
            new_node->key = i;
            new_node->parent = node;
            new_node->prefix = node->prefix + i;
            node->next.at(index) = new_node;
            this->nodes.push_back(node->next.at(index));
        }
        node = node->next.at(index);

        if(i == '$'){
            node->str = new string(w);
            node->best_terminal = node;
            node->best_terminal = 0;
        }
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