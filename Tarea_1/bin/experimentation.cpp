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
   const std::string filename = "../Tarea 1/tree.bin";

   BTree BTree(filename);
   BTreeNode node = TreeUtils::crear_raiz();

   std::ifstream in("../datos.bin", std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "No se pudo abrir datos.bin\n";
        return 1;
    }

<<<<<<< Updated upstream
   for (int i = 0; i<10; i++){
        std::streampos file_offset = i * sizeof(std::pair<int,float>);
        in.seekg(file_offset);
        in.read(reinterpret_cast<char *>(&par), sizeof(std::pair<int,float>));
        std::cout << "se agregara; "<< par.second << std::endl;
=======
    for (int i = 0; i < 342; i++) {
        int key;
        float value;

        in.read(reinterpret_cast<char*>(&key), sizeof(int));
        in.read(reinterpret_cast<char*>(&value), sizeof(float));

        if (!in.good()) break; // por si llega al final

        std::pair<int,float> par = {key, value};
        std::cout << "Se leerá: {" << key << ", " << value << "}\n";
>>>>>>> Stashed changes
        BTree.insert(par, node, 0);
    }

   TreeUtils::write_node(filename, node, 0);

   std::string fileout = "../tree.bin";
   BTreeNode node2 = TreeUtils::readNode(fileout, 0);
   std::cout << node2.llaves_valores[339].second << std::endl;

   return 0;
};
