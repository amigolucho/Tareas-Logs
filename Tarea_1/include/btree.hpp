#ifndef B_TREE_HPP
#define B_TREE_HPP
#include <fstream>
#include <iostream>

#include <array>
#include <list>
#include <string>
#include <vector>
#include <utility>
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
    std::array<std::pair<int, float>, 340> llaves_valores;
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
        std::vector<BTreeNode> nodos;
        int escrituras = 0;
        int lecturas = 0;
        bool es_mas; // true para B+Tree, false para B-Tree

        /**
         * @brief Contruye un Btree apartir de un archivo binario
         * @param fielname Nombre del archivo que contiene el Btree
         */
        BTree(const std::string &filename);

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
         * @brief Inserta un par llave-valor en un nodo del árbol B
         * 
         * @param par Par a insertar en el nodo
         * @param node nodo donde se está insertando
         * @param indice índice del nodo en el arreglo que representa el árbol en memoria RAM
         */
        void insert(std::pair<int,float> par, BTreeNode& node, int indice);

        /**
         * @brief Realiza busqueda por rango en el arbol
         * @param l Limite inferior del rango (inclusive)
         * @param u Limite superior del rango (inclusive)
         * @return Vector con todos los pares (clave, valor) cuyo clave esta en [l, u]
         * 
         * Para B-Tree: basqueda recursiva en todos los nodos relevantes
         * Para B+Tree: busqueda secuencial eficiente usando punteros entre hojas
         */
        std::vector<std::pair<int, float>> buscarRango(int l, int u);


        /**
         * @brief Busqueda por rango especifica para B+Tree
         * @param l Limite inferior
         * @param u Limite superior
         * @return Vector de pares encontrados
         */
        std::vector<std::pair<int, float>> buscarRangoBmas(int l, int u);
        
        /**
         * @brief Buqueda por rango especifica para B-Tree 
         * @param l Limite inferior
         * @param u Limite superior
         * @param nodo_index Indice del nodo donde comenzar la busqueda
         * @return Vector con pares encontrados
         */
        std::vector<std::pair<int, float>> buscarRangoB(int l, int u, int nodo_index);
        

    private:
        std::string filename;
};

namespace TreeUtils {
    /**
     * @brief Crea una raíz
     */
    BTreeNode crear_raiz();

    /**
     * @brief Función que agrega un par llave-valor a la lista de pares llave-valor de un nodo
     * 
     * @param par par a insertar
     * @param node nodo que tiene la lista donde se inserta el par
     * @return devuelve la lista ya modificada
     */
    void agregar_par(const std::pair<int, float> par, BTreeNode& node);
    
    /**
     * @brief Escribe un nodo en disco
     * 
     * @param filename Nombre del archivo donde se escribe
     * @param node Nodo a escribir
     * @param indice indice del nodo
     */
    void write_node(const std::string &filename, const BTreeNode node, int indice);

    BTreeNode readNode(std::string &filename, int offset);
};

#endif