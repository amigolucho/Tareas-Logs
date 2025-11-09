#include "bin/trie.cpp"
#include <math.h>
#include <chrono>
#include <iostream>


int main(){
    using namespace std::chrono;

    std::ifstream wiki("./datasets/wikipedia.txt");
    if (!wiki.is_open()) {
        std::cerr << "Error al abrir el archivo\n";
        return 1;
    }
    std::ifstream words("./datasets/words.txt");
    if (!words.is_open()) {
        std::cerr << "Error al abrir archivo para lectura (4.1): " << "words.txt"
            << std::endl;
        std::exit(1);
    }
    std::ifstream random("./datasets/random.txt");
    if (!random.is_open()) {
        std::cerr << "Error al abrir archivo para lectura (4.1): " << "random.txt"
            << std::endl;
        std::exit(1);
    }
    std::ifstream randomD("./datasets/random_with_distribution.txt");
    if (!randomD.is_open()) {
        std::cerr << "Error al abrir archivo para lectura (4.1): " << "random_with_distribution.txt"
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
    //-------------------------------------------------------------------------
    ofstream autocompletado("./resultados/resultado_autocompletado.csv");
    if (!autocompletado) {
        cerr << "Error al abrir el archivo de resultados de autocompletado" << endl;
        return 1;
    }
    autocompletado << "i,porcentaje,variante"<< endl;


    Trie* ultimo_f = nullptr;// variante frcuencia
    Trie* ultimo_r = nullptr;// variante reciente
    std::string palabra;
    int c_pal= 0;

    for(int i = 0; i <= 18; i++){
        Trie* trie = new Trie(true); // Hay que crearlo desde 0 para poder reiniciar los nodos
        string line;
        int N = pow(2, i);
        
        if(i==18){ // medimos tiempo
            // Guardamos el último trie para trabajar con el después
            int group = N/16;
            int group_counter = 0;
            int iter = 1;

            auto start = high_resolution_clock::now();
            for(int j = 1; j <= N; j++){
                getline(words, line);
                trie->insert(line);
                group_counter++;

                if(group_counter == group){
                    auto end = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(end - start);
                    tiempo<<iter<<","<<duration.count()<<","<<trie->total_caracters<<endl;
                    iter++;
                    auto start = high_resolution_clock::now();// Se parte denuevo a contar tiempo
                }
            }

            ultimo_f = TrieUtils::copyTrie(trie);
            memoria <<i<<","<<trie->nodes_count<<","<<trie->total_caracters<< endl;
            continue;
        }

        for(int j = 1; j <= N; j++){
                getline(words, line);
                trie->insert(line);
            }

        memoria <<i<<","<<trie->nodes_count<<","<<trie->total_caracters<< endl;
        words.seekg(0); // se vuelve al inicio
    }
    //4.2 Tiempo
    
    //4.3 Análisis de autocompletado
    int total_char = 0;// Cantidad de caracteres que debería haber escrito el usuario
    int real_char = 0;// Cantidad de caracteres reales que escribe el usuario gracias al autocompletado
    TrieNode* root = ultimo_f->nodes.at(0);

    for(int i = 0; i<= 14; i++){
        while (wiki >> palabra && log2(c_pal) <= i) { // lee palabra por palabra
            c_pal++;
            total_char += palabra.size();

            int descend_count = 0;// cuantas veces se ha descendido
            TrieNode* new_node = root;
            for(char c : palabra){
                if(c == '$'){
                    //autocompletado no funciono
                    real_char +=palabra.size();
                    ultimo_f->update_priority(new_node);
                    break;
                    //caera en el mismo caso que abajo?
                }

                new_node = ultimo_f->descend(new_node, c);
                descend_count++;
                //cout << "ene 1" << endl;
                if(new_node == nullptr){
                    //cout << "ene 2" << endl;
                    //cout << "newnode es nulo para "<< c << endl;
                    real_char +=palabra.size();
                    break;
                }else{
                    TrieNode* terminal =  ultimo_f->autocomplete(new_node);
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
                        ultimo_f->update_priority(terminal);
                        break;
                    }
                    //cout << "ene" << endl;
                }
            }
            descend_count = 0;
            //std::cout <<"Para esta iteracion se deberian haber escrito: "<< total_char
            //<< " caracteres, pero gracias al autocompletado, realmente se han escrito: " << real_char << endl;
            //cout<< "cantidad de palabras escritas: 2^"<< log2(c_pal) <<endl;
            
            wiki.clear();
            wiki.seekg(0);
        }

        autocompletado << i <<","<< float(real_char)/total_char <<","<< "Frecuencia" << endl;
    }
    wiki.close();

    cout << ultimo_f->nodes.at(45)->str << endl;
    cout << ultimo_r->nodes.at(0)->str << endl;

    return 0;
};
