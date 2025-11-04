#include "bin/trie.cpp"
#include <math.h>
#include <chrono>
#include <iostream>


int main(){
    std::ifstream wiki("./datasets/wikipedia.txt");
    if (!wiki.is_open()) {
        std::cerr << "Error al abrir el archivo\n";
        return 1;
    }
    std::ifstream in("./datasets/words.txt");
    if (!in.is_open()) {
    std::cerr << "Error al abrir archivo para lectura (4.1): " << "words.txt"
              << std::endl;
    std::exit(1);
    }
    
    //4.1 Consumo de memoria y 4.2 Tiempo
    ofstream memoria("./resultados/resultado_memoria.csv");
    if (!memoria) {
        cerr << "Error al abrir el archivo de resultados de memoria" << endl;
        return 1;
    }
    memoria << "i,nodes,characters"<< endl;
    //-------------------------------------------------------------------------
    ofstream tiempo("./resultados/resultado_tiempo.csv");
    if (!tiempo) {
        cerr << "Error al abrir el archivo de resultados de tiempo" << endl;
        return 1;
    }
    tiempo << "N/M,t,characters"<< endl;


    Trie* ultimo = nullptr;
    std::string palabra;
    int c_pal= 0;

    for(int i = 0; i <= 18; i++){//18
        Trie* trie = new Trie(true); // Hay que crearlo desde 0 para poder reiniciar los nodos
        string line;
        int N = pow(2, i);

        for(int j = 1; j <= N; j++){
            getline(in, line);
            trie->insert(line);
        }
        memoria <<i<<","<<trie->nodes_count<<","<<trie->total_caracters<< endl;
        in.seekg(0); // se vuelve al inicio
        if(i==18){
            // Guardamos el último trie para trabajar con el después
            std::cout << trie->nodes.size() << endl;
            ultimo = trie;
            std::cout << ultimo->nodes.at(500)->prefix << endl;
        }
    }
    //4.2 Tiempo
    
    //4.3 Análisis de autocompletado
    int total_char = 0;// Cantidad de caracteres que debería haber escrito el usuario
    int real_char = 0;// Cantidad de caracteres reales que escribe el usuario gracias al autocompletado
    TrieNode* root = ultimo->nodes.at(0);
    while (wiki >> palabra && log2(c_pal) <= 4) { // lee palabra por palabra
        c_pal++;
        total_char += palabra.size();

        int descend_count = 0;// cuantas veces se ha descendido
        TrieNode* new_node = root;
        for(char c : palabra){
            if(c == '$'){
                //autocompletado no funciono
                real_char +=palabra.size();
                break;
                //caera en el mismo caso que abajo?
            }
            new_node = ultimo->descend(new_node, c);
            descend_count++;
            //cout << "ene 1" << endl;
            if(new_node == nullptr){
                //cout << "ene 2" << endl;
                //cout << "newnode es nulo para "<< c << endl;
                real_char +=palabra.size();
                break;
            }else{
                TrieNode* terminal =  ultimo->autocomplete(new_node);
                //cout << "ene 3" << endl;
                if(terminal == nullptr){
                    //cout << "ene 4" << endl;
                    // terminal nulo es que nunca ha sido visitado
                    continue;
                }
                if(*terminal->str == palabra + '$'){
                    //cout << "ene 5" << endl;
                    real_char += descend_count;
                    descend_count = 0;
                    ultimo->update_priority(terminal);
                    break;
                }
                //cout << "ene" << endl;
            }
        }
        descend_count = 0;
        //std::cout <<"Para esta iteracion se deberian haber escrito: "<< total_char
        //<< " caracteres, pero gracias al autocompletado, realmente se han escrito: " << real_char << endl;
        cout<< "cantidad de palabras escritas: 2^"<< log2(c_pal) <<endl;
    }

    wiki.close();

    return 0;
};
