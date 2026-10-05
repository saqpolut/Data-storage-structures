package main

import (
	"fmt"
	"strconv"
	"strings"

	"lab1go/ds"
)

// Сколько аргументов нужно команде (после имени структуры).
var argCount = map[string]int{
	"MPUSH": 1, "MINSERT": 2, "MGET": 1, "MSET": 2, "MDEL": 1,
	"SPUSH": 1, "QPUSH": 1,
}

func init() {
	listArgs := map[string]int{
		"PUSH_HEAD": 1, "PUSH_TAIL": 1, "PUSH": 1, "PUSH_AFTER": 2, "PUSH_BEFORE": 2,
		"DEL_AFTER": 1, "DEL_BEFORE": 1, "DEL_VALUE": 1, "DEL": 1,
		"SEARCH": 1, "GET": 1,
	}
	for _, p := range []string{"F", "L"} {
		for s, n := range listArgs {
			argCount[p+s] = n
		}
	}
}

// list — общий интерфейс односвязного и двусвязного списков.
// Обе реализации разные внутри, но команды к ним одни и те же.
type list interface {
	PushHead(v string)
	PushTail(v string)
	InsertAfter(target, v string) error
	InsertBefore(target, v string) error
	PopHead() (string, error)
	PopTail() (string, error)
	RemoveAfter(target string) (string, error)
	RemoveBefore(target string) (string, error)
	RemoveValue(v string) error
	Search(v string) int
	Get(i int) (string, error)
	Len() int
	String() string
	StringBackward() string
	StringIndexed() string
}

// read — результат чтения: состав не меняется.
func read(v string, err error) (string, bool, error) { return v, false, err }

// modify — результат изменения: файл перезаписывается, если ошибки нет.
func modify(v string, err error) (string, bool, error) { return v, err == nil, err }

func toIndex(s string) (int, error) {
	i, err := strconv.Atoi(s)
	if err != nil {
		return 0, fmt.Errorf("индекс '%s' не целое число", s)
	}
	return i, nil
}

func unknown(cmd string) (string, bool, error) {
	return "", false, fmt.Errorf("неизвестная команда %s", cmd)
}

// ==================== МАССИВ ====================

func execArray(a *ds.Array, cmd string, args []string) (string, bool, error) {
	switch cmd {
	case "MPUSH":
		a.PushBack(args[0])
		return args[0], true, nil
	case "MINSERT":
		i, err := toIndex(args[0])
		if err != nil {
			return "", false, err
		}
		return modify(args[1], a.Insert(i, args[1]))
	case "MGET":
		i, err := toIndex(args[0])
		if err != nil {
			return "", false, err
		}
		return read(a.Get(i))
	case "MSET":
		i, err := toIndex(args[0])
		if err != nil {
			return "", false, err
		}
		return modify(args[1], a.Set(i, args[1]))
	case "MDEL":
		i, err := toIndex(args[0])
		if err != nil {
			return "", false, err
		}
		return modify(a.Remove(i))
	case "MLEN":
		return strconv.Itoa(a.Len()), false, nil
	}
	return unknown(cmd)
}

// ==================== СПИСКИ F и L ====================

func execList(l list, cmd string, args []string) (string, bool, error) {
	switch cmd[1:] { // без первой буквы: F/L
	case "PUSH_HEAD":
		l.PushHead(args[0])
		return args[0], true, nil
	case "PUSH_TAIL", "PUSH":
		l.PushTail(args[0])
		return args[0], true, nil
	case "PUSH_AFTER":
		return modify(args[1], l.InsertAfter(args[0], args[1]))
	case "PUSH_BEFORE":
		return modify(args[1], l.InsertBefore(args[0], args[1]))
	case "DEL_HEAD":
		return modify(l.PopHead())
	case "DEL_TAIL":
		return modify(l.PopTail())
	case "DEL_AFTER":
		return modify(l.RemoveAfter(args[0]))
	case "DEL_BEFORE":
		return modify(l.RemoveBefore(args[0]))
	case "DEL_VALUE", "DEL":
		return modify(args[0], l.RemoveValue(args[0]))
	case "SEARCH":
		return strconv.Itoa(l.Search(args[0])), false, nil // -1 = нет
	case "GET":
		i, err := toIndex(args[0])
		if err != nil {
			return "", false, err
		}
		return read(l.Get(i))
	case "LEN":
		return strconv.Itoa(l.Len()), false, nil
	case "PRINT_BACK":
		return l.StringBackward(), false, nil
	case "PRINT_IDX":
		return "\n" + l.StringIndexed(), false, nil
	}
	return unknown(cmd)
}

