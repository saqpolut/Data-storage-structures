package ds
//общие ошибки и вывод
import (
	"errors"
	"fmt"
	"strings"
)

var (
	ErrIndex    = errors.New("индекс вне диапазона")
	ErrEmpty    = errors.New("структура пуста")
	ErrNotFound = errors.New("элемент не найден")
)

// indexed — вывод элементов с номерами, по одному на строку.
func indexed(items []string) string {
	if len(items) == 0 {
		return "список пуст"
	}
	lines := make([]string, len(items))
	for i, v := range items {
		lines[i] = fmt.Sprintf("[%d] %s", i, v)
	}
	return strings.Join(lines, "\n")
}
