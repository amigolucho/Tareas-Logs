#include "bin/trie.cpp"
#include <math.h>


int main(){
    /*vector<string> palabras = {"hola", "mundo", "retumbar", "hola", "felipe"};
    for(string s : palabras){
        Trie.insert(s);
    }
    cout << "Se insertaron con exito las palabras. Deberia tener 27 nodos: nodos finales son " 
         << Trie.nodes_count << endl;
    
    cout << "Testeamos descender" << endl;
    TrieNode* node2 = Trie.nodes.at(8);
    node2 = Trie.descend(node2, 'd');
    if(node2==nullptr){
        cout << "No existe este descenso" << endl;
    }else {
        cout << "Se descendio correctamente al nodo con prefijo " << node2->prefix << endl;
    }*/

    std::ifstream in("./datasets/words.txt");
    if (!in.is_open()) {
    std::cerr << "Error al abrir archivo para lectura: " << "words.txt"
              << std::endl;
    std::exit(1);
    }

    // Escribimos nuestros resultados
    ofstream archivo("./resultado.txt");
    if (!archivo) {
        cerr << "Error al abrir el archivo de resultados" << endl;
        return 1;
    }

    for(int i = 0; i <= 18; i++){
        Trie Trie(true); // Hay que crearlo desde 0 para poder reiniciar los nodos
        string line;
        int N = pow(2, i);

        for(int j = 1; j <= N; j++){
            getline(in, line);
            //cout << line << endl;
            Trie.insert(line);
        }

        archivo << "Inserción para i = 2^"<< i << ". La cantidad de nodos es: " << Trie.nodes_count << endl;
        in.seekg(0); // se vuelve al inicio
    }

    return 0;
};