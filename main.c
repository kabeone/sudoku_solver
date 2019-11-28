#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

case_t **create_board(char *file_name)
{
    FILE * file;
    size_t a = 0;
    char *str = NULL;
    case_t **board;
    int n;
    int j = 0;
    int i;

    board = malloc(9 * sizeof(case_t *));
    if (board == NULL)
        exit(1);
    file = fopen(file_name, "r");

    while(getline(&str, &a, file) != EOF) {
        n = 0;
        board[j] = malloc(9 * sizeof(case_t));
        if (board[j] == NULL)
            exit(1);
        if (strlen(str) != 10)
            exit(1);
        while (str[n] != '\n') {
            if (str[n] == ' ') {
                board[j][n].value = 0;
                i = 0;
                while (i < 9) {
                    board[j][n].array[i] = true;
                    i++;
                }
            }
            else
                board[j][n].value = str[n] - 48;
            n++;
        }
        if (j >= 9)
            exit(1);
        j++;
        free(str);
        str = NULL;
    }
    if (j < 9)
        exit(1);
    return board;
}

void set_line(int value, case_t **board, int line)
{
    for (int i = 0; i < 9; i++)
        board[line][i].array[value - 1] = false;
}

void set_column(int value, case_t **board, int column)
{
    for (int i = 0; i < 9; i++)
        board[i][column].array[value - 1] = false;
}

int get_region(int line, int column)
{
    int region = 0;

    if (column < 3)
        region = 0;
    else if (column < 6)
        region = 1;
    else
        region = 2;

    if (line < 3)
        region += 0;
    else if (line < 6)
        region += 3;
    else
        region += 6;

    return region;
}

void set_region(int value, case_t **board, int line, int column)
{
    int region;

    region = get_region(line, column);

    line = (region / 3) * 3;
    column = (region % 3) * 3;

    board[line][column].array[value - 1] = false;
    board[line][column + 1].array[value - 1] = false;
    board[line][column + 2].array[value - 1] = false;
    board[line + 1][column].array[value - 1] = false;
    board[line + 1][column + 1].array[value - 1] = false;
    board[line + 1][column + 2].array[value - 1] = false;
    board[line + 2][column].array[value - 1] = false;
    board[line + 2][column + 1].array[value - 1] = false;
    board[line + 2][column + 2].array[value - 1] = false;
}

void set_case(int value, case_t ** board, int line, int column)
{
    set_region(value, board, line, column);
    set_column(value, board, column);
    set_line(value, board, line);
}

int check_cases(case_t **board)
{
    int line = 0;
    int column;
    int i;
    int a = 0;
    int check = 0;

    while (line < 9) {
        column = 0;
        while (column < 9) {
            if (board[line][column].value != 0)
                set_case(board[line][column].value, board, line, column);
            else {
                check++;
                i = 0;
                a = 0;
                while (i < 9) {
                    if (board[line][column].array[i] == true)
                        a++;
                    i++;
                }
                if (a == 0){
                    printf("error: sudoku invalide\n");
                    exit(1);
                }
                if (a == 1){
                    i = 0;
                    while (board[line][column].array[i] != true)
                        i++;
                    board[line][column].value = i + 1;
                }

            }
            column++;
        }
        line++;
    }
    return check;
}

void print_bool_array(case_t case_var)
{
    for(int i = 0; i < 9; i++) {
        printf("%d: ", i + 1);
        if (case_var.array[i] == false)
            printf("false\n");
        else
            printf("true\n");
    }
}

void print_sudoku(case_t **board)
{
    int i = 0;
    int j = 0;

    while(i < 9) {
        j = 0;
        while (j < 9) {
            if (board[i][j].value == 0)
                printf("_   ");
            else
                printf("%d   ", board[i][j].value);
            j++;
        }
        printf("\n");
        printf("\n");
        i++;
    }
}

void free_board(case_t **board)
{
    for (int i = 0; i < 9; i++)
        free(board[i]);
    free(board);
}

int main(int argc, char **argv)
{
    int n = 0;
    case_t **board;

    if (argc < 2)
        exit(1);

    board = create_board(argv[1]);
    print_sudoku(board);
    n = check_cases(board);

    while (n != 0)
        n = check_cases(board);
    printf("\n");
    printf("\n");
    printf("\n");
    print_sudoku(board);
    free_board(board);
    return 0;
}