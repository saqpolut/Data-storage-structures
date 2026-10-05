package ds

import "strings"

type qNode struct {
	val  string
	next *qNode
}

// Queue — очередь (FIFO) на односвязном списке с head и tail.
type Queue struct {
	head, tail *qNode
	size       int
}

func NewQueue() *Queue { return &Queue{} }

// Push — в хвост, O(1).
func (q *Queue) Push(v string) {
	n := &qNode{val: v}
	if q.tail == nil {
		q.head, q.tail = n, n
	} else {
		q.tail.next = n
		q.tail = n
	}
	q.size++
}

// Pop — из головы, O(1).
func (q *Queue) Pop() (string, error) {
	if q.head == nil {
		return "", ErrEmpty
	}
	v := q.head.val
	q.head = q.head.next
	if q.head == nil {
		q.tail = nil
	}
	q.size--
	return v, nil
}

func (q *Queue) Peek() (string, error) {
	if q.head == nil {
		return "", ErrEmpty
	}
	return q.head.val, nil
}

func (q *Queue) Len() int { return q.size }

func (q *Queue) Items() []string {
	out := make([]string, 0, q.size)
	for cur := q.head; cur != nil; cur = cur.next {
		out = append(out, cur.val)
	}
	return out
}

func (q *Queue) String() string {
	if q.head == nil {
		return "очередь пуста"
	}
	return "head -> " + strings.Join(q.Items(), " ") + " <- tail"
}
