#include "../include/btree.hpp"

//implementación del árbol B
BTree::BTree(const std::string &filename){
    this->filename = filename;
}

BTreeNode BTree::split(int offset) const {
    
}