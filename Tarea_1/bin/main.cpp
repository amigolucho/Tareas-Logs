#include "./btree.cpp"
#include <iostream>
#include <fstream>

int main() {
    const std::string filename = "../tree.bin";
    const std::string datafile = "../datos.bin";

    // Inicializar árbol B
    BTree tree(filename);
    BTreeNode root = TreeUtils::crear_raiz();

    std::ifstream in(datafile, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << " No se pudo abrir " << datafile << std::endl;
        return 1;
    }

    int N = 50; // puedes cambiarlo a 100, 1000, etc.
    std::cout << " Insertando los primeros " << N << " pares del archivo binario...\n";

    for (int i = 0; i < N; i++) {
        int key;
        float value;
        in.read(reinterpret_cast<char*>(&key), sizeof(int));
        in.read(reinterpret_cast<char*>(&value), sizeof(float));
        if (!in.good()) break;

        std::pair<int, float> par = {key, value};
        std::cout << " Insertando {" << key << ", " << value << "}\n";
        tree.insert(par, root, 0);
    }

    // Escribir raíz del árbol
    TreeUtils::write_node(filename, root, 0);
    std::cout << "Nodo raíz guardado en " << filename << "\n";

    // Leer nodo nuevamente desde el archivo
    BTreeNode check = TreeUtils::readNode(const_cast<std::string&>(filename), 0);

    std::cout << "\n Verificando contenido leído desde disco:\n";
    for (int i = 0; i < check.k; i++) {
        std::cout << "   [" << i << "] "
                  << check.llaves_valores[i].first << " -> "
                  << check.llaves_valores[i].second << std::endl;
    }

    return 0;
}

