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


/**
 * @brief Esctructura que representa un Trie
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

    std::optional<TrieNode*> parent;
    std::array<std::optional<TrieNode*>, 27> next;
    int priority;
    string* str;
    TrieNode* best_terminal;
    int best_priority;
};


/**
 * @brief Clase que representa un trie
 */
class Trie {
    public:
        std::vector<std::optional<TrieNode*>> nodes;// size cuenta como contador??x|

        int timestamp = 0;

            const string Sigma = "abcdefghijklmnopqrstuvwxyz$"; //en particular un string constante es un arreglo de chars (poner e n el readme)
            string prefix;

            /**
             * @brief Constructor de un Trie
             */
            Trie();

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

    /**
     * @brief crea un nodo a partir de un caracter
     * 
     * @param w caracter del nodo
     */
    TrieNode create_node(char w);
}