#include "singlyLinkedList.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Вспомогательная функция для обратного вывода
static void printBackwardRecF(NodeF* node) {
    if (node == nullptr) return;
    printBackwardRecF(node->next);
    cout << node->data << " ";
}

//имеем пустой список
void initListF(ListF* list) {
    list->head = nullptr;
    list->tail = nullptr;
}

//проверка на пустоту
bool isEmptyF(const ListF* list) {
    return list->head == nullptr;
}

//размер
int sizeF(const ListF* list) {
    int count = 0;
    for (NodeF* current = list->head; current != nullptr; current = current->next) {
        ++count;
    }
    return count;
}

//поиск узла по значению (возвращаем указатель на узел)
NodeF* findNodeF(const ListF* list, const string& value) {
    for (NodeF* current = list->head; current != nullptr; current = current->next) {
        if (current->data == value) return current;
    }
    return nullptr;
}

//получение узла по номеру
NodeF* getAtF(const ListF* list, int index) {
    if (index < 0) return nullptr;
    NodeF* cur = list->head;
    for (int i = 0; cur != nullptr && i < index; ++i)
        cur = cur->next;
    return cur;
}

// Добавление 4 способа

//вставка в голову
void pushFrontF(ListF* list, const string& value) {
    NodeF* node = new NodeF(value);
    node->next = list->head;
    list->head = node;
    if (list->tail == nullptr) {//если список пуст - новый узел есть и голова и хвост
        list->tail = node;
    }
}

//вставка в хвост
void pushBackF(ListF* list, const string& value) {
    NodeF* node = new NodeF(value);
    if (list->tail == nullptr) {
        list->head = list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
}

//вставка после какого либо элемента
bool insertAfterF(ListF* list, const string& target, const string& value) {
    NodeF* targetNode = findNodeF(list, target);
    if (targetNode == nullptr) return false;

    NodeF* node = new NodeF(value);
    node->next = targetNode->next;
    targetNode->next = node;

    if (list->tail == targetNode) list->tail = node; //вставка после конечного элемента
    return true;
}

//вставка перед элементом
bool insertBeforeF(ListF* list, const string& target, const string& value) {
    if (list->head == nullptr) return false;

    if (list->head->data == target) {//если на место головы
        pushFrontF(list, value);
        return true;
    }

    NodeF* prev = list->head; //ищем предыдущий узел
    while (prev->next != nullptr && prev->next->data != target)
        prev = prev->next;

    if (prev->next == nullptr) return false;

    NodeF* node = new NodeF(value);
    node->next = prev->next;
    prev->next = node;
    return true;
}

//удаление 4 способа

//удаление с начала
bool popFrontF(ListF* list) {
    if (list->head == nullptr) return false;

    NodeF* tmp = list->head;
    list->head = list->head->next;
    if (list->head == nullptr) list->tail = nullptr;
    delete tmp;
    return true;
}

//удаление с конца
bool popBackF(ListF* list) {
    if (list->head == nullptr) return false;

    if (list->head == list->tail) {
        delete list->head;
        list->head = list->tail = nullptr;
        return true;
    }

    NodeF* prev = list->head;
    while (prev->next != list->tail) prev = prev->next;

    delete list->tail;
    list->tail = prev;
    prev->next = nullptr;
    return true;
}

//удаление после элемента
bool removeAfterF(ListF* list, const string& target) {
    NodeF* targetNode = findNodeF(list, target);
    if (targetNode == nullptr || targetNode->next == nullptr) return false;

    NodeF* tmp = targetNode->next;
    targetNode->next = tmp->next;

    if (list->tail == tmp) list->tail = targetNode;
    delete tmp;
    return true;
}

//удаление перед элементом
bool removeBeforeF(ListF* list, const string& target) {
    if (list->head == nullptr || list->head->data == target) return false;

    if (list->head->next != nullptr && list->head->next->data == target)
        return popFrontF(list);

    NodeF* prev = list->head;
    while (prev->next != nullptr && prev->next->next != nullptr &&
           prev->next->next->data != target)
        prev = prev->next;

    if (prev->next == nullptr || prev->next->next == nullptr) return false;

    NodeF* tmp = prev->next;
    prev->next = tmp->next;
    delete tmp;
    return true;
}

//удаление по значению
bool removeByValueF(ListF* list, const string& value) {
    if (list->head == nullptr) return false;

    if (list->head->data == value) return popFrontF(list);

    NodeF* prev = list->head;
    while (prev->next != nullptr && prev->next->data != value)
        prev = prev->next;

    if (prev->next == nullptr) return false;

    NodeF* tmp = prev->next;
    prev->next = tmp->next;
    if (list->tail == tmp) list->tail = prev;
    delete tmp;
    return true;
}

//поиск элемента возвращает индекс
int searchF(const ListF* list, const string& value) {
    int index = 0;
    for (NodeF* cur = list->head; cur != nullptr; cur = cur->next, ++index)
        if (cur->data == value) return index;
    return -1;
}

//Чтение 3 способа:

//обычный проход по элементам
void printForwardF(const ListF* list) {
    if (isEmptyF(list)) {
        cout << "Список пуст" << endl;
        return;
    }
    for (NodeF* cur = list->head; cur != nullptr; cur = cur->next)
        cout << cur->data << " -> ";
    cout << "nullptr" << endl;
}

//обратный вывод
void printBackwardF(const ListF* list) {
    if (isEmptyF(list)) {
        cout << "Список пуст" << endl;
        return;
    }
    printBackwardRecF(list->head);
    cout << endl;
}

//вывод с индексом номеров
void printIndexedF(const ListF* list) {
    if (isEmptyF(list)) {
        cout << "Список пуст" << endl;
        return;
    }
    int i = 0;
    for (NodeF* cur = list->head; cur != nullptr; cur = cur->next, ++i)
        cout << "[" << i << "] " << cur->data << endl;
}

//удаление всего
void destroyListF(ListF* list) {
    while (list->head != nullptr)
        popFrontF(list);
}

bool saveListFToFile(const ListF* list, const char* filename) {
    ofstream out(filename);
    if (!out) return false;

    for (NodeF* cur = list->head; cur != nullptr; cur = cur->next) {
        out << cur->data << " ";
    }
    out << "\n";
    return true;
}

bool loadListFFromFile(ListF* list, const char* filename) {
    ifstream in(filename);
    if (!in) return false;

    destroyListF(list); // Очищаем старый список перед загрузкой

    string val;                 
    while (in >> val) {         
        pushBackF(list, val);   // Загружаем элементы в хвост, сохраняя порядок
    }
    return true;
}
