#include <iostream>
#include <string>
#include <limits>

#include "massiv.h"
#include "singlyLinkedList.h"
#include "doublyLinkedList.h"
#include "stack.h"
#include "queue.h"
#include "AVL_tree.h"
#include "dbms.h"

using namespace std;

const char* FILE_ARRAY  = "array_data.txt";
const char* FILE_LIST_F = "list_f_data.txt";
const char* FILE_LIST_D = "list_d_data.txt";
const char* FILE_STACK  = "stack_data.txt";
const char* FILE_QUEUE  = "queue_data.txt";
const char* FILE_TREE   = "tree_data.txt";

//Ввод

// Читает целое число (индексы, пункты меню и ключи дерева)
int readInt(const string& prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x) return x;
        if (cin.eof()) return 0;                 // ввод закрыт: считаем "выход"
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Нужно ввести целое число.\n";
    }
}

// Читает ОДНО слово-значение (данные строковых структур)
string readWord(const string& prompt) {
    string s;
    while (true) {
        cout << prompt;
        if (cin >> s) return s;
        if (cin.eof()) return "";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Нужно ввести значение.\n";
    }
}

// Читает имя файла (можно с пробелами)
string readName(const string& prompt) {
    string name;
    cout << prompt;
    cin >> ws;
    getline(cin, name);
    return name;
}

//Ввод состояния

void showArray(const Array& arr) {
    cout << "Массив (" << getLength(arr) << " эл.): ";
    printArray(arr);
}

void showListF(const ListF& list) {
    cout << "Односвязный список (" << sizeF(&list) << " эл.): ";
    printForwardF(&list);
}

void showListD(const ListD& list) {
    cout << "Двусвязный список (" << sizeD(&list) << " эл.): ";
    printForwardD(&list);
}

void showStack(const Stack& stack) {
    cout << "Стек (вершина слева): ";
    printStack(&stack);
}

void showQueue(const Queue& queue) {
    cout << "Очередь (" << sizeQueue(&queue) << " эл.): ";
    printQueue(&queue);
}

void showTree(NodeT* root) {
    cout << "Дерево AVL\n";
    cout << "По возрастанию: ";
    if (!root) cout << "(дерево пусто)";
    else printInOrderT(root);
    cout << "\nВысота: " << (int)getHeightT(root)
         << ", узлов: " << countNodesT(root) << "\n";
    if (root) {
        cout << "Структура (повёрнуто на 90 градусов):\n";
        printTreeT(root, 0);
    }
}

//Массив

void menuArray(Array& arr) {
    while (true) {
        cout << "\nМассив (M):\n"
             << "1. MPUSH     - Добавить в конец\n"
             << "2. MINSERT   - Вставить по индексу\n"
             << "3. MGET      - Получить по индексу\n"
             << "4. MSET      - Заменить по индексу\n"
             << "5. MDEL      - Удалить по индексу\n"
             << "6. MLEN      - Длина массива\n"
             << "7. MREAD     - Ввести массив с клавиатуры\n"
             << "8. MLOAD     - Загрузить из файла\n"
             << "9. MSAVE_AS  - Сохранить в указанный файл\n"
             << "10. MLOAD_AS - Загрузить из указанного файла\n"
             << "0. Назад\n";
        int c = readInt("Выбор: ");
        if (c == 0) return;

        bool changed = false;
        switch (c) {
        case 1: {
            string v = readWord("Значение: ");
            if (addToEnd(arr, v)) { cout << "Добавлено в конец: " << v << "\n"; changed = true; }
            else cout << "Ошибка добавления.\n";
            break;
        }
        case 2: {
            int i = readInt("Индекс: ");
            string v = readWord("Значение: ");
            if (i >= 0 && addAtIndex(arr, (size_t)i, v)) {
                cout << "Вставлено по индексу " << i << ": " << v << "\n"; changed = true;
            } else cout << "Некорректный индекс.\n";
            break;
        }
        case 3: {
            int i = readInt("Индекс: ");
            string v;
            if (i >= 0 && getElement(arr, (size_t)i, v)) cout << "Элемент [" << i << "] = " << v << "\n";
            else cout << "Некорректный индекс.\n";
            break;
        }
        case 4: {
            int i = readInt("Индекс: ");
            string v = readWord("Новое значение: ");
            if (i >= 0 && replaceAtIndex(arr, (size_t)i, v)) {
                cout << "Элемент [" << i << "] заменён на " << v << "\n"; changed = true;
            } else cout << "Некорректный индекс.\n";
            break;
        }
        case 5: {
            int i = readInt("Индекс: ");
            if (i >= 0 && removeAtIndex(arr, (size_t)i)) {
                cout << "Удалён элемент [" << i << "]\n"; changed = true;
            } else cout << "Некорректный индекс.\n";
            break;
        }
        case 6:
            cout << "Длина массива: " << getLength(arr) << "\n";
            break;
        case 7:
            if (readArray(arr)) { cout << "Массив введён.\n"; changed = true; }
            else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка ввода, массив не изменён.\n";
            }
            break;
        case 8:
            if (loadArrayFromFile(arr, FILE_ARRAY)) cout << "Загружено из " << FILE_ARRAY << "\n";
            else cout << "Не удалось загрузить " << FILE_ARRAY << "\n";
            break;
        case 9: {
            string name = readName("Имя файла: ");
            if (saveArrayToFile(arr, name.c_str())) cout << "Сохранено в " << name << "\n";
            else cout << "Не удалось открыть файл " << name << "\n";
            break;
        }
        case 10: {
            string name = readName("Имя файла: ");
            if (loadArrayFromFile(arr, name.c_str())) { cout << "Загружено из " << name << "\n"; changed = true; }
            else cout << "Не удалось загрузить " << name << "\n";
            break;
        }
        default:
            cout << "Нет такого пункта.\n";
            continue;
        }

        if (changed) saveArrayToFile(arr, FILE_ARRAY);
        cout << "\nТекущее состояние: ";
        showArray(arr);
    }
}

