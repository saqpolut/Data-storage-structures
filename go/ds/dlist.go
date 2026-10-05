package ds

import "strings"

type dNode struct {
	val        string
	prev, next *dNode
}

// DList — двусвязный список с указателями на голову и хвост.
type DList struct {
	head, tail *dNode
	size       int
}

func NewDList() *DList { return &DList{} }

func (l *DList) find(v string) *dNode {
	for cur := l.head; cur != nil; cur = cur.next {
		if cur.val == v {
			return cur
		}
	}
	return nil
}

// unlink — выкинуть известный узел за O(1).
func (l *DList) unlink(n *dNode) {
	if n.prev != nil {
		n.prev.next = n.next
	} else {
		l.head = n.next
	}
	if n.next != nil {
		n.next.prev = n.prev
	} else {
		l.tail = n.prev
	}
	l.size--
}

// ==================== ДОБАВЛЕНИЕ ====================

func (l *DList) PushHead(v string) {
	n := &dNode{val: v, next: l.head}
	if l.head != nil {
		l.head.prev = n
	} else {
		l.tail = n
	}
	l.head = n
	l.size++
}

func (l *DList) PushTail(v string) {
	n := &dNode{val: v, prev: l.tail}
	if l.tail != nil {
		l.tail.next = n
	} else {
		l.head = n
	}
	l.tail = n
	l.size++
}

func (l *DList) InsertAfter(target, v string) error {
	t := l.find(target)
	if t == nil {
		return ErrNotFound
	}
	n := &dNode{val: v, prev: t, next: t.next}
	if t.next != nil {
		t.next.prev = n
	} else {
		l.tail = n
	}
	t.next = n
	l.size++
	return nil
}

func (l *DList) InsertBefore(target, v string) error {
	t := l.find(target)
	if t == nil {
		return ErrNotFound
	}
	n := &dNode{val: v, prev: t.prev, next: t}
	if t.prev != nil {
		t.prev.next = n
	} else {
		l.head = n
	}
	t.prev = n
	l.size++
	return nil
}

// ==================== УДАЛЕНИЕ ====================

func (l *DList) PopHead() (string, error) {
	if l.head == nil {
		return "", ErrEmpty
	}
	v := l.head.val
	l.unlink(l.head)
	return v, nil
}

// PopTail — O(1) благодаря ссылке prev.
func (l *DList) PopTail() (string, error) {
	if l.tail == nil {
		return "", ErrEmpty
	}
	v := l.tail.val
	l.unlink(l.tail)
	return v, nil
}

func (l *DList) RemoveAfter(target string) (string, error) {
	t := l.find(target)
	if t == nil || t.next == nil {
		return "", ErrNotFound
	}
	v := t.next.val
	l.unlink(t.next)
	return v, nil
}

func (l *DList) RemoveBefore(target string) (string, error) {
	t := l.find(target)
	if t == nil || t.prev == nil {
		return "", ErrNotFound
	}
	v := t.prev.val
	l.unlink(t.prev)
	return v, nil
}

func (l *DList) RemoveValue(v string) error {
	t := l.find(v)
	if t == nil {
		return ErrNotFound
	}
	l.unlink(t)
	return nil
}

// ==================== ПОИСК И ЧТЕНИЕ ====================

func (l *DList) Search(v string) int {
	i := 0
	for cur := l.head; cur != nil; cur = cur.next {
		if cur.val == v {
			return i
		}
		i++
	}
	return -1
}

func (l *DList) Get(i int) (string, error) {
	if i < 0 {
		return "", ErrIndex
	}
	cur := l.head
	for k := 0; cur != nil && k < i; k++ {
		cur = cur.next
	}
	if cur == nil {
		return "", ErrIndex
	}
	return cur.val, nil
}

func (l *DList) Len() int { return l.size }

func (l *DList) Items() []string {
	out := make([]string, 0, l.size)
	for cur := l.head; cur != nil; cur = cur.next {
		out = append(out, cur.val)
	}
	return out
}

func (l *DList) String() string {
	if l.head == nil {
		return "список пуст"
	}
	return strings.Join(l.Items(), " <-> ") + " <-> nil"
}

// StringBackward — от хвоста к голове по ссылкам prev, без рекурсии.
func (l *DList) StringBackward() string {
	if l.tail == nil {
		return "список пуст"
	}
	out := make([]string, 0, l.size)
	for cur := l.tail; cur != nil; cur = cur.prev {
		out = append(out, cur.val)
	}
	return strings.Join(out, " <-> ") + " <-> nil"
}

func (l *DList) StringIndexed() string { return indexed(l.Items()) }
