#include "bin/trie.cpp"
#include <math.h>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std::chrono;

/**
 * @brief Realiza la experimentación del autocompletado para un dataset y variante específica
 */
void run_autocomplete_simulation(Trie* trie, std::ifstream& input_file, const std::string& dataset_name, std::ofstream& output_file, std::ofstream& time_file) {
    auto start = high_resolution_clock::now();
    TrieNode* root = trie->nodes.at(0);
    std::string palabra;
    std::string variante_label = trie->es_frecuencia ? "Frecuencia" : "Reciente";
    int char_count = 0;
    int c_pal;

    for(int i = 0; i <= 22; i++){ 
        int N_palabras_medir = (long)pow(2, i);
        
        input_file.clear(); 
        input_file.seekg(0); 
        
        c_pal = 0;      
        long total_char = 0;// Cantidad total de caracteres que el usuario debería haber escrito 
        long real_char = 0;// Cantidad real de caracteres que el usuario escribe

        while (c_pal <= N_palabras_medir && input_file >> palabra) {
            c_pal++;
            
            int descend_count = 0; 
            TrieNode* current_node = root;
            
            total_char += palabra.size(); 

            palabra += '$';
            //cout<<palabra<<endl;
            for(char c : palabra){
                //cout<<"letra "<<c<<endl;
                TrieNode* next_node = trie->descend(current_node, c); 
                descend_count++;

                
                if(next_node == nullptr){
                    real_char += palabra.size(); 
                    break;
                }

                if(c == '$'){
                    // Autocompletado no funcionó
                    real_char += palabra.size() - 1;// menos el $
                    trie->update_priority(next_node);
                    break;
                }
                TrieNode* terminal = trie->autocomplete(next_node);
                bool a = terminal == nullptr;
                if(a){
                    //Se visita por primera vez, pero no es terminal, el caso terminal se encargará de cambiar esto
                    current_node = next_node; 
                    continue;
                }

                if(*terminal->str == palabra){
                    real_char += descend_count;
                    trie->update_priority(terminal); 
                    break;
                }

                current_node = next_node; 
            }
        
        }

        float porcentaje_escrito = (float)real_char / total_char;
        output_file << i << "," << porcentaje_escrito << "," << dataset_name << "," << variante_label << std::endl;
        char_count=total_char;
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start);
    time_file<< dataset_name <<","<<duration.count()<<","<<char_count<<","<<c_pal<<","<<variante_label<<endl;
}


int main(){
    using namespace std::chrono;

    std::ifstream wiki("./datasets/wikipedia.txt");
    if (!wiki.is_open()) {
        std::cerr << "Error al abrir el archivo wikipedia.txt\n";
        return 1;
    }
    std::ifstream words("./datasets/words.txt");
    if (!words.is_open()) {
        std::cerr << "Error al abrir archivo para lectura (4.1): words.txt" << std::endl;
        std::exit(1);
    }
    std::ifstream random("./datasets/random.txt");
    if (!random.is_open()) {
        std::cerr << "Error al abrir archivo para lectura (4.1): random.txt" << std::endl;
        std::exit(1);
    }
    std::ifstream randomD("./datasets/random_with_distribution.txt");
    if (!randomD.is_open()) {
        std::cerr << "Error al abrir archivo para lectura (4.1): random_with_distribution.txt" << std::endl;
        std::exit(1);
    }
    
    ofstream memoria("./resultados/resultado_memoria.csv");
    if (!memoria) { cerr << "Error al abrir el archivo de resultados de memoria" << endl; return 1; }
    memoria << "i,nodes,characters"<< endl;

    ofstream tiempo("./resultados/resultado_tiempo.csv");
    if (!tiempo) { cerr << "Error al abrir el archivo de resultados de tiempo" << endl; return 1; }
    tiempo << "N/M,t,characters"<< endl;

    ofstream tiempo2("./resultados/resultados_tiempo2.csv");
    if (!tiempo2) { cerr << "Error al abrir el archivo de resultados de tiempo" << endl; return 1; }
    tiempo2 << "Dataset,Tiempo de ejecución,Cantidad de caracteres,Cantidad de palabras,Variante"<< endl;

    ofstream autocompletado("./resultados/resultado_autocompletado.csv");
    if (!autocompletado) { cerr << "Error al abrir el archivo de resultados de autocompletado" << endl; return 1; }
    autocompletado << "i,porcentaje,dataset,variante"<< endl;


    Trie* ultimo_f = nullptr;// variante frcuencia
    Trie* ultimo_r = nullptr;// variante reciente
    
    for(int i = 0; i <= 18; i++){
        Trie* trie = new Trie(true); 
        string line;
        int N = (int)pow(2, i);
        
        if(i == 18){ 
            int N_MAX = N; // N = 2^18
            int group = N_MAX / 16;
            int group_counter = 0;
            int iter = 1;

            for(int j = 1; j <= N; j++){
                getline(words, line);
                
                auto start = high_resolution_clock::now();
                trie->insert(line);

                
                if(line=="k"){
                    
                    
                }
                group_counter++;

                if(group_counter == group){
                    auto end = high_resolution_clock::now();
                    auto duration = duration_cast<nanoseconds>(end - start);

                    tiempo<<iter<<","<<duration.count()<<","<<trie->total_caracters<<endl;
                    iter++;

                    group_counter = 0;
                }
            }

            // 3.1. Guardar la versión Frecuencia
            ultimo_f = TrieUtils::copyTrie(trie);
            ultimo_r = TrieUtils::copyTrie(trie);
            ultimo_r->es_frecuencia = false;
            memoria << i << "," << trie->nodes_count << "," << trie->total_caracters << endl;
            
            words.clear(); 
            words.seekg(0);  
            
            continue;
        }

        for(int j = 1; j <= N; j++){
            getline(words, line);
            trie->insert(line);
        }

        memoria << i << "," << trie->nodes_count << "," << trie->total_caracters << endl;
        words.seekg(0); 
    }
    
    //wikipedia.txt
    run_autocomplete_simulation(ultimo_f, wiki, "wikipedia", autocompletado, tiempo2);
    run_autocomplete_simulation(ultimo_r, wiki, "wikipedia", autocompletado, tiempo2);

    //random.txt
    run_autocomplete_simulation(ultimo_f, random, "random", autocompletado, tiempo2);
    run_autocomplete_simulation(ultimo_r, random, "random", autocompletado, tiempo2);

    //random_with_distribution.txt
    run_autocomplete_simulation(ultimo_f, randomD, "randomD", autocompletado, tiempo2);
    run_autocomplete_simulation(ultimo_r, randomD, "randomD", autocompletado, tiempo2);


    wiki.close();
    words.close(); 
    random.close();
    randomD.close();
    memoria.close();
    tiempo.close();
    tiempo2.close();
    autocompletado.close();

    cout << "Se ejecuto la tarea con exito" << endl;

    return 0;
}