#include "bin/trie.cpp"
#include <math.h>
#include <chrono>
#include <iostream>


int main(){
    // Escribimos nuestros resultados
    ofstream resultados("./resultado.txt");
    if (!resultados) {
        cerr << "Error al abrir el archivo de resultados" << endl;
        return 1;
    }

    std::ifstream wiki("./datasets/wikipedia.txt");
    if (!wiki.is_open()) {
        std::cerr << "Error al abrir el archivo\n";
        return 1;
    }
    std::string palabra;

    //4.1 Consumo de memoria
    std::ifstream in("./datasets/words.txt");
    if (!in.is_open()) {
    std::cerr << "Error al abrir archivo para lectura (4.1): " << "words.txt"
              << std::endl;
    std::exit(1);
    }

    Trie* ultimo = nullptr;

    for(int i = 0; i <= 18; i++){//18
        Trie* trie = new Trie(true); // Hay que crearlo desde 0 para poder reiniciar los nodos
        string line;
        int N = pow(2, i);

        for(int j = 1; j <= N; j++){
            getline(in, line);
            //cout << line << endl;
            trie->insert(line);
        }

        resultados << "Inserción para i = 2^"<< i << ". La cantidad de nodos es: " << trie->nodes_count << ". Cantidad de characaters: " << trie->total_caracters << endl;
        in.seekg(0); // se vuelve al inicio
        if(i==18){
            std::cout << trie->nodes.size() << endl;
            ultimo = trie;
            std::cout << ultimo->nodes.at(500)->prefix << endl;
        }
    }
    //std::cout << ultimo->nodes.size() << endl;
    //std::cout << ultimo->nodes.at(500)->prefix << endl;
    //4.2 Tiempo
    
    //4.3 Análisis de autocompletado
    int total_char = 0;
    int real_char = 0;// Cantidad de caracteres realis que debería hacer escrito el usuario
    TrieNode* root = ultimo->nodes.at(0);
    while (wiki >> palabra) { // lee palabra por palabra
        total_char += palabra.size();

        std::cout <<"Para esta iteracion se deberian haber escrito: "<< total_char
        << " caracteres, pero gracias al autocompletado, realmente se han escrito: " << real_char << endl;
        //std::cout << palabra.size() << endl;
        int descend_count = 0;// cuantas veces se ha descendido
        for(char c : palabra){
            if(c == '$'){
                //autocompletado no funciono
                real_char +=palabra.size();
                break;
                //caera en el mismo caso que abajo?
            }
            TrieNode* new_node = ultimo->descend(root, palabra.front());
            descend_count++;

            if(new_node == nullptr){
                real_char +=palabra.size();
                break;
            }else{
                TrieNode* terminal =  ultimo->autocomplete(new_node);
                if(*terminal->str == palabra + '$'){
                    real_char += descend_count;
                    descend_count = 0;
                    ultimo->update_priority(terminal);
                    break;
                }
            }
        }
        descend_count = 0;
    }

    wiki.close();

    return 0;
};
