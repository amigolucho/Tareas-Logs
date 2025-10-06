#include "./btree.cpp"
#include <math.h>

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

   //std::vector<BTreeNode> BTree;
   BTreeNode node = TreeUtils::crear_raiz();
   node.es_interno = -1; // es raiz

   std::ifstream in("../datos.bin", std::ios::binary);
   std::pair<int,float> par;

   for (int i = 0; i<10; i++){
        std::streampos file_offset = i * sizeof(std::pair<int,float>);
        in.seekg(file_offset);
        in.read(reinterpret_cast<char *>(&par), sizeof(std::pair<int,float>));
        BTree.insert(par, node, 0);
   }

   TreeUtils::write_node(filename, node);

   return 0;
};
