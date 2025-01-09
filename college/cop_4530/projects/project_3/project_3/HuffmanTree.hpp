#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include "HuffmanBase.hpp"
#include "HeapQueue.hpp"
#include <iostream>
#include <map>
#include <stack>

class HuffmanTree : public HuffmanTreeBase {
public:
    HuffmanTree(); // Constructor
    ~HuffmanTree(); // Destructor to clean up the heap and tree nodes

    std::string compress(const std::string inputStr) override;
    std::string serializeTree() const override;
    std::string decompress(const std::string inputCode, const std::string serializedTree) override;

    void frequencyGenerator(const std::string &inputStr);
    void buildTree();
    void encodingGenerator(HuffmanNode* node, const std::string &code );

    std::string postOrderSerialize(HuffmanNode* node) const;
    HuffmanNode* deserializeTree(const std::string &serializedTree) const;

    void deleteTree(HuffmanNode* node);

private:
    HeapQueue<HuffmanNode*, HuffmanNode::Compare>* heap;
    HuffmanNode* root;
    std::map<char, std::string> encodingMap;
};

#endif // HUFFMANTREE_H
