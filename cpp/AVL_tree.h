#ifndef AVL_TREE_H
#define AVL_TREE_H

// Структура узла AVL-дерева
struct NodeT {
    int key;
    NodeT* left;
    NodeT* right;
    unsigned char height;
    NodeT(int k) { key = k; left = nullptr; right = nullptr; height = 1; }
};

unsigned char getHeightT(NodeT* node);
int getBalanceFactorT(NodeT* node);
void fixHeightT(NodeT* node);

NodeT* rotateRightT(NodeT* node);
NodeT* rotateLeftT(NodeT* node);

NodeT* insertT(NodeT* root, int key);
NodeT* removeT(NodeT* root, int key);

NodeT* findT(NodeT* root, int key);
NodeT* findWithStepsT(NodeT* root, int key, int& steps);
NodeT* findMinT(NodeT* node);

void printInOrderT(NodeT* root);
void printPreOrderT(NodeT* root);
void printPostOrderT(NodeT* root);
void printLevelOrderT(NodeT* root);
int countNodesT(NodeT* root);
void printTreeT(NodeT* root, int level);
void printTreeInfoT(NodeT* root, const char* title);

void destroyTreeT(NodeT* root);

bool saveTreeToFile(NodeT* root, const char* filename);
bool loadTreeFromFile(NodeT*& root, const char* filename);

#endif
