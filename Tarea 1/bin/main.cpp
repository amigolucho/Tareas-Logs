#include "../include/btree.hpp"
#include <iostream>

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

    return 0;
}