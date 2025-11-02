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

    for(int i = 0; i <= 14; i++){//18
        Trie Trie(true); // Hay que crearlo desde 0 para poder reiniciar los nodos
        string line;
        int N = pow(2, i);

        for(int j = 1; j <= N; j++){
            getline(in, line);
            //cout << line << endl;
            Trie.insert(line);
        }

        resultados << "Inserción para i = 2^"<< i << ". La cantidad de nodos es: " << Trie.nodes_count << ". Cantidad de characaters: " << Trie.total_caracters << endl;
        in.seekg(0); // se vuelve al inicio
        if(i==14){
            std::cout << Trie.nodes.size() << endl;
        }
    }
    //4.2 Tiempo
    
    //4.3 Análisis de autocompletado
    int real_char = 0;
    while (wiki >> palabra) { // lee palabra por palabra
        std::cout << palabra << endl;
        for(char c : palabra){
            std::cout << trie->nodes.size()<< endl;
            std::cout << c << "\n";
            TrieNode* new_trie = trie->descend(trie->nodes.at(0), palabra.front());
            std::cout << "pene" << "\n";
            if(new_trie == nullptr){
                real_char +=palabra.size();
            }else{
                TrieNode* terminal =  trie->autocomplete(new_trie);
                if(*terminal->str == palabra + '$'){
                    //Sumar llamadas de descent
                }
                trie->update_priority(terminal);
            }
        }
    }

    wiki.close();

    return 0;
};
