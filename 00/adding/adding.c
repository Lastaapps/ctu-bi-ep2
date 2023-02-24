#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t trim(uint64_t a) {
    // while (a % 10 == 0) {
    //     a /= 10;
    // }
    return a;
}

uint64_t rotate(uint64_t a) {
    uint64_t out = 0;
    while(a != 0) {
        out *= 10;
        out += a % 10;
        a /= 10;
    }
    return out;
}

int main(void) {
    
    uint32_t cases;
    scanf("%u", &cases);

    for (uint32_t i = 0; i < cases; ++i) {
        uint64_t x, y, r;
        scanf("%lu %lu", &x, &y);
        x = rotate(trim(x));
        y = rotate(trim(y));
        r = rotate(trim(x + y));
        printf("%lu\n", r);
    }

    return 0;
}

