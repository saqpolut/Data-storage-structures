package ds

import "strings"

type fNode struct {
	val  string
	next *fNode
}

// FList — односвязный список с указателями на голову и хвост.
type FList struct {
	head, tail *fNode
	size       int
}

func NewFList() *FList { return &FList{} }

func (l *FList) find(v string) *fNode {
	for cur := l.head; cur != nil; cur = cur.next {
		if cur.val == v {
			return cur
		}
	}
	return nil
}

// ==================== ДОБАВЛЕНИЕ ====================

func (l *FList) PushHead(v string) {
	l.head = &fNode{val: v, next: l.head}
	if l.tail == nil {
		l.tail = l.head
	}
	l.size++
}

func (l *FList) PushTail(v string) {
	n := &fNode{val: v}
	if l.tail == nil {
		l.head, l.tail = n, n
	} else {
		l.tail.next = n
		l.tail = n
	}
	l.size++
}

func (l *FList) InsertAfter(target, v string) error {
	t := l.find(target)
	if t == nil {
		return ErrNotFound
	}
	t.next = &fNode{val: v, next: t.next}
	if t == l.tail {
		l.tail = t.next
	}
	l.size++
	return nil
}

// InsertBefore — O(n): нужно найти предыдущий узел.
func (l *FList) InsertBefore(target, v string) error {
	if l.head == nil {
		return ErrNotFound
	}
	if l.head.val == target {
		l.PushHead(v)
		return nil
	}
	prev := l.head
	for prev.next != nil && prev.next.val != target {
		prev = prev.next
	}
	if prev.next == nil {
		return ErrNotFound
	}
	prev.next = &fNode{val: v, next: prev.next}
	l.size++
	return nil
}

// ==================== УДАЛЕНИЕ ====================

func (l *FList) PopHead() (string, error) {
	if l.head == nil {
		return "", ErrEmpty
	}
	v := l.head.val
	l.head = l.head.next
	if l.head == nil {
		l.tail = nil
	}
	l.size--
	return v, nil
}

// PopTail — O(n): в односвязном списке нет ссылки на предыдущий узел.
func (l *FList) PopTail() (string, error) {
	if l.head == nil {
		return "", ErrEmpty
	}
	v := l.tail.val
	if l.head == l.tail {
		l.head, l.tail = nil, nil
	} else {
		prev := l.head
		for prev.next != l.tail {
			prev = prev.next
		}
		prev.next = nil
		l.tail = prev
	}
	l.size--
	return v, nil
}

func (l *FList) RemoveAfter(target string) (string, error) {
	t := l.find(target)
	if t == nil || t.next == nil {
		return "", ErrNotFound
	}
	d := t.next
	t.next = d.next
	if d == l.tail {
		l.tail = t
	}
	l.size--
	return d.val, nil
}

func (l *FList) RemoveBefore(target string) (string, error) {
	if l.head == nil || l.head.val == target {
		return "", ErrNotFound
	}
	if l.head.next != nil && l.head.next.val == target {
		return l.PopHead()
	}
	prev := l.head
	for prev.next != nil && prev.next.next != nil && prev.next.next.val != target {
		prev = prev.next
	}
	if prev.next == nil || prev.next.next == nil {
		return "", ErrNotFound
	}
	d := prev.next
	prev.next = d.next
	l.size--
	return d.val, nil
}

func (l *FList) RemoveValue(v string) error {
	if l.head == nil {
		return ErrNotFound
	}
	if l.head.val == v {
		_, err := l.PopHead()
		return err
	}
	prev := l.head
	for prev.next != nil && prev.next.val != v {
		prev = prev.next
	}
	if prev.next == nil {
		return ErrNotFound
	}
	d := prev.next
	prev.next = d.next
	if d == l.tail {
		l.tail = prev
	}
	l.size--
	return nil
}

// ==================== ПОИСК И ЧТЕНИЕ ====================

// Search — индекс первого вхождения или -1.
func (l *FList) Search(v string) int {
	i := 0
	for cur := l.head; cur != nil; cur = cur.next {
		if cur.val == v {
			return i
		}
		i++
	}
	return -1
}

func (l *FList) Get(i int) (string, error) {
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

func (l *FList) Len() int { return l.size }

func (l *FList) Items() []string {
	out := make([]string, 0, l.size)
	for cur := l.head; cur != nil; cur = cur.next {
		out = append(out, cur.val)
	}
	return out
}

// String — чтение от головы к хвосту.
func (l *FList) String() string {
	if l.head == nil {
		return "список пуст"
	}
	return strings.Join(l.Items(), " -> ") + " -> nil"
}

// StringBackward — чтение от хвоста к голове через рекурсию
// (обратных ссылок в односвязном списке нет).
func (l *FList) StringBackward() string {
	if l.head == nil {
		return "список пуст"
	}
	var out []string
	var rec func(n *fNode)
	rec = func(n *fNode) {
		if n == nil {
			return
		}
		rec(n.next)
		out = append(out, n.val)
	}
	rec(l.head)
	return strings.Join(out, " ")
}

// StringIndexed — чтение с индексами.
func (l *FList) StringIndexed() string { return indexed(l.Items()) }
