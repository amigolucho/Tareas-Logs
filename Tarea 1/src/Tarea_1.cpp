#ifndef B_TREE_HPP
#define B_TREE_HPP


#include <iostream>
#include <list>
#include "block_size.h"
using namespace std;

const int b = 340;

class B_tree {
    public:
        B_tree(int a) {};
        int b =1;
        int es_interno; // 1 si es interno, 0 si es externo
        int k; // cantidad pares llave-valor
        std::pair<int, float> llaves_valores[340];// arreglo de llaves
        int hijos[341];// areglo hijos
        //siguiente para arboles ++
};

class B_tree_pp : public B_tree {
    public: 
        int siguiente = -1;//setearlo con un metodo?
};

#endif

int main(){
    std::cout << "Tamaño de bloque (B): " ;
    return 0;
}
