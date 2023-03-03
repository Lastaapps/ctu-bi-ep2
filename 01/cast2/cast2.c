#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

const char S_BACKGROUND = '.';
const char S_DASH = '*';
const char S_DOT = 'X';

typedef struct Board_t {
    char * data;
    int w, h;
} Board;

typedef struct Found_t {
    char * data;
    int count;
} Found;

int compare_ints(const void* a, const void* b) {
    char arg1 = *(const char*)a;
    char arg2 = *(const char*)b;
 
    return (arg1 > arg2) - (arg1 < arg2); // possible shortcut
}

#define mIndex(board, x, y) (((board) -> w + 2) * (y) + (x) + 1)

void readInput(Board * board) {
    for (int j = 1 ; j < board -> h + 1; ++j) {
        getchar(); // ship new line

        for (int i = 1 ; i < board -> w + 1; ++i) {

            const int index = mIndex(board, i, j);
            board -> data[index] = getchar();
        }
    }
}

void vaporizeDot(Board * board, int x, int y) {
    const int index = mIndex(board, x, y);
    const char point = board -> data[index];

    if (point == S_DOT) {
        board -> data[index] = S_DASH;
        vaporizeDot(board, x + 1, y + 0);
        vaporizeDot(board, x - 1, y + 0);
        vaporizeDot(board, x + 0, y + 1);
        vaporizeDot(board, x + 0, y - 1);
    }
}

uint32_t fillDash(Board * board, int x, int y) {
    const int index = mIndex(board, x, y);
    const char point = board -> data[index];
    char dotFound = 0;

    switch (point) {
        case S_DASH:
            break;
        case S_DOT:
            dotFound = 1;
            vaporizeDot(board, x, y);
            break;
        case S_BACKGROUND:
            return 0;
    }            

    board -> data[index] = S_BACKGROUND;
    return dotFound
        + fillDash(board, x + 1, y + 0)
        + fillDash(board, x - 1, y + 0)
        + fillDash(board, x + 0, y + 1)
        + fillDash(board, x + 0, y - 1);
}

Found findDashes(Board * board) {
    Found found = {(char*) malloc(1000000), 0};

    for (int j = 1 ; j < board -> h + 1; ++j) {
        for (int i = 1 ; i < board -> w + 1; ++i) {

            const int index = mIndex(board, i, j);
            const char  point = board -> data[index];

            if (point != S_BACKGROUND) {
                found.data[found.count++] = fillDash(board, i, j);
            }
        }
    }

    qsort(found.data, found.count, sizeof(found.data[0]), compare_ints);
    return found;
}

int process() {
    int w, h;
    scanf("%d %d", &h, &w);
    if (w == 0) {
        return 0;
    }

    int size;
    char * arr = (char*) malloc(size = (w + 2) * (h + 2) * sizeof(*arr));
    memset(arr, S_BACKGROUND, size);

    Board board = {arr, w, h};
    readInput(&board);

    Found found = findDashes(&board);

    printf("Throw:");
    for (int i = 0; i < found.count; ++i) {
        printf(" %u", found.data[i]);
    }
    printf("\n");

    free(arr);
    free(found.data);
    return 1;
}

int main(void) {
    while(process());

    return 0;
}

