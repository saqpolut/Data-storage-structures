#include "AVL_tree.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

unsigned char getHeightT(NodeT* node) {
    return node ? node->height : 0;
}

int getBalanceFactorT(NodeT* node) {
    return node ? getHeightT(node->left) - getHeightT(node->right) : 0;
}

void fixHeightT(NodeT* node) {
    if (!node) return;
    unsigned char hL = getHeightT(node->left);
    unsigned char hR = getHeightT(node->right);
    node->height = (hL > hR ? hL : hR) + 1;
}

//повороты
NodeT* rotateRightT(NodeT* node) {
    NodeT* tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    fixHeightT(node);
    fixHeightT(tmp);
    return tmp;
}

NodeT* rotateLeftT(NodeT* node) {
    NodeT* tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    fixHeightT(node);
    fixHeightT(tmp);
    return tmp;
}

//вставка
NodeT* insertT(NodeT* root, int key) {
    if (!root) return new NodeT(key);

    if (key < root->key) {
        root->left = insertT(root->left, key);
    } else if (key > root->key) {
        root->right = insertT(root->right, key);
    } else {
        return root; // Дубликаты не добавляем
    }

    fixHeightT(root);

    int balance = getBalanceFactorT(root);

    // Левый случай
    if (balance == 2) {
        if (getBalanceFactorT(root->left) >= 0) {
            return rotateRightT(root);
        } else {
            root->left = rotateLeftT(root->left);
            return rotateRightT(root);
        }
    }

    // Правый случай
    if (balance == -2) {
        if (getBalanceFactorT(root->right) <= 0) {
            return rotateLeftT(root);
        } else {
            root->right = rotateRightT(root->right);
            return rotateLeftT(root);
        }
    }

    return root;
}

//Поиск
NodeT* findT(NodeT* root, int key) {
    if (!root) return nullptr;
    if (key == root->key) return root;
    if (key < root->key) return findT(root->left, key);
    return findT(root->right, key);
}

NodeT* findWithStepsT(NodeT* root, int key, int& steps) {
    steps = 0;
    while (root != nullptr) {
        ++steps;
        if (key == root->key) return root;
        root = (key < root->key) ? root->left : root->right;
    }
    return nullptr;
}

NodeT* findMinT(NodeT* node) {
    if (!node) return nullptr;
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

//Удаление
NodeT* removeT(NodeT* root, int key) {
    if (!root) return nullptr;

    if (key < root->key) {
        root->left = removeT(root->left, key);
    } else if (key > root->key) {
        root->right = removeT(root->right, key);
    } else {
        // Нашли узел для удаления
        if (!root->left) {
            NodeT* tmp = root->right;
            delete root;
            return tmp;
        } else if (!root->right) {
            NodeT* tmp = root->left;
            delete root;
            return tmp;
        }

        // Узел с двумя детьми: заменяем минимальным из правого поддерева
        NodeT* minNode = findMinT(root->right);
        root->key = minNode->key;
        root->right = removeT(root->right, minNode->key);
    }

    if (!root) return nullptr;

    fixHeightT(root);

    int balance = getBalanceFactorT(root);

    // Левый случай
    if (balance == 2) {
        if (getBalanceFactorT(root->left) >= 0) {
            return rotateRightT(root);
        } else {
            root->left = rotateLeftT(root->left);
            return rotateRightT(root);
        }
    }

    // Правый случай
    if (balance == -2) {
        if (getBalanceFactorT(root->right) <= 0) {
            return rotateLeftT(root);
        } else {
            root->right = rotateRightT(root->right);
            return rotateLeftT(root);
        }
    }

    return root;
}

//Чтение и обходы

void printInOrderT(NodeT* root) {
    if (!root) return;
    printInOrderT(root->left);
    cout << root->key << " ";
    printInOrderT(root->right);
}

void printPreOrderT(NodeT* root) {
    if (!root) return;
    cout << root->key << " ";
    printPreOrderT(root->left);
    printPreOrderT(root->right);
}

void printPostOrderT(NodeT* root) {
    if (!root) return;
    printPostOrderT(root->left);
    printPostOrderT(root->right);
    cout << root->key << " ";
}

// Печатает узлы одного уровня (уровень 1 = корень)
static void printLevelT(NodeT* root, int level) {
    if (!root) return;
    if (level == 1) { cout << root->key << " "; return; }
    printLevelT(root->left,  level - 1);
    printLevelT(root->right, level - 1);
}

// Обход в ширину: уровни сверху вниз, внутри уровня слева направо
void printLevelOrderT(NodeT* root) {
    int h = getHeightT(root);
    for (int lvl = 1; lvl <= h; ++lvl) printLevelT(root, lvl);
}

int countNodesT(NodeT* root) {
    if (!root) return 0;
    return 1 + countNodesT(root->left) + countNodesT(root->right);
}

void printTreeT(NodeT* root, int level) {
    if (!root) return;
    printTreeT(root->right, level + 1);
    for (int i = 0; i < level; ++i) cout << "    ";
    cout << root->key << endl;
    printTreeT(root->left, level + 1);
}

void printTreeInfoT(NodeT* root, const char* title) {
    cout << "\n" << title << "\n";
    if (!root) { cout << "(дерево пусто)\n"; return; }

    cout << "По возрастанию (in-order): ";  printInOrderT(root);    cout << "\n";
    cout << "Прямой обход (pre-order): ";   printPreOrderT(root);   cout << "\n";
    cout << "Обратный обход (post-order): "; printPostOrderT(root);  cout << "\n";
    cout << "В ширину (по уровням): ";      printLevelOrderT(root); cout << "\n";
    cout << "Высота дерева: " << (int)getHeightT(root) << "\n";
    cout << "Количество узлов: " << countNodesT(root) << "\n";
    cout << "Структура (повёрнуто на 90 градусов):\n";
    printTreeT(root, 0);
}

//Очистка
void destroyTreeT(NodeT* root) {
    if (!root) return;
    destroyTreeT(root->left);
    destroyTreeT(root->right);
    delete root;
}

// Pre-order с маркером '#' на месте пустого узла.
// Так форма дерева кодируется однозначно.
static void saveTreeHelper(NodeT* root, ofstream& out) {
    if (!root) {
        out << "# ";
        return;
    }
    out << root->key << " ";
    saveTreeHelper(root->left, out);
    saveTreeHelper(root->right, out);
}

bool saveTreeToFile(NodeT* root, const char* filename) {
    ofstream out(filename);
    if (!out) return false;
    saveTreeHelper(root, out);
    out << "\n";
    return true;
}

// Восстанавливаем узлы напрямую (без insertT), поэтому форма гарантированно совпадает с сохранённой.
static NodeT* loadTreeHelper(ifstream& in) {
    string tok;
    if (!(in >> tok)) return nullptr;
    if (tok == "#")   return nullptr;   // маркер пустого узла

    int key;
    try {
        key = stoi(tok);                // stoi умеет кидать: пустой/мусорный токен, переполнение
    }
    catch (const exception&) {
        return nullptr;                 // битый токен — считаем узел пустым
    }

    NodeT* node = new NodeT(key);
    node->left  = loadTreeHelper(in);
    node->right = loadTreeHelper(in);
    fixHeightT(node);               // высоты пересчитываем снизу вверх
    return node;
}

bool loadTreeFromFile(NodeT*& root, const char* filename) {
    ifstream in(filename);
    if (!in) return false;         // файла нет: старое дерево не трогаем

    destroyTreeT(root);            // очищаем текущее дерево
    root = loadTreeHelper(in);     // строим из файла
    return true;
}
