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

     int Sigma = 27;

    char next[27];

};


/**
 * @brief Clase que representa un trie
 */
class Trie {


    /**
     * @brief inserta una palabra al trie
     * 
     * De ser necesario crea nodos
     * 
     * @param w palabra a insertar
     */
    Trie insert(string w){

    }
};
#endif