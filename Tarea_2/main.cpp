#include "bin/trie.cpp"
#include <math.h>
#include <chrono>
#include <iostream>
#include <fstream>

void run_autocomplete_simulation(Trie* trie, std::ifstream& input_file, const std::string& dataset_name, std::ofstream& output_file) {
    TrieNode* root = trie->nodes.at(0);
    std::string palabra;
    std::string variante_label = trie->es_frecuencia ? "Frecuencia" : "Reciente";

    for(int i = 0; i <= 21; i++){ 
        long N_palabras_medir = (long)pow(2, i);
        
        input_file.clear(); 
        input_file.seekg(0); 
        
        long c_pal = 0;      
        long total_char = 0; 
        long real_char = 0;  

        while (c_pal < N_palabras_medir && input_file >> palabra) {
            c_pal++;
            total_char += palabra.size(); 
            
            int descend_count = 0; 
            TrieNode* current_node = root;
            bool autocompletado_exitoso = false;
            
            for(char c : palabra){
                TrieNode* next_node = trie->descend(current_node, c); 
                
                if(next_node == nullptr){
                    real_char += (palabra.size() - descend_count) + descend_count; 
                    autocompletado_exitoso = true;
                    break;
                }
                
                TrieNode* terminal = trie->autocomplete(next_node);
                
                if(terminal != nullptr && *terminal->str == palabra + '$'){
                    real_char += descend_count;
                    trie->update_priority(terminal); 
                    autocompletado_exitoso = true;
                    break;
                }
                
                current_node = next_node; 
            }
            
            if (!autocompletado_exitoso) {
                real_char += palabra.size();
                
                if (current_node != nullptr) {
                    TrieNode* terminal_node = trie->descend(current_node, '$');
                    if (terminal_node != nullptr) {
                         trie->update_priority(terminal_node);
                    }
                }
            }
        }
        
        if(c_pal == N_palabras_medir && total_char > 0) {
            float porcentaje_escrito = (float)real_char / total_char;
            output_file << i << "," << porcentaje_escrito << "," << dataset_name << "_" << variante_label << std::endl;
        }
    }
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

    ofstream autocompletado("./resultados/resultado_autocompletado.csv");
    if (!autocompletado) { cerr << "Error al abrir el archivo de resultados de autocompletado" << endl; return 1; }
    autocompletado << "i,porcentaje,variante"<< endl;


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
                group_counter++;
                //cout<< group <<endl;
                //cout<< group_counter <<endl;

                if(group_counter == group){
                    auto end = high_resolution_clock::now();
                    auto duration = duration_cast<nanoseconds>(end - start);

                    tiempo<<iter<<","<<duration.count()<<","<<trie->total_caracters<<endl;
                    iter++;

                    group_counter = 0;
                }
            }

            // 3.1. Guardar la versión Frecuencia
            ultimo_f = trie;
            memoria << i << "," << trie->nodes_count << "," << trie->total_caracters << endl;
            
            words.clear(); 
            words.seekg(0); 
            
            Trie* trie_reciente = new Trie(false); 
            string line_reciente;
            
            for(int j = 1; j <= N_MAX; j++){
                getline(words, line_reciente);
                trie_reciente->insert(line_reciente);
            }
            
            memoria << i << "," << trie->nodes_count << "," << trie->total_caracters << endl;
            words.seekg(0); 
            delete trie;
        }

        for(int j = 1; j <= N; j++){
            getline(words, line);
            trie->insert(line);
        }

        memoria << i << "," << trie->nodes_count << "," << trie->total_caracters << endl;
        words.seekg(0); 
        delete trie; 
    }
    
    std::ifstream words_autocomp("./datasets/words.txt");
    if (!words_autocomp.is_open()) { 
        std::cerr << "Error al abrir words.txt para la simulacion 4.3." << std::endl;
        return 1; 
    }

    //words.txt
    run_autocomplete_simulation(ultimo_f, words_autocomp, "words", autocompletado);
    run_autocomplete_simulation(ultimo_r, words_autocomp, "words", autocompletado);

    //wikipedia.txt
    run_autocomplete_simulation(ultimo_f, wiki, "wikipedia", autocompletado);
    run_autocomplete_simulation(ultimo_r, wiki, "wikipedia", autocompletado);

    //random.txt
    run_autocomplete_simulation(ultimo_f, random, "random", autocompletado);
    run_autocomplete_simulation(ultimo_r, random, "random", autocompletado);

    //random_with_distribution.txt
    run_autocomplete_simulation(ultimo_f, randomD, "randomD", autocompletado);
    run_autocomplete_simulation(ultimo_r, randomD, "randomD", autocompletado);

    words_autocomp.close();
    wiki.close();
    words.close(); 
    random.close();
    randomD.close();
    memoria.close();
    tiempo.close();
    autocompletado.close();

    cout << ultimo_f->nodes.at(45)->str << endl;
    cout << ultimo_r->nodes.at(0)->str << endl;
    delete ultimo_f;
    delete ultimo_r;
    return 0;
}