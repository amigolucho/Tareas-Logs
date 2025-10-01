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
    
    std::pair<int,float> new_value = par; //valor mutable, inicia siendo el par
    std::pair<int,float> old_value;
    int value = par.first;
    
    BTreeNode U;
    
    if(!node.es_interno){// es una hoja y no raíz
        for(int i=0; i < node.k; i++){
            if (value < node.llaves_valores[i].second){
                old_value = node.llaves_valores[i];
                int value = old_value.first;
                node.llaves_valores[i] = new_value;
                new_value = old_value;
            }
        }
        node.llaves_valores[node.k] = new_value; //por la forma en que está definida, siempre hará falta reemplazar el último
        node.k = node.k + 1; //actualizamos k
        //falta escribir en filename
    }else if (node.es_interno){ //es interno
        int pos_U;
        for(int i=0; i < node.k; i++){
            if (value <= node.llaves_valores[i].first){ // no es necesario comparar si es mayor a los de i-1, pues si
                U = readNode(node.hijos[i]);            // salta de la iteración, es por que es mayor
                pos_U = i; // guardamos la posición
                break;
            }
        }
        if (U.k == b){// Si está lleno
            BTreeNode U_i;
            BTreeNode U_d;
            int k;
            float v;

            std::pair{std::pair{U_i, U_d}, std::pair{k,v}} = split(U);

            // insertar par llave valor, sale mejor hacer una func auxiliar
            for(int i=0; i < node.k; i++){
                if (value < node.llaves_valores[i].second){
                    old_value = node.llaves_valores[i];
                    int value = old_value.first;
                    node.llaves_valores[i] = new_value;
                    new_value = old_value;
                }
            }
            node.llaves_valores[node.k] = new_value; //por la forma en que está definida, siempre hará falta reemplazar el último

            // escribir
            //node.hijos[node.k] = U_r    como saco la info de U_i
            //node.hijos[pos_u] = U_i  

            node.k = node.k + 1; // actualizamos la cantidad de elementos de V

            //reescribir V

            // insertar el par en el hijo donde dependa
            // insert(par, U)


        }else{ // Si le queda espacio, se inserta ahí
            insert(par, U);
        }
    }else { // Si es la raíz
        if(node.k < b){ // no está llena
        }

    }
}

std::vector<std::pair<int, float>> BTree::buscarRango(int l, int u) {
    if (es_mas) {
        return buscarRangoBmas(l,u);
    } else {
        return buscarRangoB(l,u,0); // ojoo en la implementacion hay que garantizar que la raiz siempre este en 0 
    }
} 

