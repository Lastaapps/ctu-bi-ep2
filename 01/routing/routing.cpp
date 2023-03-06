#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

typedef std::vector<uint32_t> Breakpoints;
Breakpoints breakpoints;

typedef std::pair<int32_t, int32_t> Point;
// typedef long long unsigned int uint128_t;
typedef __int128 uint128_t;

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

uint128_t alternatives(uint32_t x, uint32_t y) {
    const uint32_t sum = x + y;
    const uint32_t some = x;
    uint128_t acu = 1;
    // TODO check of overflow
    for (uint32_t i = some + 1; i <= sum; ++i) {
        acu *= i;
    }
    return acu / (sum - some);
}

std::pair<uint32_t, uint128_t> distance(const Point& from, const Point& to) {

    std::printf("From %d %d\n", from.first, from.second);
    std::printf("To   %d %d\n", to.first, to.second);

    int32_t diffX = to.first - from.first;
    int32_t diffY = to.second - from.second;

    bool isFine = (diffX * diffY) >= 0;
    if (isFine) {
        std::printf("Fine %d %d\n", diffX, diffY);
        uint128_t alt = alternatives(abs(diffX), abs(diffY));
        return {abs(diffX) + abs(diffY), alt};
    }

    uint32_t walked = abs(diffX);
    int32_t newDiag = from.second - diffX;
    int32_t toWalk = to.second - newDiag;

    std::printf("Not fine; walked: %d, newDiag: %d, toWalk: %d\n", walked, newDiag, toWalk);

    uint128_t alt;
    if (toWalk > 0) {
        alt = alternatives(walked, toWalk);
    } else if (toWalk == 0) {
        alt = 1;
    } else {
        walked += toWalk;
        toWalk = abs(toWalk);
        alt = alternatives(walked, toWalk);
    }

    return {walked + abs(toWalk), alt};
}

bool process() {
    uint32_t from, to;
    scanf("%u %u", &from, &to);
    if (from == 0) { return false; }

    std::printf("Processing %d %d\n", from, to);
    Point pointFrom = coordinates(from);
    Point pointTo = coordinates(to);
    
    auto dists = distance(pointFrom, pointTo);
    auto dist = dists.first;
    auto alt = dists.second;

    if (alt == 1) {
        std::printf("There is %llu route of the shortest length %u.\n", alt, dist);
    } else {
        std::printf("There is %llu routes of the shortest length %u.\n", alt, dist);
    }
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

