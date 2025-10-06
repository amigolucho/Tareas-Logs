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
            insert(par, U);//hay que insertar en la lista llave valor
        }
    }else { // Si es la raíz
        if(node.k < b){ // no está llena
            //insertar par en R
        }else{ //está llena
            BTreeNode R_i;
            BTreeNode R_d;

            int k;
            float v;
            std::pair{std::pair{R_i, R_d}, std::pair{k,v}} = split(node);

            // escribir en arreglo
            BTreeNode NewR; //como me aseguro que está vacía?
            //insert par en NewR
            //NewR.hijos[0] = R_i
            //NewR.hijos[1] = R_d
            //NewR.k = 2
            // escrbir R
            // insertar en donde dependa
        }

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