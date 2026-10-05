#include "dbms.h"
#include "massiv.h"
#include "singlyLinkedList.h"
#include "doublyLinkedList.h"
#include "stack.h"
#include "queue.h"
#include "AVL_tree.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

// Одна именованная структура из файла.
struct Entry {
    char   type;     // M, F, L, S, Q, T
    string name;
    Array  arr;
    ListF  listF;
    ListD  listD;
    Stack  stack;
    Queue  queue;
    NodeT* tree;
};

//создаёт контейнер, инициализирует все поля
static Entry* newEntry(char type, const string& name) {
    Entry* e = new Entry;
    e->type = type;
    e->name = name;
    create(e->arr);
    initListF(&e->listF);
    initListD(&e->listD);
    initStack(&e->stack);
    initQueue(&e->queue);
    e->tree = nullptr;
    return e;
}

//освобождает всё при выходе
static void freeAll(vector<Entry*>& db) {
    for (Entry* e : db) {
        destroy(e->arr);
        destroyListF(&e->listF);
        destroyListD(&e->listD);
        destroyStack(&e->stack);
        destroyQueue(&e->queue);
        destroyTreeT(e->tree);
        delete e;
    }
    db.clear();
}

//ищет контейнер по имени
static Entry* findEntry(vector<Entry*>& db, const string& name) {
    for (Entry* e : db)
        if (e->name == name) return e;
    return nullptr;
}

// Разбор ИНДЕКСА (позиции): это по-прежнему число.
// Индексы — единственное место, где нужна проверка на целое.
static bool parseIndex(const string& s, size_t& out) {
    try {
        size_t pos;
        long v = stol(s, &pos);
        if (pos != s.size() || v < 0) return false;
        out = (size_t)v;
        return true;
    } catch (const exception&) {
        return false;
    }
}

//для сообщений об ошибках
static int fail(const string& msg) {
    cout << "-> ERROR: " << msg << endl;
    return 1;
}

//проверяет, что аргументов хватает
static bool enough(const vector<string>& a, size_t k, const string& cmd) {
    if (a.size() >= k) return true;
    fail("для " + cmd + " нужно аргументов: " + to_string(k));
    return false;
}

// Строка должна быть int без исключений — для команд дерева
static bool toInt(const string& s, int& out) {
    try {
        size_t pos;
        out = stoi(s, &pos);
        return pos == s.size();
    } catch (const exception&) {
        return false;
    }
}

// Дерево в файле
// Ключ — любая строка, кроме зарезервированного маркера "#".
static NodeT* buildTree(istringstream& in, bool& ok) {
    string tok;
    if (!(in >> tok) || tok == "#") return nullptr;
    int key;
    if (!toInt(tok, key)) { ok = false; return nullptr; }
    NodeT* node = new NodeT(key);
    node->left  = buildTree(in, ok);
    node->right = buildTree(in, ok);
    fixHeightT(node);
    return node;
}


static void writeTree(NodeT* node, ostream& out) {
    if (!node) { out << " #"; return; }
    out << ' ' << node->key;
    writeTree(node->left, out);
    writeTree(node->right, out);
}

//Загрузка / сохранение
// Формат строки: <тип> <имя> <данные...>

static bool loadDB(const string& filename, vector<Entry*>& db) {
    ifstream in(filename);
    if (!in) return true;                    // файла нет — начинаем с пустой базы

    string line;
    while (getline(in, line)) {
        istringstream ls(line);
        char type;
        string name;
        if (!(ls >> type >> name)) continue; // пустая строка

        if (string("MFLSQT").find(type) == string::npos) return false;
        Entry* e = newEntry(type, name);
        db.push_back(e);

        if (type == 'T') {
            bool ok = true;
            e->tree = buildTree(ls, ok);
            if (!ok) return false;
            continue;
        }


        string tok;                          // значения читаются как строки
        while (ls >> tok) {
            switch (type) {
                case 'M': addToEnd(e->arr, tok);      break;
                case 'F': pushBackF(&e->listF, tok);  break;
                case 'L': pushBackD(&e->listD, tok);  break;
                case 'S': pushStack(&e->stack, tok);  break;  // записан снизу вверх
                case 'Q': pushQueue(&e->queue, tok);  break;
            }
        }
    }
    return true;
}

