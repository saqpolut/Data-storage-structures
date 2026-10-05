#include "queue.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void initQueue(Queue* queue) {
    queue->head = nullptr;
    queue->tail = nullptr;
    queue->count = 0;
}

bool isQueueEmpty(const Queue* queue) {
    return queue->head == nullptr;
}

int sizeQueue(const Queue* queue) {
    return queue->count;
}

// добавление в хвост
void pushQueue(Queue* queue, const string& value) {
    NodeQ* node = new NodeQ(value);
    if (queue->tail == nullptr) {
        queue->head = queue->tail = node;   // очередь была пуста
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
    ++queue->count;
}

// извлечение из головы (FIFO): элемент возвращается через outValue
bool popQueue(Queue* queue, string& outValue) {
    if (isQueueEmpty(queue)) {
        return false;
    }

    NodeQ* old_head = queue->head;
    outValue = old_head->data;

    queue->head = old_head->next;
    if (queue->head == nullptr) {
        queue->tail = nullptr;              // убрали единственный элемент
    }

    delete old_head;
    --queue->count;
    return true;
}

// первый элемент без извлечения
bool peekQueue(const Queue* queue, string& outValue) {
    if (isQueueEmpty(queue)) {
        return false;
    }
    outValue = queue->head->data;
    return true;
}

void printQueue(const Queue* queue) {
    if (isQueueEmpty(queue)) {
        cout << "очередь пуста" << endl;
        return;
    }
    for (NodeQ* cur = queue->head; cur != nullptr; cur = cur->next) {
        cout << cur->data << " ";
    }
    cout << endl;
}

void destroyQueue(Queue* queue) {
    string dummy;
    while (popQueue(queue, dummy)) {}
}

// Сохранение: порядок «голова -> хвост» совпадает с порядком добавления,
// поэтому разворачивать ничего не нужно (в отличие от стека).
bool saveQueueToFile(const Queue* queue, const char* filename) {
    ofstream out(filename);
    if (!out) return false;

    for (NodeQ* cur = queue->head; cur != nullptr; cur = cur->next) {
        out << cur->data << "\n";
    }
    return true;
}

bool loadQueueFromFile(Queue* queue, const char* filename) {
    ifstream in(filename);
    if (!in) return false;

    destroyQueue(queue);   // очищаем текущую очередь перед загрузкой

    string val;
    // Читаем в том же порядке, в каком записали, и добавляем в хвост:
    // первый прочитанный снова станет головой.
    while (in >> val) {
        pushQueue(queue, val);
    }
    return true;
}
