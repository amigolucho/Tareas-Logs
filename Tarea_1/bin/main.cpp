#include "./btree.cpp"
#include <iostream>
<<<<<<< Updated upstream

int main() {
    const std::string filename = "../tree.bin";
    BTree arbol(filename);
    BTreeNode raiz = TreeUtils::crear_raiz();

    std::pair<int, float> datos[] = {
        {1733426259, 22.4}, {1740499888, 30.4}, {1677308924, 12.9},
        {1580102557, 19.8}, {1627211543, 5.05}, {1644113316, 22.3}
    };

    for (auto &par : datos) {
        arbol.insert(par, raiz, 0);
    }

    TreeUtils::write_node(filename, raiz, 0);

    std::cout << "\nBúsqueda de rango [1620000000, 1680000000]:\n";
    auto resultados = arbol.buscarRango(1620000000, 1680000000);
    for (auto &r : resultados)
        std::cout << r.first << " -> " << r.second << std::endl;
=======
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
        std::cout << "➡️ Insertando {" << key << ", " << value << "}\n";
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
>>>>>>> Stashed changes

    return 0;
}

