#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <string>

struct NodeD {
    std::string data;
    NodeD* prev;
    NodeD* next;

    NodeD(const std::string& d) { data = d; prev = nullptr; next = nullptr; }
};

struct ListD {
    NodeD* head;
    NodeD* tail;
};

void initListD(ListD* list);
bool isEmptyD(const ListD* list);
int sizeD(const ListD* list);
NodeD* findNodeD(const ListD* list, const std::string& value);
NodeD* getAtD(const ListD* list, int index);

void pushFrontD(ListD* list, const std::string& value);
void pushBackD(ListD* list, const std::string& value);
bool insertAfterD(ListD* list, const std::string& target, const std::string& value);
bool insertBeforeD(ListD* list, const std::string& target, const std::string& value);

bool popFrontD(ListD* list);
bool popBackD(ListD* list);
bool removeAfterD(ListD* list, const std::string& target);
bool removeBeforeD(ListD* list, const std::string& target);

bool removeByValueD(ListD* list, const std::string& value);
int searchD(const ListD* list, const std::string& value);

void printForwardD(const ListD* list);
void printBackwardD(const ListD* list);
void printIndexedD(const ListD* list);

void destroyListD(ListD* list);

bool saveListDToFile(const ListD* list, const char* filename);
bool loadListDFromFile(ListD* list, const char* filename);

#endif //DOUBLYLINKEDLIST_H
