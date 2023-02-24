#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

double processLine() {
    double x1, y1, x2, y2;
    scanf("%lf %lf %lf %lf", &x1, &y1, &x2, &y2);
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

char equal(double a, double b) {
    return fabs(a - b) < (fabs(a) + fabs(b)) * DBL_EPSILON;
}

int main(void) {

    while (1) {
        uint32_t lines;
        scanf("%u", &lines);
        if (lines == 0) {
            break;
        }

        uint32_t longestCount = 0;
        double longestLength = 0;

        for (uint32_t i = 0; i < lines; ++i) {
            double length = processLine();
            if (equal(length, longestLength)) {
                longestCount++;
            } else if (length > longestLength) {
                longestLength = length;
                longestCount = 1;
            } else {
            }
        }

        printf("%u usecek ma delku %.2lf.\n", longestCount, longestLength);
    }

    return 0;
}

