#include "./btree.cpp"
#include <math.h>
#include <iostream>

int main(){
    // Transformamos la lista en los N correspondientes
    /*for (int i = 0; i<1; i++){// testeamos para N 2 ** 15, despues lo vamos alargando (i<=11)
        int N = pow(2, 15 + i);
        for (int j = 0; j <= N; j++){
            printf("%d\n", j);
        };
    }*/
   const std::string filename = "../tree.bin";

   BTree BTree(filename);
   BTreeNode node = TreeUtils::crear_raiz();

   std::ifstream in("../datos.bin", std::ios::binary);
   std::pair<int,float> par;

   for (int i = 0; i<10; i++){
        std::streampos file_offset = i * sizeof(std::pair<int,float>);
        in.seekg(file_offset);
        in.read(reinterpret_cast<char *>(&par), sizeof(std::pair<int,float>));
        std::cout << "se agregara; "<< par.second << std::endl;
        BTree.insert(par, node, 0);
   }

   TreeUtils::write_node(filename, node, 0);

   std::string fileout = "../tree.bin";
   BTreeNode node2 = TreeUtils::readNode(fileout, 0);
   std::cout << node2.llaves_valores[339].second << std::endl;

   return 0;
};