//Односвязный список

void menuListF(ListF& list) {
    while (true) {
        cout << "\nОдносвязный список (F):\n"
             << "1. FPUSH_HEAD   - Добавить в голову\n"
             << "2. FPUSH_TAIL   - Добавить в хвост\n"
             << "3. FPUSH_AFTER  - Добавить после значения\n"
             << "4. FPUSH_BEFORE - Добавить перед значением\n"
             << "5. FDEL_HEAD    - Удалить из головы\n"
             << "6. FDEL_TAIL    - Удалить из хвоста\n"
             << "7. FDEL_AFTER   - Удалить после значения\n"
             << "8. FDEL_BEFORE  - Удалить перед значением\n"
             << "9. FDEL_VALUE   - Удалить по значению\n"
             << "10. FSEARCH     - Поиск по значению\n"
             << "11. FGET        - Элемент по индексу\n"
             << "12. FPRINT      - Чтение от головы к хвосту\n"
             << "13. FPRINT_BACK - Чтение от хвоста к голове\n"
             << "14. FPRINT_IDX  - Чтение с индексами\n"
             << "15. FLOAD       - Загрузить из файла\n"
             << "16. FSAVE_AS    - Сохранить в указанный файл\n"
             << "17. FLOAD_AS    - Загрузить из указанного файла\n"
             << "0. Назад\n";
        int c = readInt("Выбор: ");
        if (c == 0) return;

        bool changed = false;
        switch (c) {
        case 1: {
            string v = readWord("Значение: ");
            pushFrontF(&list, v);
            cout << "Добавлено в голову: " << v << "\n"; changed = true;
            break;
        }
        case 2: {
            string v = readWord("Значение: ");
            pushBackF(&list, v);
            cout << "Добавлено в хвост: " << v << "\n"; changed = true;
            break;
        }
        case 3: {
            string t = readWord("После какого значения: ");
            string v = readWord("Новое значение: ");
            if (insertAfterF(&list, t, v)) { cout << "Вставлено " << v << " после " << t << "\n"; changed = true; }
            else cout << "Значение " << t << " не найдено.\n";
            break;
        }
        case 4: {
            string t = readWord("Перед каким значением: ");
            string v = readWord("Новое значение: ");
            if (insertBeforeF(&list, t, v)) { cout << "Вставлено " << v << " перед " << t << "\n"; changed = true; }
            else cout << "Значение " << t << " не найдено.\n";
            break;
        }
        case 5:
            if (popFrontF(&list)) { cout << "Удалён элемент из головы.\n"; changed = true; }
            else cout << "Список пуст.\n";
            break;
        case 6:
            if (popBackF(&list)) { cout << "Удалён элемент из хвоста.\n"; changed = true; }
            else cout << "Список пуст.\n";
            break;
        case 7: {
            string t = readWord("После какого значения удалить: ");
            if (removeAfterF(&list, t)) { cout << "Удалён элемент после " << t << "\n"; changed = true; }
            else cout << "Не удалось: значения нет или после него ничего нет.\n";
            break;
        }
        case 8: {
            string t = readWord("Перед каким значением удалить: ");
            if (removeBeforeF(&list, t)) { cout << "Удалён элемент перед " << t << "\n"; changed = true; }
            else cout << "Не удалось: значения нет или оно первое.\n";
            break;
        }
        case 9: {
            string v = readWord("Какое значение удалить: ");
            if (removeByValueF(&list, v)) { cout << "Удалено значение " << v << "\n"; changed = true; }
            else cout << "Значение " << v << " не найдено.\n";
            break;
        }
        case 10: {
            string v = readWord("Что искать: ");
            int pos = searchF(&list, v);              // pos — индекс, остаётся int
            if (pos != -1) cout << "Значение " << v << " найдено на позиции " << pos << "\n";
            else cout << "Значение " << v << " не найдено.\n";
            break;
        }
        case 11: {
            int i = readInt("Индекс: ");
            NodeF* node = getAtF(&list, i);
            if (node) cout << "Элемент [" << i << "] = " << node->data << "\n";
            else cout << "Некорректный индекс.\n";
            break;
        }
        case 12:
            cout << "От головы к хвосту: ";
            printForwardF(&list);
            break;
        case 13:
            cout << "От хвоста к голове: ";
            printBackwardF(&list);
            break;
        case 14:
            cout << "С индексами:\n";
            printIndexedF(&list);
            break;
        case 15:
            if (loadListFFromFile(&list, FILE_LIST_F)) cout << "Загружено из " << FILE_LIST_F << "\n";
            else cout << "Не удалось загрузить " << FILE_LIST_F << "\n";
            break;
        case 16: {
            string name = readName("Имя файла: ");
            if (saveListFToFile(&list, name.c_str())) cout << "Сохранено в " << name << "\n";
            else cout << "Не удалось открыть файл " << name << "\n";
            break;
        }
        case 17: {
            string name = readName("Имя файла: ");
            if (loadListFFromFile(&list, name.c_str())) { cout << "Загружено из " << name << "\n"; changed = true; }
            else cout << "Не удалось загрузить " << name << "\n";
            break;
        }
        default:
            cout << "Нет такого пункта.\n";
            continue;
        }

        if (changed) saveListFToFile(&list, FILE_LIST_F);
        cout << "\nТекущее состояние: ";
        showListF(list);
    }
}

