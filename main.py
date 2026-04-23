import random
import time
import os

def create_grid(width, height):
    """Создает сетку со случайным начальным состоянием."""
    return [[random.choice([0, 1]) for _ in range(width)] for _ in range(height)]

def print_grid(grid):
    """Выводит сетку в консоль."""
    os.system('clear' if os.name == 'posix' else 'cls')
    for row in grid:
        print(''.join(['█' if cell else ' ' for cell in row]))
    print()

def count_neighbors(grid, x, y):
    """Подсчитывает количество живых соседей вокруг клетки (x, y)."""
    height = len(grid)
    width = len(grid[0])
    count = 0
    for i in range(-1, 2):
        for j in range(-1, 2):
            if i == 0 and j == 0:
                continue
            nx, ny = x + i, y + j
            if 0 <= nx < width and 0 <= ny < height:
                count += grid[ny][nx]
    return count

def next_generation(grid):
    """Вычисляет следующее поколение клеток."""
    height = len(grid)
    width = len(grid[0])
    new_grid = [[0 for _ in range(width)] for _ in range(height)]
    
    for y in range(height):
        for x in range(width):
            neighbors = count_neighbors(grid, x, y)
            if grid[y][x] == 1:
                # Живая клетка остается живой, если у нее 2 или 3 соседа
                if neighbors == 2 or neighbors == 3:
                    new_grid[y][x] = 1
            else:
                # Мертвая клетка оживает, если у нее ровно 3 соседа
                if neighbors == 3:
                    new_grid[y][x] = 1
    return new_grid

def main():
    width = 40
    height = 20
    generations = 100
    delay = 0.2

    grid = create_grid(width, height)

    try:
        for _ in range(generations):
            print_grid(grid)
            grid = next_generation(grid)
            time.sleep(delay)
    except KeyboardInterrupt:
        print("Игра остановлена пользователем.")

if __name__ == "__main__":
    main()
