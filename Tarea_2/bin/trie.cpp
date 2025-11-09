#include "../include/trie.hpp"

void Trie::insert(string w){
    this->contador_palabras += 1; 
    this->total_caracters += w.size(); // sumamos antes para no contar el $
    TrieNode* node = this->nodes.at(0);// Partimos insertando en la raiz
    w += '$'; 
    
    for (char i: w){
        int index = Sigma.find(i); 

        if (node->next.at(index) == nullptr){
            // Si no existe el nodo se crea
            TrieNode* new_node = new TrieNode();
            this->nodes_count++;
            new_node->key = i;
            new_node->parent = node;
            new_node->prefix = node->prefix + i;
            node->next.at(index) = new_node;
            this->nodes.push_back(node->next.at(index));
        }
        
        if(i == '$'){
            node->str = &w;
            node->best_terminal = node;
            node->best_priority = 0;
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
    TrieNode* terminal = v->best_terminal;// va a depender de la variante
    
    return terminal;
}

void Trie::update_priority(TrieNode* v){
    if(this->es_frecuencia){// Frecuencia
        v->priority += 1;
    }else{// Reciente
        this->timestamp += 1;
        v->priority = this->timestamp;
    }
    TrieUtils::update_info(v);
}


namespace TrieUtils {
    void update_info(TrieNode* v){
        TrieNode* padre = v->parent;
        if (padre == nullptr){
            //es la raíz
            return ;
        }

        if (padre->best_priority <= v->priority){
            // Si es menor hay que actualizar
            padre->best_priority = v->priority;
            padre->best_terminal = v;
            update_info(padre);
        }
    }

    Trie* copyTrie(Trie* trie){
        Trie* new_trie = new Trie(trie->es_frecuencia);
        new_trie->nodes = trie->nodes;
        new_trie->nodes_count = trie->nodes_count;
        new_trie->total_caracters = trie->total_caracters; 
        new_trie->contador_palabras = trie->contador_palabras;
        new_trie->timestamp = trie->timestamp;

        return new_trie;
    }
}