//Двусвязный список
void menuListD(ListD& list) {
    while (true) {
        cout << "\nДвусвязный список (L):\n"
             << "1. LPUSH_HEAD   - Добавить в голову\n"
             << "2. LPUSH_TAIL   - Добавить в хвост\n"
             << "3. LPUSH_AFTER  - Добавить после значения\n"
             << "4. LPUSH_BEFORE - Добавить перед значением\n"
             << "5. LDEL_HEAD    - Удалить из головы\n"
             << "6. LDEL_TAIL    - Удалить из хвоста\n"
             << "7. LDEL_AFTER   - Удалить после значения\n"
             << "8. LDEL_BEFORE  - Удалить перед значением\n"
             << "9. LDEL_VALUE   - Удалить по значению\n"
             << "10. LSEARCH     - Поиск по значению\n"
             << "11. LGET        - Элемент по индексу\n"
             << "12. LPRINT      - Чтение от головы к хвосту\n"
             << "13. LPRINT_BACK - Чтение от хвоста к голове\n"
             << "14. LPRINT_IDX  - Чтение с индексами\n"
             << "15. LLOAD       - Загрузить из файла\n"
             << "16. LSAVE_AS    - Сохранить в указанный файл\n"
             << "17. LLOAD_AS    - Загрузить из указанного файла\n"
             << "0. Назад\n";
        int c = readInt("Выбор: ");
        if (c == 0) return;

        bool changed = false;
        switch (c) {
        case 1: {
            string v = readWord("Значение: ");
            pushFrontD(&list, v);
            cout << "Добавлено в голову: " << v << "\n"; changed = true;
            break;
        }
        case 2: {
            string v = readWord("Значение: ");
            pushBackD(&list, v);
            cout << "Добавлено в хвост: " << v << "\n"; changed = true;
            break;
        }
        case 3: {
            string t = readWord("После какого значения: ");
            string v = readWord("Новое значение: ");
            if (insertAfterD(&list, t, v)) { cout << "Вставлено " << v << " после " << t << "\n"; changed = true; }
            else cout << "Значение " << t << " не найдено.\n";
            break;
        }
        case 4: {
            string t = readWord("Перед каким значением: ");
            string v = readWord("Новое значение: ");
            if (insertBeforeD(&list, t, v)) { cout << "Вставлено " << v << " перед " << t << "\n"; changed = true; }
            else cout << "Значение " << t << " не найдено.\n";
            break;
        }
        case 5:
            if (popFrontD(&list)) { cout << "Удалён элемент из головы.\n"; changed = true; }
            else cout << "Список пуст.\n";
            break;
        case 6:
            if (popBackD(&list)) { cout << "Удалён элемент из хвоста.\n"; changed = true; }
            else cout << "Список пуст.\n";
            break;
        case 7: {
            string t = readWord("После какого значения удалить: ");
            if (removeAfterD(&list, t)) { cout << "Удалён элемент после " << t << "\n"; changed = true; }
            else cout << "Не удалось: значения нет или после него ничего нет.\n";
            break;
        }
        case 8: {
            string t = readWord("Перед каким значением удалить: ");
            if (removeBeforeD(&list, t)) { cout << "Удалён элемент перед " << t << "\n"; changed = true; }
            else cout << "Не удалось: значения нет или оно первое.\n";
            break;
        }
        case 9: {
            string v = readWord("Какое значение удалить: ");
            if (removeByValueD(&list, v)) { cout << "Удалено значение " << v << "\n"; changed = true; }
            else cout << "Значение " << v << " не найдено.\n";
            break;
        }
        case 10: {
            string v = readWord("Что искать: ");
            int pos = searchD(&list, v);              // pos — индекс, остаётся int
            if (pos != -1) cout << "Значение " << v << " найдено на позиции " << pos << "\n";
            else cout << "Значение " << v << " не найдено.\n";
            break;
        }
        case 11: {
            int i = readInt("Индекс: ");
            NodeD* node = getAtD(&list, i);
            if (node) cout << "Элемент [" << i << "] = " << node->data << "\n";
            else cout << "Некорректный индекс.\n";
            break;
        }
        case 12:
            cout << "От головы к хвосту: ";
            printForwardD(&list);
            break;
        case 13:
            cout << "От хвоста к голове: ";
            printBackwardD(&list);
            break;
        case 14:
            cout << "С индексами:\n";
            printIndexedD(&list);
            break;
        case 15:
            if (loadListDFromFile(&list, FILE_LIST_D)) cout << "Загружено из " << FILE_LIST_D << "\n";
            else cout << "Не удалось загрузить " << FILE_LIST_D << "\n";
            break;
        case 16: {
            string name = readName("Имя файла: ");
            if (saveListDToFile(&list, name.c_str())) cout << "Сохранено в " << name << "\n";
            else cout << "Не удалось открыть файл " << name << "\n";
            break;
        }
        case 17: {
            string name = readName("Имя файла: ");
            if (loadListDFromFile(&list, name.c_str())) { cout << "Загружено из " << name << "\n"; changed = true; }
            else cout << "Не удалось загрузить " << name << "\n";
            break;
        }
        default:
            cout << "Нет такого пункта.\n";
            continue;
        }

        if (changed) saveListDToFile(&list, FILE_LIST_D);
        cout << "\nТекущее состояние: ";
        showListD(list);
    }
}

