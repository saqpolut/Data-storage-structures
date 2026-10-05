#ifndef DBMS_H
#define DBMS_H

#include <string>

// Выполнить одну команду над файлом данных. Возвращает 0 при успехе.
int runQuery(const std::string& filename, const std::string& query);

#endif
