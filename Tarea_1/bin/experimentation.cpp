#include "./btree.cpp"
#include <math.h>
#include <iostream>

int main(){
    // Transformamos la lista en los N correspondientes
    const std::string filename = "../tree.bin";
    std::ifstream in("../datos.bin", std::ios::binary);
    std::pair<int,float> par;

    if (!in.is_open()) {
    std::cerr << "Error al abrir archivo para lectura: " << "../datos.bin"
              << std::endl;
    std::exit(1);
  }

    for (int i = 0; i<1; i++){// testeamos para N 2 ** 15, despues lo vamos alargando (i<=11)
        BTree bTree(filename);
        bTree.nodos.at(0) = TreeUtils::crear_raiz();
        int N = pow(2, 15 + i);

        for (int j = 0; j <= N; j++){
            std::streampos file_offset = j * sizeof(std::pair<int,float>);
            in.seekg(file_offset);
            in.read(reinterpret_cast<char *>(&par), sizeof(std::pair<int,float>));
            if (!in) {
                std::cerr << "Error al leer par en posición " << file_offset << std::endl;
                std::exit(1);
            }
            std::cout << "Iteracion N=2^"<< 15+i <<" se inserta el par "<< par.second << " Iteraciones " << j << std::endl;
            bTree.insert(par, 0);
        };

        std::cout << "Escrituras para N=2^"<< 15+i << " "<< bTree.escrituras << std::endl;
        std::cout << "Lecturas para N=2^"<< 15+i << " "<< bTree.lecturas << std::endl;
        std::cout << "Nodos para N=2^"<< 15+i << " " << bTree.nodos.size() << std::endl;
        
    }


   //TreeUtils::write_node(filename, node, 0);
   

   return 0;
};
