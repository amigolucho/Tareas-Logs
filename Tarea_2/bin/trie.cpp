#include "../include/trie.hpp"

Trie::Trie(){

}

void Trie::insert(string w){
    cout << this->nodes.size() << endl;
    TrieNode* node = this->nodes.at(0).value();
    w = w + '$'; 
    cout << "Se leyo bien la raiz" << endl;
    /*if (node == nullptr){
            // Crea la raíz en caso de no existir
            TrieNode* Trie;
            node = Trie;
            //node->value = ' ';
            cout << "Era null asi que hay que crearlo" << endl;
        }*/

    // nos aseguramos de siempre pasarle la raiz creada
    
    for (char i: w){
        cout << "Se inserta el caracter "<< i << endl;
        int index = Sigma.find(i);
        TrieNode* hijo = node->next.at(index).value();

        cout << "se lee hijo" << endl;
        if (hijo == nullptr){
            cout << "El hijo es nulo asi que hay que crearlo" << endl;
            // Si no existe el nodo se crea
            TrieNode new_node = TrieUtils::create_node(i);
            new_node.parent = node;
            node->next.at(index) = &new_node;
            
            hijo = &new_node;
            this->nodes.push_back(hijo);
        }
        node = hijo;
    }
    
}

TrieNode* Trie::descend(TrieNode* v,char c){
    int index = Sigma.find(c);
    // Por como se define next, si existe lo devuelve, y si no retorna un null

    return v->next.at(index).value();
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
        TrieNode* padre = v->parent.value();
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
        TrieNode trie;
        trie.key = w;

        trie.next.fill(nullptr);


        return trie;
    }
}