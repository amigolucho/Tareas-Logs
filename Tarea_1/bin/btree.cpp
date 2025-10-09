#include "../include/btree.hpp"
#include <vector>
#include <iostream>
#include <utility>

//implementación del árbol B
BTree::BTree(const std::string &filename){
    this->filename = filename;
    this->nodos.resize(1);
}

std::pair<std::pair<BTreeNode,BTreeNode>, std::pair<int,float>> BTree::split(BTreeNode &node) const {

    int par_mediano = b/2 - 1;

    BTreeNode hijo_izq;
    BTreeNode hijo_der;

    hijo_izq.k = par_mediano;
    hijo_der.k = par_mediano + 1;
  
    for (int i=0; i < par_mediano; i++) {
        hijo_izq.llaves_valores[i] = node.llaves_valores[i];
        hijo_der.llaves_valores[i] = node.llaves_valores[par_mediano + i + 1];
    }
    // Por la forma del for, hay que agregar un ultimo elemento al nodo derecho
    hijo_der.llaves_valores[par_mediano] = node.llaves_valores[b - 1];

    if(node.es_interno) {
        std::cout << "adada" << std::endl;
        for(int i=0; i <= par_mediano; i++) {
            hijo_izq.hijos[i] = node.hijos[i];
            hijo_der.hijos[i] = node.hijos[par_mediano + i + 1];
        }
        hijo_der.hijos[par_mediano] = node.hijos[b]; // falta agregar el último hijo

        hijo_izq.es_interno = 1;
        hijo_der.es_interno = 1;
    }else{
        hijo_izq.es_interno = 0;
        hijo_der.es_interno = 0;// Guardan el mismo tipo del que se separan
    }
    //std::cout << "El split deja nodos con " << hijo_izq.k<< " y " << hijo_der.k << std::endl;
    return {{hijo_izq, hijo_der}, node.llaves_valores[par_mediano]};
}

