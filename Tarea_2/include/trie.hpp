#ifndef TRIE_HPP
#define TRIE_HPP
#include <fstream>
#include <iostream>

#include <array>
#include <list>
#include <string>
#include <vector>
#include <utility>
#include <optional>
using namespace std; 

const string sigma = "abcdefghijklmnopqrstuvwxyz$";
const int sigma_size = 27;

/**
 * @brief Esctructura que representa un Trie
 * 
 * - key: caracter de este nodo
 * - prefix: prefijo formado hasta este nodo
 * 
 * - parent: una referencia a su nodo padre (nulo si es la raíz)
 * - next: estructura que mapea caracteres Sigma a hijos
 * - priority: dependiendo del criterio a utilizar, es el tiempo de acceso o la cantidad de accesos a este nodo terminal
 * - str: si el nodo es terminal esto debe contener un puntero al string asociado; no es estrictamentenecesario en implementaciones
 *  de trie, pero facilita la experimentación
 * - best_terminal: un puntero al nodo terminal del subárbol con mayor prioridad
 * - best_priority: la prioridad del nodo con mayor prioridad del subárbol
 */
struct TrieNode {
    char key;
    string prefix;

    TrieNode* parent;
    array<TrieNode*, sigma_size> next;

    int priority;
    string* str;
    TrieNode* best_terminal;
    int best_priority;

    /** 
     * @brief Constructor de un nodo de un trie
     */
    TrieNode() {
        key = ' ';
        prefix = "";
        parent = nullptr;
        next.fill(nullptr);  // Todos los hijos en nullptr
        priority = 0;
        str = nullptr;       // No es terminal por defecto
        best_terminal = nullptr;
        best_priority = 0;
    }
};


/**
 * @brief Clase que representa un trie
 * 
 * - es_frecuencia: indica si el autocompletado es de frecuencia (true), o de de reciente (false)
 * - timestamp: entero usado para actualizar la prioridad en el caso de autocompletado reciente
 * - Sigma: caracteres válidos
 * - nodes: lista de los nodos del trie (sus punteros)
 * - nodes_count: cuenta cuantos nodos hay en el trie
 * - total_caracteres: Cantidad total de caracteres insertados.
 */
class Trie {
    private:
        const string Sigma = "abcdefghijklmnopqrstuvwxyz$"; //en particular un string constante es un arreglo de chars (poner e n el readme)

    public:
        int timestamp = 0;
        bool es_frecuencia;// indica si el autocompletado es de frecuencia (true), o de de reciente (false)
        std::vector<TrieNode* > nodes;// size cuenta como contador??x|

        int nodes_count;// Cuenta cuantos nodos hay en el trie
        int total_caracters;// Cantidad total de caracteres insertados.
        int contador_palabras;// Cantidad de palabras insertadas
        /**
         * @brief Constructor de un Trie
         */
        Trie(bool variante) {
            TrieNode* root = new TrieNode();
            root->key = ' ';  // La raíz no tiene carácter asociado
            root->prefix = "";
            
            nodes_count = 1;
            total_caracters = 0;
            contador_palabras = 0;

            nodes.push_back(root);
            timestamp = 0;
            es_frecuencia = variante;
        }
        ~Trie() {
            for (TrieNode* node : nodes) {
                if (node != nullptr) {
                    if (node->str != nullptr) {
                        delete node->str;
                    }
                    delete node;
                }
            }
            nodes.clear();
        }
        
        
        /**
         * @brief inserta una palabra al trie
         * 
         * De ser necesario crea nodos
         * 
         * @param w palabra a insertar
         */
        void insert(string w);
        /**
         * @brief Retorna un puntero al nodo asociado al bajar por el caracter c, o nulo en caso de no existir
         * 
         * @param v puntero al nodo desde donde se dedsciende
         * @param c caracter al cual buscar
         */
        TrieNode* descend(TrieNode* v,char c);
        /**
         * @brief Retorna el puntero al nodo terminal qu representa mejor autocompletado del subárbol v
         * 
         * @param v Subárbol a autocompletar
         */
        TrieNode* autocomplete(TrieNode* v);
        /**
         * @brief  actualiza la prioridad del nodo terminal 𝑣 según la variante y actualiza los nodos en el camino a la raíz.
         * 
         * @param v nodo terminal que se actualizará
         */
        void update_priority(TrieNode* v);

};
#endif

namespace TrieUtils {

    /**
     * @brief actualiza la información del nodo del trie de manera iterativa hasta la raíz o que se cumpla la invariante
     * 
     * @param nodo a actualizar en esta iteración
     */
    void update_info(TrieNode* v);

    Trie* copyTrie(Trie* trie);
}