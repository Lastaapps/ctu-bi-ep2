#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <string>

using Breakpoints= std::vector<uint32_t>;
using Polynom = std::array<uint32_t, 6>;
using Leves = std::array<uint32_t, 6>;
using Indexes = std::array<uint32_t, 6>;
const uint32_t MAX_LEVELS = 32767;

Breakpoints breakpoints;

void fillLevels() {
    breakpoints.reserve(300);

    uint32_t acu = 0;
    uint32_t level = 0;
    breakpoints.push_back(0);
    while (acu <= MAX_LEVELS) {
        breakpoints.push_back(acu + 1);
        level += 1;
        acu += level;
    }
}

void printArray(char * name, const std::array<uint32_t, 6>& arr) {
    std::cout << name << ": " << arr[0] << ", " << arr[1] << ", " << arr[2] << ", " << arr[3] << ", " << arr[4] << ", " << arr[5] << std::endl;
}

std::pair<uint32_t, uint32_t> pointLevel(uint32_t point) {
    const auto itr = std::lower_bound(breakpoints.begin(), breakpoints.end(), point + 1);
    const auto distance = std::distance(breakpoints.begin(), itr);
    return {distance, point - breakpoints[distance - 1]};
}

template<std::size_t C>
std::pair<Leves, Indexes> toLevels(Polynom& polynom) {
    Leves levels;
    Indexes indexes;
    for (uint32_t i = 0; i < C; ++i) {
        auto [level, index] = pointLevel(polynom[i]);
        levels[i] = level;
        indexes[i] = index;
    }
    return {levels, indexes};
}

template<std::size_t C>
Polynom sortedPoints(const Polynom& polynom) {
    Polynom sorted = polynom;
    std::sort(sorted.begin(), sorted.begin() + C);
    return sorted;
}

bool checkTriangleLevel(uint32_t x1, uint32_t x2, uint32_t mLevel, uint32_t tLevel, bool above) {
    uint32_t dist = (x2 - x1) * (above ? -1 : 1);
    return mLevel + dist == tLevel;
}

bool processTriangle(Polynom& triangle) {
    Polynom sorted = sortedPoints<3>(triangle);
    auto [levels, indexes] = toLevels<3>(sorted);

    return (levels[0] == levels[1] &&
        checkTriangleLevel(sorted[0], sorted[1], levels[0], levels[2], false)
            && indexes[1] == indexes[2])
        ||
    (levels[1] == levels[2] &&
        checkTriangleLevel(sorted[1], sorted[2], levels[1], levels[0], true)
            && indexes[0] == indexes[1])
     ;
}

bool processParallel(Polynom& parallel) {
    Polynom sorted = sortedPoints<4>(parallel);
    auto [levels, indexes] = toLevels<4>(sorted);

    if (levels[0] == levels[1] && levels[2] == levels[3]) {
        uint32_t dist1 = sorted[1] - sorted[0];
        uint32_t dist2 = sorted[3] - sorted[2];
        uint32_t dist3 = levels[2] - levels[0];
        bool check4 = indexes[0] == indexes[2] || indexes[1] == indexes[2] ;
        return dist1 == dist2 && dist2 == dist3 && check4;
    }

    return (levels[1] == levels[2] &&
        checkTriangleLevel(sorted[1], sorted[2], levels[1], levels[0], true)
            && checkTriangleLevel(sorted[1], sorted[2], levels[1], levels[3], false)
            && indexes[0] == indexes[1]
            && indexes[2] == indexes[3]);
}

bool processHexagon(Polynom& hexagon) {
    Polynom sorted = sortedPoints<6>(hexagon);
    auto [levels, indexes] = toLevels<6>(sorted);

    bool check1 = levels[0] == levels[1];
    bool check2 = levels[2] == levels[3];
    bool check3 = levels[4] == levels[5];
    uint32_t dist1 = sorted[1] - sorted[0];
    uint32_t dist2 = sorted[3] - sorted[2];
    uint32_t dist3 = sorted[5] - sorted[4];
    bool check4 = dist1 == dist3 && dist1 * 2 == dist2;
    uint32_t level1 = levels[2] - levels[0];
    uint32_t level2 = levels[4] - levels[2];
    bool check5 = level1 == level2;
    bool check6 = level1 == dist1;
    bool check7 = indexes[0] == indexes[2];
    bool check8 = indexes[1] == indexes[4];

    return check1 && check2 && check3 && check4 && check5 && check6 && check7 && check8;
}

bool processInput() {
    char * line = nullptr; size_t len;
    ssize_t ret = getline(&line, &len, stdin);
    if (ret < 0) {
        free(line);
        return 0;
    }

    Polynom poly = {0};
    ssize_t read = sscanf(
            line, "%u %u %u %u %u %u", 
            &poly[0], &poly[1], &poly[2], &poly[3], &poly[4], &poly[5]
            );
    free(line);

    switch(read) {
        case 3:
            if (processTriangle(poly)) {
                printf("%d %d %d are the vertices of a triangle\n", poly[0], poly[1], poly[2]);
                return true;
            }
            break;
        case 4:
            if (processParallel(poly)){
                printf("%d %d %d %d are the vertices of a parallelogram\n", poly[0], poly[1], poly[2], poly[3]);
                return true;
            }
            break;
        case 6:
            if (processHexagon(poly)) {
                printf("%d %d %d %d %d %d are the vertices of a hexagon\n", poly[0], poly[1], poly[2], poly[3], poly[4], poly[5]);
                return true;
            }
            break;
    }

    {
        uint8_t i = 0;
        switch(read) {
            case 6: printf("%d ", poly[i++]);
            case 5: printf("%d ", poly[i++]);
            case 4: printf("%d ", poly[i++]);
            case 3: printf("%d ", poly[i++]);
            case 2: printf("%d ", poly[i++]);
            case 1: printf("%d ", poly[i++]);
        }
        printf("are not the vertices of an acceptable figure\n");
    }
    return true;
}

void test() {
    for (uint32_t i = 1; i <= 42; ++i) {
        auto [level, index] = pointLevel(i);
        std::cout << i << ": " << level << ", " << index << std::endl;
    }
}

int main() {
    fillLevels();

    // test();

    while(processInput());
    return 0;
}

