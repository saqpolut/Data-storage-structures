#include "doublyLinkedList.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void initListD(ListD* list) {
    list->head = nullptr;
    list->tail = nullptr;
}

bool isEmptyD(const ListD* list) {
    return list->head == nullptr;
}

int sizeD(const ListD* list) {
    int count = 0;
    for (NodeD* cur = list->head; cur != nullptr; cur = cur->next) {
        ++count;
    }
    return count;
}

NodeD* findNodeD(const ListD* list, const string& value) {
    for (NodeD* cur = list->head; cur != nullptr; cur = cur->next) {
        if (cur->data == value) return cur;
    }
    return nullptr;
}

NodeD* getAtD(const ListD* list, int index) {
    if (index < 0) return nullptr;
    NodeD* cur = list->head;
    for (int i = 0; cur != nullptr && i < index; ++i) {
        cur = cur->next;
    }
    return cur;
}

//ДОБАВЛЕНИЕ

void pushFrontD(ListD* list, const string& value) {
    NodeD* node = new NodeD(value);
    node->next = list->head;
    if (list->head != nullptr) {
        list->head->prev = node;
    } else {
        list->tail = node; // Список был пуст
    }
    list->head = node;
}

void pushBackD(ListD* list, const string& value) {
    NodeD* node = new NodeD(value);
    node->prev = list->tail;
    if (list->tail != nullptr) {
        list->tail->next = node;
    } else {
        list->head = node; // Список был пуст
    }
    list->tail = node;
}

bool insertAfterD(ListD* list, const string& target, const string& value) {
    NodeD* t = findNodeD(list, target);
    if (t == nullptr) return false;

    NodeD* node = new NodeD(value);
    node->next = t->next;
    node->prev = t;

    if (t->next != nullptr) {
        t->next->prev = node;
    } else {
        list->tail = node; // Вставили после последнего
    }
    t->next = node;
    return true;
}

bool insertBeforeD(ListD* list, const string& target, const string& value) {
    NodeD* t = findNodeD(list, target);
    if (t == nullptr) return false;

    NodeD* node = new NodeD(value);
    node->prev = t->prev;
    node->next = t;

    if (t->prev != nullptr) {
        t->prev->next = node;
    } else {
        list->head = node; // Вставили перед первым
    }
    t->prev = node;
    return true;
}

//УДАЛЕНИЕ

bool popFrontD(ListD* list) {
    if (list->head == nullptr) return false;

    NodeD* tmp = list->head;
    list->head = tmp->next;

    if (list->head != nullptr) {
        list->head->prev = nullptr;
    } else {
        list->tail = nullptr; // Был единственный элемент
    }
    delete tmp;
    return true;
}

bool popBackD(ListD* list) {
    if (list->tail == nullptr) return false;

    NodeD* tmp = list->tail;
    list->tail = tmp->prev;

    if (list->tail != nullptr) {
        list->tail->next = nullptr;
    } else {
        list->head = nullptr; // Был единственный элемент
    }
    delete tmp;
    return true;
}

bool removeAfterD(ListD* list, const string& target) {
    NodeD* t = findNodeD(list, target);
    if (t == nullptr || t->next == nullptr) return false;

    NodeD* tmp = t->next;
    t->next = tmp->next;

    if (tmp->next != nullptr) {
        tmp->next->prev = t;
    } else {
        list->tail = t; // Удаляли последний элемент
    }
    delete tmp;
    return true;
}

bool removeBeforeD(ListD* list, const string& target) {
    NodeD* t = findNodeD(list, target);
    if (t == nullptr || t->prev == nullptr) return false; // Перед головой ничего нет

    NodeD* tmp = t->prev;
    t->prev = tmp->prev;

    if (tmp->prev != nullptr) {
        tmp->prev->next = t;
    } else {
        list->head = t; // Удаляли голову
    }
    delete tmp;
    return true;
}

//ПО ЗНАЧЕНИЮ

bool removeByValueD(ListD* list, const string& value) {
    NodeD* t = findNodeD(list, value);
    if (t == nullptr) return false;

    if (t->prev != nullptr) {
        t->prev->next = t->next;
    } else {
        list->head = t->next;
    }

    if (t->next != nullptr) {
        t->next->prev = t->prev;
    } else {
        list->tail = t->prev;
    }
    delete t;
    return true;
}

int searchD(const ListD* list, const string& value) {
    int index = 0;
    for (NodeD* cur = list->head; cur != nullptr; cur = cur->next, ++index) {
        if (cur->data == value) return index;
    }
    return -1;
}

//ЧТЕНИЕ

void printForwardD(const ListD* list) {
    if (isEmptyD(list)) {
        cout << "Список пуст" << endl;
        return;
    }
    for (NodeD* cur = list->head; cur != nullptr; cur = cur->next) {
        cout << cur->data << " <-> ";
    }
    cout << "nullptr" << endl;
}

void printBackwardD(const ListD* list) {
    if (isEmptyD(list)) {
        cout << "Список пуст" << endl;
        return;
    }
    for (NodeD* cur = list->tail; cur != nullptr; cur = cur->prev) {
        cout << cur->data << " <-> ";
    }
    cout << "nullptr" << endl;
}

void printIndexedD(const ListD* list) {
    if (isEmptyD(list)) {
        cout << "Список пуст" << endl;
        return;
    }
    int i = 0;
    for (NodeD* cur = list->head; cur != nullptr; cur = cur->next, ++i) {
        cout << "[" << i << "] " << cur->data << endl;
    }
}

//ОЧИСТКА
void destroyListD(ListD* list) {
    while (list->head != nullptr) {
        popFrontD(list);
    }
}

//ФАЙЛОВЫЕ ОПЕРАЦИИ
bool saveListDToFile(const ListD* list, const char* filename) {
    ofstream out(filename);
    if (!out) return false;

    for (NodeD* cur = list->head; cur != nullptr; cur = cur->next) {
        out << cur->data << " ";
    }
    out << "\n";
    return true;
}

bool loadListDFromFile(ListD* list, const char* filename) {
    ifstream in(filename);
    if (!in) return false;

    destroyListD(list); // Очищаем старый список перед загрузкой

    string val;               
    while (in >> val) {         
        pushBackD(list, val);   // Загружаем элементы в хвост, сохраняя порядок
    }
    return true;
}
