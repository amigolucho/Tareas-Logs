#ifndef BPTREE_HPP
#define BPTREE_HPP

#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include "../include/btree.hpp" 

class BPlusTree {
public:
    std::string filename;
    std::vector<BTreeNode> nodos;
    int lecturas = 0;
    int escrituras = 0;

    // Constructor
    BPlusTree(const std::string &filename);

    // Operaciones principales
    void insert(std::pair<int, float> par, BTreeNode &node, int indice);
    std::pair<std::pair<BTreeNode,BTreeNode>, std::pair<int,float>> split(BTreeNode &node) const;
    std::vector<std::pair<int, float>> buscarRango(int l, int u);
    std::vector<std::pair<int, float>> buscarRangoBmas(int l, int u);

private:
    std::vector<std::pair<int, float>> buscarRangoRec(int l, int u, int nodo_index);
};

#endif
