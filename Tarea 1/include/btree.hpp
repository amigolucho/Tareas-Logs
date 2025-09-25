#ifndef B_TREE_HPP
#define B_TREE_HPP


#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>
using namespace std;

const int b = 340;

/**
 * @brief Estructura que representa un B Tree en disco
 * 
 * - es_interno: Indicador si es un nodo intero o externo
 * - k: Cantidad de pares llave-valor actualmente contenidas
 * - llaves_valores: Arreglo tamaño b para pares llave-valor. Los pares estan ordenados de manera ascendente
 * - hijos: Arreglo de b+1 enteros que guarda las posiciones en disco de los hijos del nodo
 * - siguiente: Entero solo en hojas de los árboles B+ que indica la posición en disco de la siguiente hoja
 */
struct BTreeNode {
    int es_interno; // 1 si es interno, 0 si es externo
    int k;
    std::pair<int, float> llaves_valores[340];
    int hijos[341];
    int siguiente;// valor inicial, en los B+ se cambia a su valor real 
};

/**
 * @brief Clase que representa un B tree en disco
 * 
 * Permite navegar por un B tree guardado en disco
 */
class BTree {
    public:
        /**
         * @brief Contruye un Btree apartir de un archivo binario
         * @param fielname Nombre del archivo que contiene el Btree
         */
        BTree(const std::string &filename);

        BTreeNode readNode(int offset) const;

        /**
         * @brief Realiza el split para un nodo de un árbol B
         * 
         * Lee los bytes de la posición dada que corresponden a un nodo lleno (con b pares y b+1 hijos)
         * y los deserializa para contruir el BTree node en memoria. ??????
         * Es responsabilidad del usuario asegurarse de que el offset sea válido,
         * es decir, que exista un nodo en esa posición.
         * 
         * @param node Nodo a splitear
         * @return Retorna los 2 nuevos nodos y el par mediano
         * @throws Termina el programa si no puede abrir el archivo o leer los bytes
         * del nodo
         */
        std::pair<std::pair<BTreeNode,BTreeNode>, std::pair<int,float>> split(BTreeNode node) const;  

        /**
         * @brief Inserta un par llave-valor en un árbol B
         * 
         * @param par Par a insertar en el árbol
         */
        void insert(std::pair<int,float> par, BTreeNode node) const;

        /**
         * @brief Funcion recursiva para busqueda en B-tree
         * @param l Limite inferior del rango
         * @param u limite superior del rango
         * @param nodo_index Indice del nodo actual
         * @return Vector con todos los pares encontrados 
         */

         std::vector<std::pair<int, float>> buscarRango(int l, int u, int nodo_index);
    
    private:
        std::string filename;
};

//namespace TreeUtils {

//};

#endif

