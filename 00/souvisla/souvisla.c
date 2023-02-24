#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t readNum() {
    uint32_t x;
    scanf("%u", &x);
    return x;
}

uint32_t max(uint32_t a, uint32_t b) {
    return a > b ? a : b;
}

int main(void) {

    uint32_t cases;
    scanf("%u", &cases);

    for (uint32_t i = 0; i < cases; ++i) {

        uint32_t longestSequence = 0;
        uint32_t length = 0;
        uint32_t lastNum = 0;

        while (1) {
            uint32_t num = readNum();

            if (num == 0) {
                longestSequence = max(longestSequence, length);
                break;
            }

            if (num >= lastNum) {
                ++length;
            } else {
                longestSequence = max(longestSequence, length);
                length = 1;
            }

            lastNum = num;
        }

        printf("%u\n", longestSequence);
    }

    return 0;
}