//Стек
void menuStack(Stack& stack) {
    while (true) {
        cout << "\nСтек (S):\n"
             << "1. SPUSH    - Добавить на вершину\n"
             << "2. SPOP     - Снять с вершины\n"
             << "3. SGET     - Прочитать вершину\n"
             << "4. SLOAD    - Загрузить из файла\n"
             << "5. SSAVE_AS - Сохранить в указанный файл\n"
             << "6. SLOAD_AS - Загрузить из указанного файла\n"
             << "0. Назад\n";
        int c = readInt("Выбор: ");
        if (c == 0) return;

        bool changed = false;
        switch (c) {
        case 1: {
            string v = readWord("Значение: ");
            pushStack(&stack, v);
            cout << "Добавлено на вершину: " << v << "\n"; changed = true;
            break;
        }
        case 2: {
            string top;
            if (popStack(&stack, top)) {
                cout << "Снят элемент с вершины: " << top << "\n"; changed = true;
            } else cout << "Стек пуст.\n";
            break;
        }
        case 3: {
            string top;
            if (peekStack(&stack, top)) cout << "Вершина: " << top << "\n";
            else cout << "Стек пуст.\n";
            break;
        }
        case 4:
            if (loadStackFromFile(&stack, FILE_STACK)) cout << "Загружено из " << FILE_STACK << "\n";
            else cout << "Не удалось загрузить " << FILE_STACK << "\n";
            break;
        case 5: {
            string name = readName("Имя файла: ");
            if (saveStackToFile(&stack, name.c_str())) cout << "Сохранено в " << name << "\n";
            else cout << "Не удалось открыть файл " << name << "\n";
            break;
        }
        case 6: {
            string name = readName("Имя файла: ");
            if (loadStackFromFile(&stack, name.c_str())) { cout << "Загружено из " << name << "\n"; changed = true; }
            else cout << "Не удалось загрузить " << name << "\n";
            break;
        }
        default:
            cout << "Нет такого пункта.\n";
            continue;
        }

        if (changed) saveStackToFile(&stack, FILE_STACK);
        cout << "\nТекущее состояние: ";
        showStack(stack);
    }
}

