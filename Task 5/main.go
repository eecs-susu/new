package main

import (
	"fmt"
	"sync"
	"time"
)

func routine(id int, wg *sync.WaitGroup) {
	for i := 0; i < 5; i++ {
		fmt.Printf("Hello world from coroutine %d\n", i)
		time.Sleep(time.Second * time.Duration(id+1))
	}
	wg.Done()
}

func main() {
	wg := &sync.WaitGroup{}
	for i := 0; i < 5; i++ {
		wg.Add(1)
		go routine(i, wg)
	}
	wg.Wait()
}
