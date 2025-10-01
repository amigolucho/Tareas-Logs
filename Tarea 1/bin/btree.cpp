#include "../include/btree.hpp"
#include <vector>
#include <utility>

//implementación del árbol B
BTree::BTree(const std::string &filename){
    this->filename = filename;
}

BTreeNode BTree::readNode(int offset) const {
    BTreeNode node;
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Error al abrir archivo para lectura" << filename << std::endl;
        std::exit(1);
    }

    std::streampos file_offset = offset * sizeof(BTreeNode);
    in.seekg(file_offset);
    in.read(reinterpret_cast<char *>(&node), sizeof(BTreeNode));

    if(!in){
        std::cerr << "Error al leer el nodo en la posición " << offset << std::endl;
        std::exit(1);  
    }

    return node;
}

std::pair<std::pair<BTreeNode,BTreeNode>, std::pair<int,float>> BTree::split(BTreeNode node) const {

    int par_mediano = b/2;

    BTreeNode hijo_izq;
    BTreeNode hijo_der;

    hijo_izq.k = par_mediano - 1;
    hijo_der.k = par_mediano;
  
    for (int i=0; i < par_mediano; i++) {
        hijo_izq.llaves_valores[i] = node.llaves_valores[i];
        hijo_der.llaves_valores[i] = node.llaves_valores[par_mediano + i];

    }

    if(node.es_interno) {
        for(int i=0; i < par_mediano; i++) {
            hijo_izq.hijos[i] = node.hijos[i];
            hijo_der.hijos[i] = node.hijos[par_mediano + i];
        }
        hijo_der.hijos[par_mediano] = node.hijos[b]; // falta agregar el último hijo
    }
    return std::pair {std::pair {hijo_izq, hijo_der}, node.llaves_valores[par_mediano]};
}

void BTree::insert(std::pair<int,float> par, BTreeNode node) const {
    
    BTreeNode U;
    
    if (node.es_interno){
        for(int i=0; i < node.k; i++){
            if (node.llaves_valores[i-1].second < par.second && par.second <= node.llaves_valores[i].second){
                U = readNode(node.hijos[i]);
            }else{
                U = readNode(node.hijos[i+1]);
            }
        }
        if (U.k == b){// Si está lleno
            BTreeNode U_i;
            BTreeNode U_d;
            int k;
            float v;

            std::pair{std::pair{U_i, U_d}, std::pair{k,v}} = split(U);

        }else{ // Si le queda espacio, se inserta ahí
            insert(par, U);
        }
    }else{

    }
}

bool debeBuscarEnHijo(BTreeNode& nodo, int hijo_index, int l, int u) {
    if (hijo_index == 0) {
        return l <= nodo.llaves_valores[0].first;
    } else if (hijo_index == nodo.k) {
        return u >= nodo.llaves_valores[nodo.k-1].first;
    } else {
        return u >= nodo.llaves_valores[hijo_index-1].first && 
               l <= nodo.llaves_valores[hijo_index].first;
    }
}

std::vector<std::pair<int, float>> BTree::buscarRango(int l, int u) {
    if (es_mas) {
        return buscarRangoBmas(l,u);
    } else {

        // La raíz siempre está en la posición 0
        return buscarRangoB(l,u,0); 
    }
} 

std::vector<std::pair<int, float>> BTree::buscarRangoB(int l, int u, int nodo_index) {
    BTreeNode nodo = readNode(nodo_index);
    std::vector<std::pair<int, float>> resultados;

    if (nodo.es_interno) {

        //buscar en los pares del nodo interno
        for (int i = 0; i < nodo.k; i++) {
            int clave = nodo.llaves_valores[i].first;
            if (clave >= l && clave <= u) {
                resultados.push_back(nodo.llaves_valores[i]);
            }
        }
        
        //Buscar recursivamente en hijos que podrían contener elementos en el rango
        for (int i = 0; i <= nodo.k; i++) {
            if (debeBuscarEnHijo(nodo, i, l, u)) {
                std::vector<std::pair<int, float>> resultados_hijo = buscarRangoB(l, u, nodo.hijos[i]);
                resultados.insert(resultados.end(), resultados_hijo.begin(), resultados_hijo.end());
            }
        }
    } else {

        //buscar en los pares de este nodo
        for (int i = 0; i < nodo.k; i++) {
            int clave = nodo.llaves_valores[i].first;
            if (clave >= l && clave <= u) {
                resultados.push_back(nodo.llaves_valores[i]);
            }
        }
    }
    return resultados;
}

std::vector<std::pair<int, float>> BTree::buscarRangoBmas(int l, int u) {
    std::vector<std::pair<int, float>> resultados;
    
    //Encontrar la hoja donde debería comenzar la búsqueda
    int nodo_actual = 0;
    BTreeNode nodo = readNode(nodo_actual);
    

    while (nodo.es_interno) {
        int i = 0;
        // Encontrar el primer hijo cuya llave sea mayor o igual a l
        while (i < nodo.k && l > nodo.llaves_valores[i].first) {
            i++;
        }
        nodo_actual = nodo.hijos[i];
        nodo = readNode(nodo_actual);
    }
    
    //Recorrer las hojas
    bool continuar = true;
    while (continuar && nodo_actual != -1) {
        nodo = readNode(nodo_actual);
        
        // Buscar en todos los pares de la hoja actual
        for (int i = 0; i < nodo.k; i++) {
            int clave = nodo.llaves_valores[i].first;
            if (clave > u) {

                // Si encontramos una clave mayor a u, podemos terminar
                continuar = false;
                break;
            }
            if (clave >= l && clave <= u) {
                resultados.push_back(nodo.llaves_valores[i]);
            }
        }
        // Ir a la siguiente hoja
        nodo_actual = nodo.siguiente;
    }
    return resultados;
}