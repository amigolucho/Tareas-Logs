#include "../include/btree.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

int main() {
    const std::string filename = "../tree.bin";  // acá guardará el árbol

    // 1️⃣ Construir el árbol desde datos.bin
    BTree arbol(filename);
    BTreeNode raiz = TreeUtils::crear_raiz();

    std::ifstream in("../datos.bin", std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "❌ No se pudo abrir datos.bin\n";
        return 1;
    }

    std::pair<int, float> par;
    int count = 0;
    while (in.read(reinterpret_cast<char*>(&par), sizeof(par))) {
        arbol.insert(par, raiz, 0);
        count++;
    }
    in.close();
    TreeUtils::write_node(filename, raiz, 0);

    std::cout << "✅ Árbol B creado con " << count << " pares.\n\n";

    // 2️⃣ Probar tus funciones de búsqueda reales
    std::vector<std::pair<int, float>> resultados;

    std::cout << "1️⃣ Rango pequeño [1627211543, 1627249463]:\n";
    resultados = arbol.buscarRango(1627211543, 1627249463);
    std::cout << "Encontrados: " << resultados.size() << " resultados\n";
    for (auto &r : resultados)
        std::cout << r.first << " -> " << r.second << std::endl;

    std::cout << "\n2️⃣ Rango medio [1670000000, 1680000000]:\n";
    resultados = arbol.buscarRango(1670000000, 1680000000);
    std::cout << "Encontrados: " << resultados.size() << " resultados\n";
    for (auto &r : resultados)
        std::cout << r.first << " -> " << r.second << std::endl;

    std::cout << "\n3️⃣ Rango específico [1733426259, 1740499888]:\n";
    resultados = arbol.buscarRango(1733426259, 1740499888);
    std::cout << "Encontrados: " << resultados.size() << " resultados\n";
    for (auto &r : resultados)
        std::cout << r.first << " -> " << r.second << std::endl;

    std::cout << "\n4️⃣ Búsqueda exacta [1644113316, 1644113316]:\n";
    resultados = arbol.buscarRango(1644113316, 1644113316);
    std::cout << "Encontrados: " << resultados.size() << " resultados\n";
    for (auto &r : resultados)
        std::cout << r.first << " -> " << r.second << std::endl;

    std::cout << "\n✅ Fin de las pruebas del árbol B.\n";
    return 0;
}
