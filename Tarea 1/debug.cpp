#include <fstream>
#include <iostream>
#include <utility>

int main() {
    std::ifstream in("datos.bin", std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "❌ No se pudo abrir datos.bin\n";
        return 1;
    }

    std::pair<int,float> par;
    int count = 0;
    while (in.read(reinterpret_cast<char*>(&par), sizeof(par)) && count < 10) {
        std::cout << "Par " << count << ": (" << par.first << ", " << par.second << ")\n";
        count++;
    }

    in.close();
    return 0;
}