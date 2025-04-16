package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type Cell struct {
	items map[string]int
}

type Storage struct {
	zones         []string
	racks         []int
	vSects        []int
	hSects        []int
	loadedZones   map[string]int
	totalCapacity int
	freeCapacity  int
	cells         map[string]Cell
}

type result int

const (
	SUCCESS result = iota
	NOCELL
	TOOMANY
)

// добавляем товар в ячейку
func add(procStorage *Storage, item string, amount int, cell string) result {
	storageCell, ok := procStorage.cells[cell]
	if !ok {
		return NOCELL
	}
	currQty := 0
	for _, qty := range storageCell.items {
		currQty += qty
	}
	if currQty+amount <= 10 {
		storageCell.items[item] += amount
		procStorage.cells[cell] = storageCell
		procStorage.freeCapacity -= amount
		zone := string(cell[0])
		procStorage.loadedZones[zone] += amount
		return SUCCESS
	}
	return TOOMANY
}

// убираем товар из ячейки
func remove(procStorage *Storage, item string, amount int, cell string) result {
	storageCell, ok := procStorage.cells[cell]
	if !ok {
		return NOCELL
	}
	currQty := storageCell.items[item]
	if currQty-amount >= 0 {
		storageCell.items[item] -= amount
		procStorage.cells[cell] = storageCell
		procStorage.freeCapacity += amount
		zone := string(cell[0])
		procStorage.loadedZones[zone] -= amount
		return SUCCESS
	}
	return TOOMANY
}

// выводим данные по складу
func printInfo(procStorage *Storage) {
	totalLoad := float64((procStorage.totalCapacity-procStorage.freeCapacity)*100) / float64(procStorage.totalCapacity)
	fmt.Printf("Весь склад загружен на %.2f%%\n", totalLoad)

	zoneAm := len(procStorage.loadedZones)
	for zone, load := range procStorage.loadedZones {
		zoneLoad := float64(load) / (float64(procStorage.totalCapacity) / float64(zoneAm)) * 100
		fmt.Printf("Зона %s загружена на %.2f%%\n", zone, zoneLoad)
	}

	for cell, c := range procStorage.cells {
		for item, qty := range c.items {
			if qty > 0 {
				fmt.Println(cell, item, qty)
			}
		}
	}

	// пустые ячейки
	for _, zone := range procStorage.zones {
		for _, rack := range procStorage.racks {
			for _, v := range procStorage.vSects {
				for _, h := range procStorage.hSects {
					address := fmt.Sprintf("%s%d%d%d", zone, rack, v, h)
					if len(procStorage.cells[address].items) == 0 {
						fmt.Print(address + " ")
					}
				}
			}
			fmt.Println()
		}
	}
}

// разбиваем строку на части
func splitString(s string, divider string) []string {
	return strings.Split(s, divider)
}

func main() {
	zones := []string{"A", "B", "C"}
	racks := []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20}
	vSects := []int{1, 2, 3, 4, 5}
	hSects := []int{1, 2}
	loadedZones := map[string]int{"A": 0, "B": 0, "C": 0}

	mainStorage := Storage{
		zones:       zones,
		racks:       racks,
		vSects:      vSects,
		hSects:      hSects,
		loadedZones: loadedZones,
		cells:       make(map[string]Cell),
	}

	mainStorage.totalCapacity = len(zones) * len(racks) * len(vSects) * len(hSects) * 10
	mainStorage.freeCapacity = mainStorage.totalCapacity

	// генерим адреса ячеек
	for _, zone := range zones {
		for _, rack := range racks {
			for _, v := range vSects {
				for _, h := range hSects {
					address := fmt.Sprintf("%s%d%d%d", zone, rack, v, h)
					mainStorage.cells[address] = Cell{items: make(map[string]int)}
				}
			}
		}
	}

	scanner := bufio.NewScanner(os.Stdin)
	for scanner.Scan() {
		command := scanner.Text()
		splitted := splitString(command, " ")
		if len(splitted) == 0 {
			continue
		}
		task := splitted[0]

		switch task {
		case "ADD":
			if len(splitted) != 4 {
				fmt.Println("Invalid arguments amount. Try again")
				continue
			}
			amount, _ := strconv.Atoi(splitted[2])
			status := add(&mainStorage, splitted[1], amount, splitted[3])
			if status == SUCCESS {
				fmt.Println("Элемент успешно добавлен в ячейку")
			} else if status == NOCELL {
				fmt.Println("Ячейка не найдена")
			} else if status == TOOMANY {
				fmt.Println("Недостаточно места в ячейке")
			}

		case "REMOVE":
			if len(splitted) != 4 {
				fmt.Println("Invalid arguments amount. Try again")
				continue
			}
			amount, _ := strconv.Atoi(splitted[2])
			status := remove(&mainStorage, splitted[1], amount, splitted[3])
			if status == SUCCESS {
				fmt.Println("Товар успешно удален из ячейки")
			} else if status == NOCELL {
				fmt.Println("Ячейка не найдена")
			} else if status == TOOMANY {
				fmt.Println("Недостаточно товаров в ячейке")
			}

		case "INFO":
			printInfo(&mainStorage)

		case "EXIT":
			fmt.Println("Executing exit")
			return

		default:
			fmt.Println("Unknown command. Try again")
		}
	}
}
