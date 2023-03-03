#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

const uint8_t S_BACKGROUND = '.';
const uint8_t S_DASH = '*';
const uint8_t S_DOT = 'X';

typedef struct Board_t {
    uint8_t * data;
    uint16_t w, h;
} Board;

typedef struct Found_t {
    uint8_t * data;
    uint32_t count;
} Found;

int compare_ints(const void* a, const void* b) {
    uint8_t arg1 = *(const uint8_t*)a;
    uint8_t arg2 = *(const uint8_t*)b;
 
    return (arg1 > arg2) - (arg1 < arg2); // possible shortcut
}


void readInput(Board * board) {
    for (uint32_t j = 1 ; j < board -> h + 1u; ++j) {
        getchar(); // ship new line

        for (uint32_t i = 1 ; i < board -> w + 1u; ++i) {

            const uint32_t index = board -> w * j + i;
            board -> data[index] = getchar();
        }
    }
}

void vaporizeDot(Board * board, uint16_t x, uint16_t y) {
    const uint32_t index = board -> w * y + x;
    const uint8_t point = board -> data[index];

    if (point == S_DOT) {
        board -> data[index] = S_DASH;
        vaporizeDot(board, x + 1, y + 0);
        vaporizeDot(board, x - 1, y + 0);
        vaporizeDot(board, x + 0, y + 1);
        vaporizeDot(board, x + 0, y - 1);
    }
}

uint32_t fillDash(Board * board, uint16_t x, uint16_t y) {
    const uint32_t index = board -> w * y + x;
    const uint8_t point = board -> data[index];
    uint8_t dotFound = 0;

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
    Found found = {(uint8_t*) malloc(1000000), 0};

    for (uint32_t j = 1 ; j < board -> h + 1u; ++j) {
        for (uint32_t i = 1 ; i < board -> w + 1u; ++i) {

            const uint32_t index = board -> w * j + i;
            const int16_t  point = board -> data[index];

            if (point != S_BACKGROUND) {
                found.data[found.count++] = fillDash(board, i, j);
            }
        }
    }

    qsort(found.data, found.count, sizeof(found.data[0]), compare_ints);
    return found;
}

int process() {
    uint16_t w, h;
    scanf("%hu %hu", &h, &w);
    if (w == 0) {
        return 0;
    }

    uint32_t size;
    uint8_t * arr = (uint8_t*) malloc(
            size = (w + 2) * (h + 2) * sizeof(*arr)
            );
    memset(arr, S_BACKGROUND, size);

    Board board = {arr, w, h};
    readInput(&board);

    Found found = findDashes(&board);

    printf("Throw:");
    for (uint32_t i = 0; i < found.count; ++i) {
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

