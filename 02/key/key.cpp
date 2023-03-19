#include <algorithm>
#include <cstdio>
#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <string>
#include <array>
#include <bitset>

const char WALL = '#';

const int MAP_SIZE = 102 + 2;
using Map   = std::array<char, MAP_SIZE * MAP_SIZE>;
using Point = std::pair<int, int>;
using Mask  = uint8_t;
using State = std::pair<Point, Mask>;

char& atPos(Map& map, int x, int y) {
    return map[MAP_SIZE * y + x];
}

Point loadMap(Map& map, int w, int h) {
    int sw, sh;
    for (int i = 1; i < h + 1; ++i) {
        std::getchar(); // \n
        for (int j = 1; j < w + 1; ++j) {
            const char c = getchar();
            if (c == '*') {
                sw = j;
                sh = i;
            }
            atPos(map, j, i) = c;
        }
    }

    return {sw, sh};
}

int distIndex(int w, int h, int area, int x, int y, Mask mask) {
    return y * (w + 2) + x + mask * area;
}

bool solve() {
    int w, h;
    scanf("%d %d", &h, &w);
    if (w == 0) {
        return false;
    }

    Map map;
    std::fill(map.begin(), map.end(), WALL);
    const auto [startX, startY] = loadMap(map, w, h);

    std::queue<State> queue;
    const int area = (w + 2) * (h + 2);
    std::vector<int> distances(area * (1 << 4));

    Mask masks['Y' - 'B' + 1];
    masks['B' - 'B'] = 1 << 0;
    masks['Y' - 'B'] = 1 << 1;
    masks['R' - 'B'] = 1 << 2;
    masks['G' - 'B'] = 1 << 3;

    const auto enqueue = [&map, &distances, &queue, w, h, area, &masks](int x, int y, int dist, Mask mask) {
        const char me = atPos(map, x, y);
        int index = distIndex(w, h, area, x, y, mask);
        const int mdist = distances[index];

        if (mdist != 0) {
            return;
        }

        switch (me) {
            case WALL:
                return;

                // door
            case 'B':
            case 'Y':
            case 'R':
            case 'G':
                if (!(masks[me - 'B'] & mask)) {
                    return;
                }
                break;

                // key
            case 'b':
            case 'y':
            case 'r':
            case 'g':
                mask |= masks[me - 'b'];
                index = distIndex(w, h, area, x, y, mask);
                break;
        }

        distances[index] = dist + 1;
        queue.push({{x, y}, mask});
    };

    enqueue(startX, startY, 1, 0);

    while(!queue.empty()) {
        const auto [point, mask] = queue.front();
        const auto [x, y] = point;
        queue.pop();

        const int index = distIndex(w, h, area, x, y, mask);
        const int mdist = distances[index];

        if (atPos(map, x, y) == 'X') {
            std::printf("Escape possible in %d steps.\n", mdist - 2);
            return true;
        }

        enqueue(x + 1, y + 0, mdist, mask);
        enqueue(x - 1, y + 0, mdist, mask);
        enqueue(x + 0, y + 1, mdist, mask);
        enqueue(x + 0, y - 1, mdist, mask);
    }

    std::printf("The poor student is trapped!\n");
    return true;
}

int main(void) {
    while(solve());
    return 0;
}