static bool saveDB(const string& filename, const vector<Entry*>& db) {
    ofstream out(filename);
    if (!out) return false;

    for (Entry* e : db) {
        out << e->type << ' ' << e->name;
        switch (e->type) {
        case 'M':
            for (size_t i = 0; i < e->arr.size; ++i) out << ' ' << e->arr.data[i];
            break;
        case 'F':
            for (NodeF* c = e->listF.head; c; c = c->next) out << ' ' << c->data;
            break;
        case 'L':
            for (NodeD* c = e->listD.head; c; c = c->next) out << ' ' << c->data;
            break;
        case 'S': {                                       // снизу вверх
            vector<string> tmp;
            for (NodeS* c = e->stack.head; c; c = c->next) tmp.push_back(c->data);
            for (size_t i = tmp.size(); i > 0; --i) out << ' ' << tmp[i - 1];
            break;
        }
        case 'Q':
            for (NodeQ* c = e->queue.head; c; c = c->next) out << ' ' << c->data;
            break;
        case 'T':
            writeTree(e->tree, out);
            break;
        }
        out << '\n';
    }
    return true;
}

// Вывод

static void printEntry(Entry* e) {
    switch (e->type) {
    case 'M': cout << "-> Массив " << e->name << " (" << getLength(e->arr) << " эл.): ";
              printArray(e->arr); break;
    case 'F': cout << "-> Односвязный список " << e->name << ": ";
              printForwardF(&e->listF); break;
    case 'L': cout << "-> Двусвязный список " << e->name << ": ";
              printForwardD(&e->listD); break;
    case 'S': cout << "-> Стек " << e->name << " (вершина слева): ";
              printStack(&e->stack); break;
    case 'Q': cout << "-> Очередь " << e->name << " (" << sizeQueue(&e->queue) << " эл.): ";
              printQueue(&e->queue); break;
    case 'T': cout << "->";
              printTreeInfoT(e->tree, ("Дерево AVL " + e->name).c_str()); break;
    }
}

// Обработчики команд

static int execArray(Entry* e, const string& cmd, const vector<string>& a, bool& changed) {
    Array& arr = e->arr;
    string v;

    if (cmd == "MPUSH") {
        if (!enough(a, 1, cmd)) return 1;
        addToEnd(arr, a[0]); changed = true;
        cout << "-> " << a[0] << "\n"; return 0;
    }
    if (cmd == "MINSERT") {
        if (!enough(a, 2, cmd)) return 1;
        size_t idx;
        if (!parseIndex(a[0], idx) || !addAtIndex(arr, idx, a[1]))
            return fail("индекс вне диапазона");
        changed = true;
        cout << "-> " << a[1] << "\n"; return 0;
    }
    if (cmd == "MGET") {
        if (!enough(a, 1, cmd)) return 1;
        size_t idx;
        if (!parseIndex(a[0], idx) || !getElement(arr, idx, v))
            return fail("индекс вне диапазона");
        cout << "-> " << v << "\n"; return 0;
    }
    if (cmd == "MSET") {
        if (!enough(a, 2, cmd)) return 1;
        size_t idx;
        if (!parseIndex(a[0], idx) || !replaceAtIndex(arr, idx, a[1]))
            return fail("индекс вне диапазона");
        changed = true;
        cout << "-> " << a[1] << "\n"; return 0;
    }
    if (cmd == "MDEL") {
        if (!enough(a, 1, cmd)) return 1;
        size_t idx;
        if (!parseIndex(a[0], idx) || !getElement(arr, idx, v))
            return fail("индекс вне диапазона");
        removeAtIndex(arr, idx); changed = true;
        cout << "-> " << v << "\n"; return 0;
    }
    if (cmd == "MLEN") {
        cout << "-> " << getLength(arr) << "\n"; return 0;
    }
    return fail("неизвестная команда " + cmd);
}