// ==================== СТЕК И ОЧЕРЕДЬ ====================

func execStack(s *ds.Stack, cmd string, args []string) (string, bool, error) {
	switch cmd {
	case "SPUSH":
		s.Push(args[0])
		return args[0], true, nil
	case "SPOP":
		return modify(s.Pop())
	case "SGET":
		return read(s.Peek())
	case "SLEN":
		return strconv.Itoa(s.Len()), false, nil
	}
	return unknown(cmd)
}

func execQueue(q *ds.Queue, cmd string, args []string) (string, bool, error) {
	switch cmd {
	case "QPUSH":
		q.Push(args[0])
		return args[0], true, nil
	case "QPOP":
		return modify(q.Pop())
	case "QGET":
		return read(q.Peek())
	case "QLEN":
		return strconv.Itoa(q.Len()), false, nil
	}
	return unknown(cmd)
}

// ==================== PRINT ====================

func describe(e *entry) string {
	switch o := e.obj.(type) {
	case *ds.Array:
		return fmt.Sprintf("Массив %s (%d эл.): %s", e.name, o.Len(), o)
	case *ds.FList:
		return fmt.Sprintf("Односвязный список %s (%d эл.): %s", e.name, o.Len(), o)
	case *ds.DList:
		return fmt.Sprintf("Двусвязный список %s (%d эл.): %s", e.name, o.Len(), o)
	case *ds.Stack:
		return fmt.Sprintf("Стек %s (%d эл.): %s", e.name, o.Len(), o)
	case *ds.Queue:
		return fmt.Sprintf("Очередь %s (%d эл.): %s", e.name, o.Len(), o)
	}
	return fmt.Sprintf("%s — структура типа %c, обрабатывается C++-версией", e.name, e.kind)
}

// ==================== РАЗБОР ЗАПРОСА ====================

func execute(db *[]*entry, a []string) (bool, error) {
	cmd := strings.ToUpper(a[0])
	if len(a) < 2 {
		return false, fmt.Errorf("не указано имя структуры")
	}
	name, args := a[1], a[2:]
	e := findEntry(*db, name)

	if cmd == "PRINT" {
		if e == nil {
			return false, fmt.Errorf("структура %s не найдена", name)
		}
		fmt.Println("->", describe(e))
		return false, nil
	}

	kind := cmd[0]
	if kind == 'T' || cmd == "ISMEMBER" {
		return false, fmt.Errorf("деревья реализованы в C++-версии")
	}
	if !strings.ContainsRune(goKinds, rune(kind)) {
		return false, fmt.Errorf("неизвестная команда %s", cmd)
	}
	if need := argCount[cmd]; len(args) < need {
		return false, fmt.Errorf("для %s нужно аргументов: %d", cmd, need)
	}
	if e != nil && e.kind != kind {
		return false, fmt.Errorf("имя %s уже занято структурой типа %c", name, e.kind)
	}

	created := false
	if e == nil {
		e = &entry{kind: kind, name: name, obj: newObj(kind)}
		created = true
	}

	var out string
	var changed bool
	var err error
	switch o := e.obj.(type) {
	case *ds.Array:
		out, changed, err = execArray(o, cmd, args)
	case *ds.FList:
		out, changed, err = execList(o, cmd, args)
	case *ds.DList:
		out, changed, err = execList(o, cmd, args)
	case *ds.Stack:
		out, changed, err = execStack(o, cmd, args)
	case *ds.Queue:
		out, changed, err = execQueue(o, cmd, args)
	}
	if err != nil {
		return false, err
	}
	if created && changed {
		*db = append(*db, e) // новая структура появляется в файле только после изменения
	}
	fmt.Println("->", out)
	return changed, nil
}

func fail(msg string) int {
	fmt.Println("-> ERROR:", msg)
	return 1
}

func run(file, query string) int {
	a := strings.Fields(query)
	if len(a) == 0 {
		return fail("пустой запрос")
	}
	db, err := loadDB(file)
	if err != nil {
		return fail(err.Error())
	}
	changed, err := execute(&db, a)
	if err != nil {
		return fail(err.Error())
	}
	// Файл перезаписывается только при изменении состава элементов
	if changed {
		if err := saveDB(file, db); err != nil {
			return fail("не удалось записать файл: " + err.Error())
		}
	}
	return 0
}
