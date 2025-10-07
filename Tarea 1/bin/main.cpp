#include "../include/btree.hpp"
#include <iostream>
#include <vector>

int main() {
    const std::string filename = "../datos.bin";
    BTree arbol(filename);
    
    std::cout << "Probando con rango MUY pequeño [1000, 5000]:\n";
    auto resultados = arbol.buscarRango(1000, 5000);
    std::cout << "Encontrados: " << resultados.size() << " resultados\n";
    
    std::cout << "\nProbando con rango pequeño [10000, 50000]:\n";
    resultados = arbol.buscarRango(10000, 50000);
    std::cout << "Encontrados: " << resultados.size() << " resultados\n";
    
    if (resultados.size() < 1000) { // límite razonable
        std::cout << "\nBúsqueda de rango [1620000000, 1680000000]:\n";
        resultados = arbol.buscarRango(1620000000, 1680000000);
        
        if (resultados.empty()) {
            std::cout << "No se encontraron resultados en ese rango.\n";
        } else {
            int max_mostrar = std::min(10, (int)resultados.size());
            for (int i = 0; i < max_mostrar; i++) {
                std::cout << resultados[i].first << " -> " << resultados[i].second << std::endl;
            }
            if (resultados.size() > max_mostrar) {
                std::cout << "... y " << (resultados.size() - max_mostrar) << " más\n";
            }
            std::cout << "Total de resultados: " << resultados.size() << std::endl;
        }
    }
    
    return 0;
}