static int execListF(Entry* e, const string& cmd, const vector<string>& a, bool& changed) {
    ListF* l = &e->listF;
    if (cmd == "FPUSH_HEAD") {
        if (!enough(a, 1, cmd)) return 1;
        pushFrontF(l, a[0]); changed = true;
        cout << "-> " << a[0] << "\n"; return 0;
    }
    if (cmd == "FPUSH_TAIL" || cmd == "FPUSH") {
        if (!enough(a, 1, cmd)) return 1;
        pushBackF(l, a[0]); changed = true;
        cout << "-> " << a[0] << "\n"; return 0;
    }
    if (cmd == "FPUSH_AFTER") {
        if (!enough(a, 2, cmd)) return 1;
        if (!insertAfterF(l, a[0], a[1])) return fail("значение " + a[0] + " не найдено");
        changed = true;
        cout << "-> " << a[1] << "\n"; return 0;
    }
    if (cmd == "FPUSH_BEFORE") {
        if (!enough(a, 2, cmd)) return 1;
        if (!insertBeforeF(l, a[0], a[1])) return fail("значение " + a[0] + " не найдено");
        changed = true;
        cout << "-> " << a[1] << "\n"; return 0;
    }
    if (cmd == "FDEL_HEAD") {
        if (!l->head) return fail("список пуст");
        string v = l->head->data;
        popFrontF(l); changed = true;
        cout << "-> " << v << "\n"; return 0;
    }
    if (cmd == "FDEL_TAIL") {
        if (!l->tail) return fail("список пуст");
        string v = l->tail->data;
        popBackF(l); changed = true;
        cout << "-> " << v << "\n"; return 0;
    }
    if (cmd == "FDEL_AFTER") {
        if (!enough(a, 1, cmd)) return 1;
        NodeF* t = findNodeF(l, a[0]);
        if (!t || !t->next) return fail("после " + a[0] + " нет элемента");
        string v = t->next->data;
        removeAfterF(l, a[0]); changed = true;
        cout << "-> " << v << "\n"; return 0;
    }
    if (cmd == "FDEL_BEFORE") {
        if (!enough(a, 1, cmd)) return 1;
        int idx = searchF(l, a[0]);
        if (idx < 1) return fail("перед " + a[0] + " нет элемента");
        string v = getAtF(l, idx - 1)->data;
        removeBeforeF(l, a[0]); changed = true;
        cout << "-> " << v << "\n"; return 0;
    }
    if (cmd == "FDEL_VALUE" || cmd == "FDEL") {
        if (!enough(a, 1, cmd)) return 1;
        if (!removeByValueF(l, a[0])) return fail("значение " + a[0] + " не найдено");
        changed = true;
        cout << "-> " << a[0] << "\n"; return 0;
    }
    if (cmd == "FSEARCH") {
        if (!enough(a, 1, cmd)) return 1;
        cout << "-> " << searchF(l, a[0]) << "\n"; return 0;   // -1 = нет
    }
    if (cmd == "FGET") {
        if (!enough(a, 1, cmd)) return 1;
        size_t idx;
        if (!parseIndex(a[0], idx)) return fail("индекс вне диапазона");
        NodeF* node = getAtF(l, (int)idx);
        if (!node) return fail("индекс вне диапазона");
        cout << "-> " << node->data << "\n"; return 0;
    }
    if (cmd == "FPRINT")      { cout << "-> ";   printForwardF(l);  return 0; }
    if (cmd == "FPRINT_BACK") { cout << "-> ";   printBackwardF(l); return 0; }
    if (cmd == "FPRINT_IDX")  { cout << "->\n";  printIndexedF(l);  return 0; }
    return fail("неизвестная команда " + cmd);
}