void BTree::insert(std::pair<int,float> par, BTreeNode& node, int indice) {
    int value = par.first;
    std::vector<BTreeNode>& arbol = this->nodos;

    if (indice == 0) { // Es la raíz 
        if(node.k < b){ // no está llena
            this->insert(par, node, -1);
        }else{ //está llena, se hace split
            auto [nodos, par_mediano] = this->split(node);
            auto& [R_i, R_d] = nodos;
            auto [k, v] = par_mediano;
            
            // escribir en arreglo ambos nodos
            arbol.push_back(R_i);
            arbol.push_back(R_d);

            this->escrituras += 2; // Se escribieron 2 nodos

            BTreeNode NewR = TreeUtils::crear_raiz(); 
            TreeUtils::agregar_par({k,v}, NewR); // agregamos el par ,ediano a la raiz

            int last_pos = arbol.size();
            NewR.hijos[0] = last_pos-2;
            NewR.hijos[1] = last_pos-1;

            NewR.es_interno = 1;// Pasa a ser un nodo interno
            arbol.at(0) = NewR;// Escribimos la nueva raíz
            this->escrituras += 1; // Se escribió la raíz

            //std::cout << "el tamano arbol antes de insertar en el hijo " << arbol.size() << std::endl;
            if (value <= k){
                this->insert(par, arbol.at(NewR.hijos[0]), NewR.hijos[0]);// Se inserta en la izq si es menor o igual
            }else {
                this->insert(par, arbol.at(NewR.hijos[1]), NewR.hijos[1]);// Se inserta en la derecha de ser contrario
            }
        }

    }else if(node.es_interno == 0){// es una hoja y no raíz
        //std::cout << "Es hoja y hay k pares "<< node.k << std::endl;
        if(indice == -1){
           indice = 0; // Se escribió la raíz
        }
        TreeUtils::agregar_par(par, node); // Agregamos par a H
        //std::cout << "tamano del arbol luego de insertar es " << arbol.size() << " Y estamos insertando en "<< indice << std::endl;
        arbol.at(indice) = node; // Reescribimos la hoja actualizada
        this->escrituras += 1; // Se escribió un nodo
        return;

    }else if (node.es_interno == 1){ //es interno pero no la raíz
        std::cout << "Es interno" << std::endl;
        BTreeNode U;
        if(indice == -1){indice = 0;}// Viene desde la raíz
        
        int pos_U = -1;
        //std::cout << "Es interno. Antes de entrar a buscar al hijo donde se inserta, hay k=" << node.k << std::endl;
        for(int i=0; i < node.k; i++){
            if (value <= node.llaves_valores[i].first){ // no es necesario comparar si es mayor a los de i-1, pues si
                U = arbol.at(node.hijos[i]);            // salta de la iteración, es por que es mayor
                this->lecturas += 1; // Se lee el hijo U

                pos_U = i; // guardamos la posición
                break;
            }
        }

        if (pos_U == -1){
            U = arbol.at(node.hijos[node.k]);
            this->lecturas += 1;

            pos_U = node.k;
        }
        //std::cout << "Se encontro que U esta en el arbol en la pos " << node.hijos[pos_U] << " con U.k=" << U.k << std::endl;
        if (U.k == b){// Si está lleno
            /*BTreeNode U_i;
            BTreeNode U_d;
            int k;
            float v;

            std::pair<std::pair<BTreeNode*,BTreeNode*>, std::pair<int,float>> {{&U_i, &U_d}, {k,v}} = this->split(U);
            std::cout << "Hay un split Y LOS HIJOS TIENEN " << U_i.k<< "y" << U_d.k << std::endl;
            // insertar par llave valor*/
            auto [nodos, par_mediano] = this->split(U);
            auto& [U_i, U_d] = nodos;
            auto [k, v] = par_mediano;

            TreeUtils::agregar_par(std::pair{k,v}, node);

            // escribir nodos
            arbol.push_back(U_d);
            arbol.at(node.hijos[pos_U]) = U_i; 
            this->escrituras += 2; // Se escribieron 2 nodos

            // Agregar indices de hijos
            for(int i = node.k; i > pos_U; i--){
                node.hijos[i+1] = node.hijos[i];
            }
            node.hijos[pos_U + 1] = arbol.size() - 1;

            arbol.at(indice) = node;
            this->escrituras += 1; // Reescribimos el nodo padre

            if (value <= k){
                this->insert(par, U_i, node.hijos[pos_U]);// Se inserta en la izq si es menor o igual
            }else {
                this->insert(par, U_d, node.hijos[pos_U + 1]);// Se inserta en la derecha de ser contrario
            }
        }else{ // Si le queda espacio, se inserta ahí
            //std::cout << "No esta lleno con pares " << U.k << std::endl;    
            //std::cout << "Se va a insertar " <<par.second<<" donde U va en la pos " <<node.hijos[pos_U] 
            //<<" y es interno? "<< U.es_interno <<std::endl;     
            this->insert(par, U, node.hijos[pos_U]);//hay que insertar en la lista llave valor
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
    BTreeNode nodo = TreeUtils::readNode(this->filename, nodo_index);
    std::vector<std::pair<int, float>> resultados;

    if(nodo.es_interno) {

        //buscar en los pares del nodo interno
        for(int i = 0; i < nodo.k; i++) {
            int clave = nodo.llaves_valores[i].first;
            if (clave >= l && clave <= u) {
                resultados.push_back(nodo.llaves_valores[i]);
            }
        }
        
        //Buscar recursivamente en hijos que podrían contener elementos en el rango
        for(int i = 0; i <= nodo.k; i++) {
            if (debeBuscarEnHijo(nodo, i, l, u)) {
                std::vector<std::pair<int, float>> resultados_hijo = buscarRangoB(l, u, nodo.hijos[i]);
                resultados.insert(resultados.end(), resultados_hijo.begin(), resultados_hijo.end());
            }
        }
    } else {

        //buscar en los pares de este nodo
        for(int i = 0; i < nodo.k; i++) {
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
    BTreeNode nodo = TreeUtils::readNode(this->filename, nodo_actual);
    

    while(nodo.es_interno) {
        int i = 0;
        // Encontrar el primer hijo cuya llave sea mayor o igual a l
        while (i < nodo.k && l > nodo.llaves_valores[i].first) {
            i++;
        }
        nodo_actual = nodo.hijos[i];
        nodo = TreeUtils::readNode(this->filename, nodo_actual);

    }
    
    //Recorrer las hojas
    bool continuar = true;
    while(continuar && nodo_actual != -1) {
        nodo = TreeUtils::readNode(this->filename, nodo_actual);

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


namespace TreeUtils {
    BTreeNode crear_raiz() {
        BTreeNode root;
        root.k = 0;
        root.es_interno = 0;// Como es uno recien creado, es un nodo externo

        return root;
    }

    void agregar_par(const std::pair<int, float> par, BTreeNode& node) {
        if (node.k >= b) {
        std::cerr << "ERROR: intentar agregar en nodo ya lleno (k=" << node.k << ")\n";
        std::cerr << "Info del nodo, es interno "<< node.es_interno << ")\n";
        throw std::runtime_error("Error");
        } 

        int i = node.k - 1;
        // simula un shift
        while (i >= 0 && node.llaves_valores[i].first > par.first) {
            node.llaves_valores[i + 1] = node.llaves_valores[i];
            --i;
        }
        node.llaves_valores[i + 1] = par; 
        node.k ++;

    }

    void write_node(const std::string &filename, const BTreeNode node, int indice){

        std::ofstream out(filename, std::ios::binary);
        if (!out) {
            std::cerr << "Error al abrir archivo para escritura: " << filename
                      << std::endl;
            std::exit(1);
        }

        std::streampos file_offset = indice * sizeof(BTreeNode);
        out.seekp(file_offset);
        std::cout << "llave 3:" << node.llaves_valores[2].second << std::endl;
        out.write(reinterpret_cast<const char *>(&node), sizeof(node));
        out.close();
   
    }
    
    BTreeNode readNode(std::string &filename, int offset) {
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
}