package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
)

type ticketS struct {
	id       string
	duration int
}

// создаём тикет, добавляем в слайс, возвращаем id
func pushTicket(tickets *[]ticketS, time int) string {
	ticketN := 0
	if len(*tickets) > 0 {
		last := (*tickets)[len(*tickets)-1]
		numPart := last.id[1:]
		ticketN, _ = strconv.Atoi(numPart)
	}
	ticketId := "T" + strconv.Itoa(ticketN+1)
	*tickets = append(*tickets, ticketS{ticketId, time})
	return ticketId
}

// сплит по пробелу, типа для ENQUEUE 15
func splitString(s string) []string {
	return strings.SplitN(s, " ", 2)
}

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	fmt.Print(">>> Введите количество окон:\n<<< ")
	scanner.Scan()
	threadsAmount, _ := strconv.Atoi(scanner.Text())

	var tickets []ticketS

	for {
		fmt.Print("<<< ")
		scanner.Scan()
		command := scanner.Text()
		splitted := splitString(command)
		task := splitted[0]

		if task == "ENQUEUE" {
			if len(splitted) != 2 {
				fmt.Println(">>> Неверный формат. Используйте: ENQUEUE <время>")
				continue
			}
			time, _ := strconv.Atoi(splitted[1])
			ticketId := pushTicket(&tickets, time)
			fmt.Println(">>>", ticketId)
		} else if task == "DISTRIBUTE" {
			// создаём очередь из пар: [тикеты, сумма времени]
			queue := make([][]ticketS, threadsAmount)
			totals := make([]int, threadsAmount)

			sortedTickets := make([]ticketS, len(tickets))
			copy(sortedTickets, tickets)

			// сортировка по убыванию времени
			sort.Slice(sortedTickets, func(i, j int) bool {
				return sortedTickets[i].duration > sortedTickets[j].duration
			})

			// раскидываем тикеты по окнам с минимальной загруженностью
			for _, ticket := range sortedTickets {
				minIdx := 0
				for i := 1; i < threadsAmount; i++ {
					if totals[i] < totals[minIdx] {
						minIdx = i
					}
				}
				queue[minIdx] = append(queue[minIdx], ticket)
				totals[minIdx] += ticket.duration
			}

			for i, window := range queue {
				fmt.Printf(">>> Окно %d (%d минут): ", i, totals[i])
				for _, ticket := range window {
					fmt.Print(ticket.id, " ")
				}
				fmt.Println()
			}
		} else if task == "EXIT" {
			fmt.Println(">>> Executing exit")
			break
		} else {
			fmt.Println(">>> Unknown command. Try again")
		}
	}
}