static int execListD(Entry* e, const string& cmd, const vector<string>& a, bool& changed) {
    ListD* l = &e->listD;
    if (cmd == "LPUSH_HEAD") {
        if (!enough(a, 1, cmd)) return 1;
        pushFrontD(l, a[0]); changed = true;
        cout << "-> " << a[0] << "\n"; return 0;
    }
    if (cmd == "LPUSH_TAIL" || cmd == "LPUSH") {
        if (!enough(a, 1, cmd)) return 1;
        pushBackD(l, a[0]); changed = true;
        cout << "-> " << a[0] << "\n"; return 0;
    }
    if (cmd == "LPUSH_AFTER") {
        if (!enough(a, 2, cmd)) return 1;
        if (!insertAfterD(l, a[0], a[1])) return fail("значение " + a[0] + " не найдено");
        changed = true;
        cout << "-> " << a[1] << "\n"; return 0;
    }
    if (cmd == "LPUSH_BEFORE") {
        if (!enough(a, 2, cmd)) return 1;
        if (!insertBeforeD(l, a[0], a[1])) return fail("значение " + a[0] + " не найдено");
        changed = true;
        cout << "-> " << a[1] << "\n"; return 0;
    }
    if (cmd == "LDEL_HEAD") {
        if (!l->head) return fail("список пуст");
        string v = l->head->data;
        popFrontD(l); changed = true;
        cout << "-> " << v << "\n"; return 0;
    }
    if (cmd == "LDEL_TAIL") {
        if (!l->tail) return fail("список пуст");
        string v = l->tail->data;
        popBackD(l); changed = true;
        cout << "-> " << v << "\n"; return 0;
    }
    if (cmd == "LDEL_AFTER") {
        if (!enough(a, 1, cmd)) return 1;
        NodeD* t = findNodeD(l, a[0]);
        if (!t || !t->next) return fail("после " + a[0] + " нет элемента");
        string v = t->next->data;
        removeAfterD(l, a[0]); changed = true;
        cout << "-> " << v << "\n"; return 0;
    }
    if (cmd == "LDEL_BEFORE") {
        if (!enough(a, 1, cmd)) return 1;
        NodeD* t = findNodeD(l, a[0]);
        if (!t || !t->prev) return fail("перед " + a[0] + " нет элемента");
        string v = t->prev->data;
        removeBeforeD(l, a[0]); changed = true;
        cout << "-> " << v << "\n"; return 0;
    }
    if (cmd == "LDEL_VALUE" || cmd == "LDEL") {
        if (!enough(a, 1, cmd)) return 1;
        if (!removeByValueD(l, a[0])) return fail("значение " + a[0] + " не найдено");
        changed = true;
        cout << "-> " << a[0] << "\n"; return 0;
    }
    if (cmd == "LSEARCH") {
        if (!enough(a, 1, cmd)) return 1;
        cout << "-> " << searchD(l, a[0]) << "\n"; return 0;
    }
    if (cmd == "LGET") {
        if (!enough(a, 1, cmd)) return 1;
        size_t idx;
        if (!parseIndex(a[0], idx)) return fail("индекс вне диапазона");
        NodeD* node = getAtD(l, (int)idx);
        if (!node) return fail("индекс вне диапазона");
        cout << "-> " << node->data << "\n"; return 0;
    }
    if (cmd == "LPRINT")      { cout << "-> ";  printForwardD(l);  return 0; }
    if (cmd == "LPRINT_BACK") { cout << "-> ";  printBackwardD(l); return 0; }
    if (cmd == "LPRINT_IDX")  { cout << "->\n"; printIndexedD(l);  return 0; }
    return fail("неизвестная команда " + cmd);
}

static int execStack(Entry* e, const string& cmd, const vector<string>& a, bool& changed) {
    string v;
    if (cmd == "SPUSH") {
        if (!enough(a, 1, cmd)) return 1;
        pushStack(&e->stack, a[0]); changed = true;
        cout << "-> " << a[0] << "\n"; return 0;
    }
    if (cmd == "SPOP") {
        if (!peekStack(&e->stack, v)) return fail("стек пуст");
        popStack(&e->stack, v); changed = true;
        cout << "-> " << v << "\n"; return 0;
    }
    if (cmd == "SGET") {
        if (!peekStack(&e->stack, v)) return fail("стек пуст");
        cout << "-> " << v << "\n"; return 0;
    }
    return fail("неизвестная команда " + cmd);
}

