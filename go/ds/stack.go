package ds

import "strings"

type sNode struct {
	val  string
	next *sNode
}

// Stack — стек (LIFO) на односвязном списке.
type Stack struct {
	top  *sNode
	size int
}

func NewStack() *Stack { return &Stack{} }

func (s *Stack) Push(v string) {
	s.top = &sNode{val: v, next: s.top}
	s.size++
}

func (s *Stack) Pop() (string, error) {
	if s.top == nil {
		return "", ErrEmpty
	}
	v := s.top.val
	s.top = s.top.next
	s.size--
	return v, nil
}

func (s *Stack) Peek() (string, error) {
	if s.top == nil {
		return "", ErrEmpty
	}
	return s.top.val, nil
}

func (s *Stack) Len() int { return s.size }

// Items — от вершины ко дну.
func (s *Stack) Items() []string {
	out := make([]string, 0, s.size)
	for cur := s.top; cur != nil; cur = cur.next {
		out = append(out, cur.val)
	}
	return out
}

func (s *Stack) String() string {
	if s.top == nil {
		return "стек пуст"
	}
	return "вершина -> " + strings.Join(s.Items(), " ")
}
