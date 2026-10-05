#ifndef QUEUE_H
#define QUEUE_H

#include <string>

// Структура узла очереди
struct NodeQ {
    std::string data;
    NodeQ* next;
    NodeQ(const std::string& d) { data = d; next = nullptr; }
};

// Структура самой очереди
struct Queue {
    NodeQ* head;  // Голова: откуда извлекаем (pop)
    NodeQ* tail;  // Хвост: куда добавляем (push)
    int count;    // Количество элементов для быстрого получения размера
};

void initQueue(Queue* queue);
bool isQueueEmpty(const Queue* queue);
int sizeQueue(const Queue* queue);

void pushQueue(Queue* queue, const std::string& value);        // Добавить в хвост
bool popQueue(Queue* queue, std::string& outValue);            // Извлечь из головы
bool peekQueue(const Queue* queue, std::string& outValue);     // Посмотреть первый, не извлекая

void printQueue(const Queue* queue);
void destroyQueue(Queue* queue);

bool saveQueueToFile(const Queue* queue, const char* filename);
bool loadQueueFromFile(Queue* queue, const char* filename);

#endif // QUEUE_H
