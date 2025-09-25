#include "../include/btree.hpp"

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

std::vector<std::pair<int, float>> BTree::buscarRango(int l, int u) {
    if (es_mas) {
        return buscarRangoBmas(l,u);
    } else {
        return buscarRangoB(l,u,0); // ojoo en la implementacion hay que garantizar que la raiz siempre este en 0 
    }
} 

