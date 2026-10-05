package main

import (
	"flag"
	"fmt"
	"os"
)

func main() {
	file := flag.String("file", "", "файл с данными")
	query := flag.String("query", "", "команда, например 'SPUSH mystack item'")
	flag.Parse()

	if *file == "" || *query == "" {
		fmt.Println("Использование: dbms --file <файл> --query '<команда>'")
		os.Exit(1)
	}
	os.Exit(run(*file, *query))
}
