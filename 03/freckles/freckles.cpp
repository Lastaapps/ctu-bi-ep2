#include <map>
#include <vector>
#include <iostream>
#include <array>
#include <cmath>
#include <limits>
#include <cfloat>

const size_t ITEMS_SIZE = 101;

using Vec2 = std::pair<double, double>;

double dist(const Vec2& a, const Vec2& b) {
    double x = a.first - b.first;
    double y = a.second - b.second;
    return std::sqrt(x * x  + y * y);
}

void solve() {
    size_t lines;
    scanf("%lu", &lines);
    std::array<Vec2, ITEMS_SIZE> read;
    std::array<std::array<double, ITEMS_SIZE>, ITEMS_SIZE> distances;

    for (size_t i = 0; i < lines; ++i) {
        Vec2& vec = read[i];
        scanf("%lf %lf", &vec.first, &vec.second);
        
        for (size_t j = 0; j <= i; ++j) {
            const Vec2& toCount = read[j];
            const double d = dist(vec, toCount);
            distances[i][j] = d;
            distances[j][i] = d;
        }
    }

    double sum = 0;
    std::array<bool, ITEMS_SIZE> used;
    std::fill(used.begin(), used.end(), false);
    std::array<double, ITEMS_SIZE> minDists = distances[0];
    used[0] = true;

    for (size_t j = 0; j < lines - 1; ++j) {

        double foundMin = std::numeric_limits<double>::max();
        size_t minIndex = 0;

        for (size_t i = 0; i < lines; ++i) {
            if (used[i] == true) {
                continue;
            }

            if (minDists[i] < foundMin) {
                foundMin = minDists[i];
                minIndex = i;
            }
        }

        used[minIndex] = true;
        sum += minDists[minIndex];
        
        std::array<double, ITEMS_SIZE>& newDists = distances[minIndex];
        for (size_t i = 0; i < lines; ++i) {
            minDists[i] = std::min(minDists[i], newDists[i]);
        }
    }

    std::printf("%.2f\n", sum);
}

int main(void) {
    int cases;
    scanf("%d", &cases);

    for (int i = 0; i < cases; ++i) {
        solve();
    }

    return 0;
}

