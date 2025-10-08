#include "./btree.cpp"
#include <math.h>
#include <iostream>

int main(){
    // Transformamos la lista en los N correspondientes
    const std::string filename = "../tree.bin";
    std::ifstream in("../datos.bin", std::ios::binary);
    std::pair<int,float> par;

    for (int i = 0; i<11; i++){// testeamos para N 2 ** 15, despues lo vamos alargando (i<=11)
        BTree BTree(filename);
        BTreeNode node = TreeUtils::crear_raiz();
        int N = pow(2, 15 + i);

        for (int j = 0; j <= N; j++){
            std::streampos file_offset = i * sizeof(std::pair<int,float>);
            in.seekg(file_offset);
            in.read(reinterpret_cast<char *>(&par), sizeof(std::pair<int,float>));
            std::cout << "Iteracion N=2^"<< 15+i <<" se inserta el par "<< par.second << " Iteraciones " << j << std::endl;
            BTree.insert(par, node, 0);
        };

        std::cout << "Escrituras para N=2^"<< 15+1 << BTree.escrituras << std::endl;
        std::cout << "Lecturas para N=2^"<< 15+1 << BTree.lecturas << std::endl;
        std::cout << "Nodos para N=2^"<< 15+1 << BTree.nodos.size() << std::endl;
        
    }


   //TreeUtils::write_node(filename, node, 0);
   

   return 0;
};