static int execQueue(Entry* e, const string& cmd, const vector<string>& a, bool& changed) {
    string v;
    if (cmd == "QPUSH") {
        if (!enough(a, 1, cmd)) return 1;
        pushQueue(&e->queue, a[0]); changed = true;
        cout << "-> " << a[0] << "\n"; return 0;
    }
    if (cmd == "QPOP") {
        if (!popQueue(&e->queue, v)) return fail("очередь пуста");
        changed = true;
        cout << "-> " << v << "\n"; return 0;
    }
    if (cmd == "QGET") {
        if (!peekQueue(&e->queue, v)) return fail("очередь пуста");
        cout << "-> " << v << "\n"; return 0;
    }
    return fail("неизвестная команда " + cmd);
}

static int execTree(Entry* e, const string& cmd, const vector<string>& a, bool& changed) {
    if (!enough(a, 1, cmd)) return 1;
    int key;
    if (!toInt(a[0], key)) return fail("аргумент '" + a[0] + "' не целое число");

    if (cmd == "TINSERT") {
        if (findT(e->tree, key)) return fail("значение " + a[0] + " уже есть в дереве");
        e->tree = insertT(e->tree, key); changed = true;
        cout << "-> " << key << "\n"; return 0;
    }
    if (cmd == "TDEL") {
        if (!findT(e->tree, key)) return fail("значение " + a[0] + " не найдено");
        e->tree = removeT(e->tree, key); changed = true;
        cout << "-> " << key << "\n"; return 0;
    }
    if (cmd == "TGET") {
        int steps = 0;
        bool found = findWithStepsT(e->tree, key, steps) != nullptr;
        cout << "-> " << (found ? "TRUE" : "FALSE")
             << " (просмотрено узлов: " << steps << ")\n";
        return 0;
    }
    if (cmd == "ISMEMBER") {
        cout << "-> " << (findT(e->tree, key) ? "TRUE" : "FALSE") << "\n"; return 0;
    }
    return fail("неизвестная команда " + cmd);
}


//Разбор запроса 

static int execute(vector<Entry*>& db, vector<string>& a, bool& changed) {
    for (char& c : a[0]) c = (char)toupper((unsigned char)c);
    const string cmd = a[0];
    if (a.size() < 2) return fail("не указано имя структуры");
    const string& name = a[1];
    Entry* e = findEntry(db, name);

    if (cmd == "PRINT") {
        if (!e) return fail("структура " + name + " не найдена");
        printEntry(e);
        return 0;
    }

    // Тип структуры = первая буква команды
    char type = (cmd == "ISMEMBER") ? 'T' : cmd[0];
    if (string("MFLSQT").find(type) == string::npos) return fail("неизвестная команда " + cmd);
    if (e && e->type != type)
        return fail("имя " + name + " уже занято структурой типа " + e->type);

    // Аргументы после имени — это уже СТРОКИ, никакого toInt.
    // Индексы разбираются отдельно, внутри нужных команд (parseIndex).
    vector<string> args(a.begin() + 2, a.end());

    if (!e) { e = newEntry(type, name); db.push_back(e); }   // новая структура

    switch (type) {
        case 'M': return execArray(e, cmd, args, changed);
        case 'F': return execListF(e, cmd, args, changed);
        case 'L': return execListD(e, cmd, args, changed);
        case 'S': return execStack(e, cmd, args, changed);
        case 'Q': return execQueue(e, cmd, args, changed);
        default:  return execTree (e, cmd, args, changed);
    }
}

//точка входа
int runQuery(const string& filename, const string& query) {
    vector<string> a;
    istringstream qs(query);
    string w;
    while (qs >> w) a.push_back(w);
    if (a.empty()) return fail("пустой запрос");

    vector<Entry*> db;
    if (!loadDB(filename, db)) {
        freeAll(db);
        return fail("файл " + filename + " повреждён");
    }

    bool changed = false;
    int code = execute(db, a, changed);

    // Файл перезаписывается только при изменении состава элементов
    if (changed && !saveDB(filename, db)) code = fail("не удалось записать " + filename);

    freeAll(db);
    return code;
}
