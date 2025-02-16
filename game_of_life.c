#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 25
#define M 80
#define speed_multiplier 50000

int game(int matrix[N][M], int speed);
int update_matrix(int matrix[N][M]);
int is_life(int matrix[N][M], int y, int);
int count_of_living_cells(const int matrix[N][M], int y, int x);
void render(const int matrix[N][M], int speed);
int input(int matrix[N][M]);
int get_speed(int argc, char **argv, int *speed);

int main(int argc, char **argv) {
    int matrix[N][M];
    int speed;

    if (get_speed(argc, argv, &speed) == 1) {
        printf("Enter speed > 0, like ./game_of_life 2\n");
        return 0;
    }

    if (input(matrix) == 1) {
        printf("Invalid input\n");
        return 0;
    }

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    clear();

    game(matrix, speed);

    return 0;
}

int game(int matrix[N][M], int speed) {
    render(matrix, speed);
    usleep(speed * speed_multiplier);

    while (1) {
        clear();

        update_matrix(matrix);
        render(matrix, speed);
        refresh();
        usleep(speed * speed_multiplier);
    }

    endwin();
}

int update_matrix(int matrix[N][M]) {
    int arr_next_step[N][M];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            arr_next_step[i][j] = is_life(matrix, i, j);
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            matrix[i][j] = arr_next_step[i][j];
        }
    }

    return 1;
}

int is_life(int matrix[N][M], int y, int x) {
    int res = 0;
    int count = 0;
    count = count_of_living_cells(matrix, y, x);

    if (matrix[y][x] == 0 && count == 3) {
        res = 1;
    }
    if (matrix[y][x] == 1 && (count == 2 || count == 3)) {
        res = 1;
    }
    return res;
}

int count_of_living_cells(const int matrix[N][M], int y, int x) {
    int count = 0;
    int indi, indj = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            indi = (N + (y + i) % N) % N;
            indj = (M + (x + j) % M) % M;

            if (matrix[indi][indj]) {
                count++;
            }
        }
    }
    return count;
}

void render(const int matrix[N][M], int speed) {
    move(0, 0);
    for (int y = 0; y < N + 2; y++) {
        for (int x = 0; x < M + 2; x++) {
            if (y == 0 || y == N + 1) {
                mvaddch(y, x, '-');
            } else if (x == 0 || x == M + 1) {
                mvaddch(y, x, '|');
            } else if (matrix[y - 1][x - 1]) {
                mvaddch(y, x, 'X');
            } else {
                mvaddch(y, x, '.');
            }
        }
        printw("\n");
    }

    move(N + 2, 0);
    mvprintw(N + 2, 4, "Speed : %d", speed);
    mvprintw(N + 3, 4, "Ctrl + C - exit");

    refresh();
}

int input(int matrix[N][M]) {
    int a;
    int i = 0;

    while (i < N) {
        int j = 0;
        while (j < M) {
            a = getchar();
            if (a != '\n' && (a == '0' || a == '1')) {
                matrix[i][j] = a - '0';
                j++;
            } else if (a != '\n') {
                return 1;
            }
        }
        i++;
    }

    return 0;
}

int get_speed(int argc, char **argv, int *speed) {
    if (argc == 1) {
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        *speed += atoi(argv[i]);
    }

    if (*speed <= 0) {
        return 1;
    }

    return 0;
}