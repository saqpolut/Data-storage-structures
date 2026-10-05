package ds

import "strings"

// Array — динамический массив. При заполнении ёмкость удваивается.
type Array struct {
	data []string // выделенная память: len(data) = ёмкость
	size int      // сколько элементов занято
}

func NewArray() *Array { return &Array{} }

func (a *Array) grow() { //увеличение буфера дин масива
	newCap := 1
	if len(a.data) > 0 {
		newCap = len(a.data) * 2
	}
	newData := make([]string, newCap)
	copy(newData, a.data[:a.size])
	a.data = newData
}

// PushBack — добавить в конец. Амортизированно O(1).
func (a *Array) PushBack(v string) {
	if a.size == len(a.data) {
		a.grow()
	}
	a.data[a.size] = v
	a.size++
}

// Insert — вставить по индексу со сдвигом вправо. O(n).
func (a *Array) Insert(i int, v string) error {
	if i < 0 || i > a.size {
		return ErrIndex
	}
	if a.size == len(a.data) {
		a.grow()
	}
	for j := a.size; j > i; j-- {
		a.data[j] = a.data[j-1]
	}
	a.data[i] = v
	a.size++
	return nil
}

// Get — элемент по индексу. O(1).
func (a *Array) Get(i int) (string, error) {
	if i < 0 || i >= a.size {
		return "", ErrIndex
	}
	return a.data[i], nil
}

// Set — замена по индексу. O(1).
func (a *Array) Set(i int, v string) error {
	if i < 0 || i >= a.size {
		return ErrIndex
	}
	a.data[i] = v
	return nil
}

// Remove — удалить по индексу со сдвигом влево. O(n).
func (a *Array) Remove(i int) (string, error) {
	if i < 0 || i >= a.size {
		return "", ErrIndex
	}
	v := a.data[i]
	for j := i; j+1 < a.size; j++ {
		a.data[j] = a.data[j+1]
	}
	a.size--
	a.data[a.size] = ""
	return v, nil
}

func (a *Array) Len() int { return a.size }

func (a *Array) Items() []string {
	out := make([]string, a.size)
	copy(out, a.data[:a.size])
	return out
}

func (a *Array) String() string {
	if a.size == 0 {
		return "массив пуст"
	}
	return "[" + strings.Join(a.Items(), ", ") + "]"
}