//Очередь
void menuQueue(Queue& queue) {
    while (true) {
        cout << "\nОчередь (Q):\n"
             << "1. QPUSH    - Добавить в конец\n"
             << "2. QPOP     - Извлечь из начала\n"
             << "3. QGET     - Прочитать первый\n"
             << "4. QLOAD    - Загрузить из файла\n"
             << "5. QSAVE_AS - Сохранить в указанный файл\n"
             << "6. QLOAD_AS - Загрузить из указанного файла\n"
             << "0. Назад\n";
        int c = readInt("Выбор: ");
        if (c == 0) return;

        bool changed = false;
        switch (c) {
        case 1: {
            string v = readWord("Значение: ");
            pushQueue(&queue, v);
            cout << "Добавлено в конец: " << v << "\n"; changed = true;
            break;
        }
        case 2: {
            string v;
            if (popQueue(&queue, v)) { cout << "Извлечён элемент: " << v << "\n"; changed = true; }
            else cout << "Очередь пуста.\n";
            break;
        }
        case 3: {
            string v;
            if (peekQueue(&queue, v)) cout << "Первый на выход: " << v << "\n";
            else cout << "Очередь пуста.\n";
            break;
        }
        case 4:
            if (loadQueueFromFile(&queue, FILE_QUEUE)) cout << "Загружено из " << FILE_QUEUE << "\n";
            else cout << "Не удалось загрузить " << FILE_QUEUE << "\n";
            break;
        case 5: {
            string name = readName("Имя файла: ");
            if (saveQueueToFile(&queue, name.c_str())) cout << "Сохранено в " << name << "\n";
            else cout << "Не удалось открыть файл " << name << "\n";
            break;
        }
        case 6: {
            string name = readName("Имя файла: ");
            if (loadQueueFromFile(&queue, name.c_str())) { cout << "Загружено из " << name << "\n"; changed = true; }
            else cout << "Не удалось загрузить " << name << "\n";
            break;
        }
        default:
            cout << "Нет такого пункта.\n";
            continue;
        }

        if (changed) saveQueueToFile(&queue, FILE_QUEUE);
        cout << "\nТекущее состояние: ";
        showQueue(queue);
    }
}

