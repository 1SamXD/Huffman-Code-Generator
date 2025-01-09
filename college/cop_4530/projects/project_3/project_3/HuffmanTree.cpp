#include "HuffmanTree.hpp"
#include <iostream>

// constructor
HuffmanTree::HuffmanTree() {
    heap = new HeapQueue<HuffmanNode*, HuffmanNode::Compare>();
    root = nullptr;
}

// destructor to delete evrything
HuffmanTree::~HuffmanTree() {
    delete heap;
    deleteTree(root);
}

// recursive deleting node function
void HuffmanTree::deleteTree(HuffmanNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

//compression function
std::string HuffmanTree::compress(const std::string inputStr) {
    frequencyGenerator(inputStr);
    buildTree();
    encodingGenerator(root, "");
    std::string compressed;
    for (char ch : inputStr) {
        compressed += encodingMap[ch];
    }
    return compressed;
}

//function to get frequency and put it in a map
void HuffmanTree::frequencyGenerator(const std::string &inputStr) {
    std::map<char, int> frequency;
    for (char ch : inputStr) {
        frequency[ch]++;
    }

    for (const auto& var : frequency) {
        HuffmanNode* node = new HuffmanNode(var.first, var.second);
        heap->insert(node);
    }
}

//function to build heap
void HuffmanTree::buildTree() {
    while (heap->size() > 1) {
        HuffmanNode* left = heap->min();
        heap->removeMin();
        HuffmanNode* right = heap->min();
        heap->removeMin();

        HuffmanNode* parent = new HuffmanNode('\0', left->getFrequency() + right->getFrequency(), nullptr, left, right);
        left->parent = parent;
        right->parent = parent;
        heap->insert(parent);
    }
    root = heap->min();
    heap->removeMin();
}

//function that assign branch or edge values for the code
void HuffmanTree::encodingGenerator(HuffmanNode* node, const std::string &code) {
    if (!node) return;

    if (node->isLeaf()) {
        encodingMap[node->getCharacter()] = code;
    } 
    else {
        encodingGenerator(node->left, code + "0");
        encodingGenerator(node->right, code + "1");
    }
}

// function to seralize the tree using post order traveral
std::string HuffmanTree::serializeTree() const {
    return postOrderSerialize(root);
}

//function to recurse to serlize
std::string HuffmanTree::postOrderSerialize(HuffmanNode* node) const {
    if (!node) return "";
    std::string serialized;
    serialized += postOrderSerialize(node->left);
    serialized += postOrderSerialize(node->right);
    serialized += node->isLeaf() ? "L" + std::string(1, node->getCharacter()) : "B";
    return serialized;
}

//function to decode the string
std::string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree) {
    root = deserializeTree(serializedTree);
    std::string decompressed;
    HuffmanNode* currentNode = root;
    for (char bit : inputCode) {
        currentNode = (bit == '0') ? currentNode->left : currentNode->right;
        if (currentNode->isLeaf()) {
            decompressed += currentNode->getCharacter();
            currentNode = root;
        }
    }
    return decompressed;
}

// function to undo the seralization to decompress
HuffmanNode* HuffmanTree::deserializeTree(const std::string &serializedTree) const {
    std::stack<HuffmanNode*> nodeStack;
    for (size_t i = 0; i < serializedTree.size(); ++i) {
        if (serializedTree[i] == 'L') {
            char character = serializedTree[++i];
            HuffmanNode* leafNode = new HuffmanNode(character, 0);
            nodeStack.push(leafNode);
        } 
        else if (serializedTree[i] == 'B') {
            if (nodeStack.size() < 2) throw std::logic_error("Invalid serialized tree format.");
            HuffmanNode* right = nodeStack.top();
            nodeStack.pop();
            HuffmanNode* left = nodeStack.top();
            nodeStack.pop();
            HuffmanNode* branchNode = new HuffmanNode('\0', 0, nullptr, left, right);
            nodeStack.push(branchNode);
        }
    }
    if (nodeStack.size() != 1) throw std::logic_error("Invalid serialized tree format.");
    return nodeStack.top();
}