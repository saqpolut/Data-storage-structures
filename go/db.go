package main

//чтение и запись в файл
import (
	"bufio"
	"errors"
	"fmt"
	"os"
	"strings"

	"lab1go/ds"
)

// Типы, которые обрабатывает Go-версия. Дерево (T) — только в C++.
const goKinds = "MFLSQ"

// entry — одна именованная структура из файла.
type entry struct {
	kind byte // M, F, L, S, Q - один байт символ
	name string
	obj  any    // *ds.Array, *ds.FList, *ds.DList, *ds.Stack, *ds.Queue
	raw  string // строка чужого типа (дерево T) — переписывается как есть
}

func newObj(kind byte) any {
	switch kind {
	case 'M':
		return ds.NewArray()
	case 'F':
		return ds.NewFList()
	case 'L':
		return ds.NewDList()
	case 'S':
		return ds.NewStack()
	case 'Q':
		return ds.NewQueue()
	}
	return nil
}

func findEntry(db []*entry, name string) *entry {
	for _, e := range db {
		if e.name == name {
			return e
		}
	}
	return nil
}

// fill — заполнить структуру элементами из файла.
func fill(e *entry, items []string) {
	for _, v := range items {
		switch o := e.obj.(type) {
		case *ds.Array:
			o.PushBack(v)
		case *ds.FList:
			o.PushTail(v)
		case *ds.DList:
			o.PushTail(v)
		case *ds.Stack:
			o.Push(v) // в файле снизу вверх, поэтому вершина встанет на место
		case *ds.Queue:
			o.Push(v)
		}
	}
}

// items — элементы в порядке записи в файл.
func items(e *entry) []string {
	switch o := e.obj.(type) {
	case *ds.Array:
		return o.Items()
	case *ds.FList:
		return o.Items()
	case *ds.DList:
		return o.Items()
	case *ds.Stack:
		it := o.Items() // от вершины ко дну — разворачиваем
		for i, j := 0, len(it)-1; i < j; i, j = i+1, j-1 {
			it[i], it[j] = it[j], it[i]
		}
		return it
	case *ds.Queue:
		return o.Items()
	}
	return nil
}

// loadDB — формат строки: <тип> <имя> <данные...>
func loadDB(path string) ([]*entry, error) {
	f, err := os.Open(path)
	if errors.Is(err, os.ErrNotExist) {
		return nil, nil // файла нет — начинаем с пустой базы
	}
	if err != nil {
		return nil, err
	}
	defer f.Close()

	var db []*entry
	sc := bufio.NewScanner(f)
	for sc.Scan() {
		line := sc.Text()
		fields := strings.Fields(line)
		if len(fields) == 0 {
			continue
		}
		if len(fields) < 2 || len(fields[0]) != 1 {
			return nil, fmt.Errorf("файл %s повреждён: %q", path, line)
		}
		e := &entry{kind: fields[0][0], name: fields[1]}
		if !strings.ContainsRune(goKinds, rune(e.kind)) {
			e.raw = line
		} else {
			e.obj = newObj(e.kind)
			fill(e, fields[2:])
		}
		db = append(db, e)
	}
	return db, sc.Err()
}

func saveDB(path string, db []*entry) error {
	var b strings.Builder
	for _, e := range db {
		if e.raw != "" {
			b.WriteString(e.raw)
		} else {
			b.WriteByte(e.kind)
			b.WriteByte(' ')
			b.WriteString(e.name)
			for _, v := range items(e) {
				b.WriteByte(' ')
				b.WriteString(v)
			}
		}
		b.WriteByte('\n')
	}
	return os.WriteFile(path, []byte(b.String()), 0644)
}
