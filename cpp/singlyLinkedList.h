#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

#include <string>

// Структура узла односвязного списка
struct NodeF {
    std::string data;
    NodeF* next;
    NodeF(const std::string& d) { data = d; next = nullptr; }
};

// Структура самого списка
struct ListF {
    NodeF* head;
    NodeF* tail;
};

void initListF(ListF* list);
bool isEmptyF(const ListF* list);
int sizeF(const ListF* list);
NodeF* findNodeF(const ListF* list, const std::string& value);
NodeF* getAtF(const ListF* list, int index);

void pushFrontF(ListF* list, const std::string& value);
void pushBackF(ListF* list, const std::string& value);
bool insertAfterF(ListF* list, const std::string& target, const std::string& value);
bool insertBeforeF(ListF* list, const std::string& target, const std::string& value);

bool popFrontF(ListF* list);
bool popBackF(ListF* list);
bool removeAfterF(ListF* list, const std::string& target);
bool removeBeforeF(ListF* list, const std::string& target);

bool removeByValueF(ListF* list, const std::string& value);
int searchF(const ListF* list, const std::string& value);

void printForwardF(const ListF* list);
void printBackwardF(const ListF* list);
void printIndexedF(const ListF* list);

void destroyListF(ListF* list);

bool saveListFToFile(const ListF* list, const char* filename);
bool loadListFFromFile(ListF* list, const char* filename);

#endif // SINGLYLINKEDLIST_H
