#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

typedef std::vector<uint32_t> Breakpoints;
Breakpoints breakpoints;

typedef std::pair<int32_t, int32_t> Point;

void computeReakpoints() {
    const uint32_t max = 1'000'000;

    uint32_t level = 0;
    uint32_t acu = 0;
    do {
        // printf("Breakpoint: %d\n", acu);
        breakpoints.push_back(acu);
        level += 6;
        acu += level;
    } while (breakpoints[breakpoints.size()-1] < max);

    // printf("Total breakpoints %lu\n", breakpoints.size());
}

uint32_t levelOf(uint32_t point) {
    const auto itr = std::lower_bound(breakpoints.begin(), breakpoints.end(), point);
    const uint32_t distance = std::distance(breakpoints.begin(), itr);
    return distance;
}

Point coordinates(const uint32_t point) {
    if (point == 1) return {0, 0};

    const uint32_t level = levelOf(point - 1);
    const uint32_t levelStart = breakpoints[level - 1] + 1;
    const uint32_t itemsInLevel = breakpoints[level] - breakpoints[level - 1];
    const uint32_t chunkSize = itemsInLevel / 6;
    const uint32_t indexInLevel = point - levelStart - 1;
    const uint32_t indexInChunk = indexInLevel % chunkSize;
    const uint32_t chunkNumber = indexInLevel / chunkSize;
    const uint32_t isCorner = indexInChunk == chunkSize - 1;
    
    int32_t vertical;
    int32_t diagonal;

    switch (chunkNumber) {
        case 0: 
        case 3: 
            vertical = indexInChunk + 1;
            break;
        case 1: 
        case 4: 
            vertical = chunkSize;
            break;
        case 2: 
        case 5: 
            vertical = chunkSize - indexInChunk - 1;
            break;
    }
    if (chunkNumber < 3) {
        vertical *= -1;
    }

    switch (chunkNumber) {
        case 5: 
        case 2: 
            diagonal = indexInChunk + 1;
            break;
        case 0: 
        case 3: 
            diagonal = chunkSize;
            break;
        case 1: 
        case 4: 
            diagonal = chunkSize - indexInChunk - 1;
            break;
    }
    if (!(chunkNumber < 2 || chunkNumber == 5)) {
        diagonal *= -1;
    }

    // std::cout
    //     << "Item: " << point << ", "
    //     << "lvl: " << level << ", "
    //     << "str: " << levelStart << ", "
    //     << "cnt: " << itemsInLevel << ", "
    //     << "chnk: " << chunkSize << ", "
    //     << "lvlInd: " << indexInLevel << ", " 
    //     << "chkInd: " << indexInChunk << ", " 
    //     << "chNum: " << chunkNumber << ", " 
    //     << "corn: " << isCorner << ", " 
    //     << "vert: " << vertical << ", " 
    //     << "diag: " << diagonal << ", " 
    //     << std::endl;

    return {vertical, diagonal};
}

uint32_t distance(const Point& from, const Point& to) {
    if (from.first > to.first) {
        return distance(to, from);
    }

    int32_t diffX = from.first - to.first;
    int32_t diffY = from.second - to.second;

    bool isFine = (diffX * diffY) >= 0;
    if (isFine) {
        return abs(diffX) + abs(diffY);
    }

    uint32_t walked = abs(diffX);
    int32_t toWalk = to.second - walked;
    if (toWalk < 0) {
        toWalk = 0;
    }
    return walked + toWalk;
}

bool process() {
    uint32_t from, to;
    scanf("%u %u", &from, &to);
    if (from == 0) { return false; }

    Point pointFrom = coordinates(from);
    Point pointTo = coordinates(to);
    
    uint32_t dist = distance(pointFrom, pointTo);

    std::printf("%d\n", dist);
    return true;
}

int main() {
    computeReakpoints();
    // for (uint32_t i = 1; i < 38; ++i) {
    //     coordinates(i);        
    // }
    while (process());
    return 0;
}

