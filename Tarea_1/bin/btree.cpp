#include "../include/btree.hpp"
#include <iostream>
#include <utility>
#include <vector>

//implementación del árbol B
BTree::BTree(const std::string &filename){
    this->filename = filename;
    this->nodos.resize(1);
}

std::pair<std::pair<BTreeNode, BTreeNode>, std::pair<int,float>> 
BTree::split(const BTreeNode& node) const {

    int mid = node.k / 2;
    std::pair<int, float> median = node.llaves_valores[mid];

    BTreeNode left, right;
    left.es_interno = node.es_interno;
    right.es_interno = node.es_interno;

    // Copiar llaves y valores a la izquierda
    for (int i = 0; i < mid; i++) {
        left.llaves_valores[i] = node.llaves_valores[i];
    }
    left.k = mid;

    // Copiar llaves y valores a la derecha (después del mediano)
    for (int i = mid + 1, j = 0; i < node.k; i++, j++) {
        right.llaves_valores[j] = node.llaves_valores[i];
    }
    right.k = node.k - mid - 1;

    // Copiar punteros hijos si es nodo interno
    if (node.es_interno) {
        for (int i = 0; i <= mid; i++)
            left.hijos[i] = node.hijos[i];
        for (int i = mid + 1, j = 0; i <= node.k; i++, j++)
            right.hijos[j] = node.hijos[i];
    }

    return {{left, right}, median};
}

void BTree::insert(const std::pair<int, float>& par, BTreeNode& node, int indice) {
    if (node.es_interno == 0) {
        // Nodo hoja
        if (node.k < b) {
            TreeUtils::agregar_par(par, node);
            node.k++;
            if (indice >= 0)
                this->nodos[indice] = node;
            else
                this->nodos[0] = node;
            return;
        } else {
            // Dividir hoja llena
            auto [hijos, mediana] = split(node);
            auto [left, right] = hijos;

            // Escribir hijos
            this->nodos.push_back(left);
            this->nodos.push_back(right);

            // Nueva raíz
            BTreeNode new_root = TreeUtils::crear_raiz();
            new_root.es_interno = 1;
            new_root.k = 1;
            new_root.llaves_valores[0] = mediana;
            new_root.hijos[0] = this->nodos.size() - 2;
            new_root.hijos[1] = this->nodos.size() - 1;
            this->nodos[0] = new_root;

            // Insertar el nuevo valor en el hijo correcto
            if (par.first < mediana.first)
                insert(par, this->nodos[new_root.hijos[0]], new_root.hijos[0]);
            else
                insert(par, this->nodos[new_root.hijos[1]], new_root.hijos[1]);
        }
    } else {
        // Nodo interno
        int i = 0;
        while (i < node.k && par.first > node.llaves_valores[i].first)
            i++;

        int hijo_index = node.hijos[i];
        BTreeNode& hijo = this->nodos[hijo_index];

        if (hijo.k == b) {
            auto [hijos, mediana] = split(hijo);
            auto [left, right] = hijos;
            this->nodos[hijo_index] = left;
            this->nodos.push_back(right);
            int new_index = this->nodos.size() - 1;

            // Insertar la mediana en el nodo actual
            TreeUtils::agregar_par(mediana, node);
            node.k++;
            for (int j = node.k; j > i + 1; j--)
                node.hijos[j] = node.hijos[j - 1];
            node.hijos[i + 1] = new_index;

            // Elegir el hijo adecuado
            if (par.first < mediana.first)
                insert(par, this->nodos[hijo_index], hijo_index);
            else
                insert(par, this->nodos[new_index], new_index);
        } else {
            insert(par, hijo, hijo_index);
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
        int i = node.k - 1;
        // Mueve los pares mayores hacia la derecha
        while (i >= 0 && par.first < node.llaves_valores[i].first) {
            node.llaves_valores[i + 1] = node.llaves_valores[i];
            i--;
        }
        node.llaves_valores[i + 1] = par;
        std::cout << "Se agregó el par {" << par.first << ", " << par.second << "}\n";
    }

    void write_node(const std::string &filename, const BTreeNode node, int indice){

        std::ofstream out(filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!out) {
            std::cerr << "Error al abrir archivo para escritura: " << filename
                      << std::endl;
            std::exit(1);
        }

        std::streampos file_offset = indice * sizeof(BTreeNode);
        out.seekp(file_offset);
        out.write(reinterpret_cast<const char*>(&node), sizeof(BTreeNode));

        if (!out.good()) {
            std::cerr << "Error al escribir nodo en índice " << indice << std::endl;
        } else {
            std::cout << "Nodo " << indice << " escrito correctamente (" 
                      << node.k << " claves, es_interno=" << node.es_interno << ")\n";
        }

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
