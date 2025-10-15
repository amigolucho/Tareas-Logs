#ifndef TRIE_HPP
#define TRIE_HPP
#include <fstream>
#include <iostream>

#include <array>
#include <list>
#include <string>
#include <vector>
#include <utility>
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

    char value;

    TrieNode* parent;
    std::array<TrieNode*, 27> next;
    int priority;
    string* str;
    TrieNode* best_terminal;
    int best_priority;
};


/**
 * @brief Clase que representa un trie
 */
class Trie {

    std::vector<TrieNode> nodes;

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
    TrieNode* autocomplete(Trie v);

    /**
     * @brief  actualiza la prioridad del nodo terminal 𝑣 según la variante y actualiza los nodos en el camino a la raíz.
     * 
     * @param v nodo terminal que se actualizará
     */
    void update_priority(TrieNode* v);



};
#endif