//Дерево
void menuTree(NodeT*& root) {
    while (true) {
        cout << "\nДерево AVL (T):\n"
             << "1. TINSERT  - Вставить\n"
             << "2. TDEL     - Удалить\n"
             << "3. TGET     - Найти (с числом шагов)\n"
             << "4. TINFO    - Обходы, высота, структура\n"
             << "5. TLOAD    - Загрузить из файла\n"
             << "6. TSAVE_AS - Сохранить в указанный файл\n"
             << "7. TLOAD_AS - Загрузить из указанного файла\n"
             << "0. Назад\n";
        int c = readInt("Выбор: ");
        if (c == 0) return;

        bool changed = false;
        switch (c) {
        case 1: {
            int v = readInt("Значение: ");
            if (findT(root, v)) cout << "Значение " << v << " уже есть, дубликаты не добавляются.\n";
            else { root = insertT(root, v); cout << "Вставлено: " << v << "\n"; changed = true; }
            break;
        }
        case 2: {
            int v = readInt("Значение: ");
            if (findT(root, v)) { root = removeT(root, v); cout << "Удалено: " << v << "\n"; changed = true; }
            else cout << "Значение " << v << " не найдено.\n";
            break;
        }
        case 3: {
            int v = readInt("Значение: ");
            int steps = 0;                          // steps — число, остаётся int
            if (findWithStepsT(root, v, steps))
                cout << "Значение " << v << " найдено. Просмотрено узлов: " << steps << "\n";
            else
                cout << "Значение " << v << " не найдено. Просмотрено узлов: " << steps << "\n";
            break;
        }
        case 4:
            printTreeInfoT(root, "Информация о дереве AVL:");
            continue;                       // полный вывод уже напечатан
        case 5:
            if (loadTreeFromFile(root, FILE_TREE)) cout << "Загружено из " << FILE_TREE << "\n";
            else cout << "Не удалось загрузить " << FILE_TREE << "\n";
            break;
        case 6: {
            string name = readName("Имя файла: ");
            if (saveTreeToFile(root, name.c_str())) cout << "Сохранено в " << name << "\n";
            else cout << "Не удалось открыть файл " << name << "\n";
            break;
        }
        case 7: {
            string name = readName("Имя файла: ");
            if (loadTreeFromFile(root, name.c_str())) { cout << "Загружено из " << name << "\n"; changed = true; }
            else cout << "Не удалось загрузить " << name << "\n";
            break;
        }
        default:
            cout << "Нет такого пункта.\n";
            continue;
        }

        if (changed) saveTreeToFile(root, FILE_TREE);
        cout << "\nТекущее состояние: ";
        showTree(root);
    }
}

//Главная функция
int main(int argc, char* argv[]) {
    // Режим запросов: dbms --file <файл> --query '<команда>'
    string file, query;

    //значение идёт сразу за своим ключом
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc){
            file  = argv[++i];
        }
        else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
    }
    //оба параметра обязательны
    if (argc > 1) {
        if (file.empty() || query.empty()) {
            cout << "Использование: dbms --file <файл> --query '<команда>'\n";
            return 1;
        }
        return runQuery(file, query);
    }

    Array arr;      create(arr);
    ListF listF;    initListF(&listF);
    ListD listD;    initListD(&listD);
    Stack stack;    initStack(&stack);
    Queue queue;    initQueue(&queue);
    NodeT* treeRoot = nullptr;

    // Автозагрузка: если файла нет, структура остаётся пустой
    const char* noFile = "файла нет, структура пуста";
    cout << "Загрузка сохранённых данных:\n";
    cout << "Массив: "             << (loadArrayFromFile(arr, FILE_ARRAY)       ? "загружен"  : noFile) << "\n";
    cout << "Односвязный список: " << (loadListFFromFile(&listF, FILE_LIST_F)   ? "загружен"  : noFile) << "\n";
    cout << "Двусвязный список: "  << (loadListDFromFile(&listD, FILE_LIST_D)   ? "загружен"  : noFile) << "\n";
    cout << "Стек: "               << (loadStackFromFile(&stack, FILE_STACK)    ? "загружен"  : noFile) << "\n";
    cout << "Очередь: "            << (loadQueueFromFile(&queue, FILE_QUEUE)    ? "загружена" : noFile) << "\n";
    cout << "Дерево AVL: "         << (loadTreeFromFile(treeRoot, FILE_TREE)    ? "загружено" : noFile) << "\n";

    bool running = true;
    while (running) {
        cout << "\nГлавное меню:\n"
             << "1. Массив (M)\n"
             << "2. Односвязный список (F)\n"
             << "3. Двусвязный список (L)\n"
             << "4. Стек (S)\n"
             << "5. Очередь (Q)\n"
             << "6. Дерево AVL (T)\n"
             << "7. Показать все структуры\n"
             << "0. Выход\n";
        int c = readInt("Выбор: ");

        switch (c) {
        case 1: menuArray(arr);       break;
        case 2: menuListF(listF);     break;
        case 3: menuListD(listD);     break;
        case 4: menuStack(stack);     break;
        case 5: menuQueue(queue);     break;
        case 6: menuTree(treeRoot);   break;
        case 7:
            cout << "\nТекущее состояние всех структур:\n";
            showArray(arr);
            showListF(listF);
            showListD(listD);
            showStack(stack);
            showQueue(queue);
            showTree(treeRoot);
            break;
        case 0:
            running = false;
            break;
        default:
            cout << "Нет такого пункта.\n";
        }
    }

    destroy(arr);
    destroyListF(&listF);
    destroyListD(&listD);
    destroyStack(&stack);
    destroyQueue(&queue);
    destroyTreeT(treeRoot);

    cout << "Память освобождена. Программа завершена.\n";
    return 0;
}
