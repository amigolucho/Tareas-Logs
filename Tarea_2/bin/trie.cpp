#include "../include/trie.hpp"
#include <typeinfo>


void Trie::insert(string w){
    TrieNode* node = this->nodes.at(0);
    w = w + '$'; 

    TrieNode* hijo;
    
    for (char i: w){
        int index = Sigma.find(i);

        hijo = node->next.at(index);
        cout << "Se inserta el caracter "<< i << " con el indice "<< index << endl;
        
        if (node->next.at(index) == nullptr){
            // Si no existe el nodo se crea
            TrieNode new_node = TrieUtils::create_node(i);
            new_node.parent = node;
            new_node.prefix.push_back(i);
            //cout<< i <<'\n';
            node->next.at(index) = &new_node;
            this->nodes.push_back(node->next.at(index));
        }
        node = node->next.at(index);
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
    
    TrieNode create_node(char w){
        TrieNode* trie = new TrieNode();  
        trie->key = w;
        trie->prefix = "";
        trie->next.fill(nullptr);
        trie->parent = nullptr;
        trie->str = nullptr;
        trie->best_terminal = nullptr;
        trie->priority = 0;
        trie->best_priority = 0;
        
        cout << "Se crea el nodo con el caracter " << trie->key << endl;
        
        return *trie;
    }
}