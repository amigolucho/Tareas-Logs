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

    // Escribimos nuestros resultados
    ofstream archivo("../resultado.txt");
    if (!archivo) {  // siempre conviene verificar que se abrió bien
        cerr << "Error al abrir el archivo" << endl;
        return 1;
    }




    for (int i = 0; i<2; i++){// testeamos para N 2 ** 15, despues lo vamos alargando (i<=11)
        BTree BTree(filename);
        BTreeNode node = TreeUtils::crear_raiz();
        int N = pow(2, 15 + i);

        for (int j = 0; j <= N; j++){
            std::streampos file_offset = j * sizeof(std::pair<int,float>);
            in.seekg(file_offset);
            in.read(reinterpret_cast<char *>(&par), sizeof(std::pair<int,float>));
            if (!in) {
                std::cerr << "Error al leer par en posición " << file_offset << std::endl;
                std::exit(1);
            }
            //std::cout << "Iteracion N=2^"<< 15+i <<" se inserta el par "<< par.second << " Iteraciones " << j << std::endl;
            BTree.insert(par, node, 0);
        };

        archivo << "Escrituras para N=2^"<< 15+i << " "<< BTree.escrituras << std::endl;
        archivo << "Lecturas para N=2^"<< 15+i << " "<< BTree.lecturas << std::endl;
        archivo << "Nodos para N=2^"<< 15+i << " " << BTree.nodos.size() << std::endl;
        
    
    int pares = 0;
    for(int i =0; i < BTree.nodos.size(); i++){
        pares += BTree.nodos.at(i).k;
    }
    std::cout << "pares totales "<< pares << std::endl;}

    archivo.close();


   //TreeUtils::write_node(filename, node, 0);
   

   return 0;
};
