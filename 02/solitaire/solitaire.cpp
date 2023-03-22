#include <cstdio>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <algorithm>

const int PROBLEM_SIZE = 4;
const int STEPS_NEEDED = 4;

using Position = std::pair<char, char>;
using Config = std::array<Position, PROBLEM_SIZE>;

Position operator+(const Position& l, const Position& r) { 
    return {l.first + r.first, l.second + r.second};                                    
}

void operator+=(Position& l, const Position& r) { 
    l.first  += r.first;
    l.second += r.second;
}

int toIndex(Config config) {
    std::sort(config.begin(), config.end());
    return  0
        + config[0].first  * 1
        + config[0].second * 8
        + config[1].first  * 8 * 8
        + config[1].second * 8 * 8 * 8
        + config[2].first  * 8 * 8 * 8 * 8
        + config[2].second * 8 * 8 * 8 * 8 * 8
        + config[3].first  * 8 * 8 * 8 * 8 * 8 * 8
        + config[3].second * 8 * 8 * 8 * 8 * 8 * 8 * 8
        ;
}

bool readLine(Config& config) {
    for (int i = 0; i < PROBLEM_SIZE; ++i) {
        int x, y;
        if (scanf("%d %d", &x, &y) < 0) {
            return false;
        }
        config[i] = {x - 1, y - 1};
    }

    return true;
}

void enqueueWithDelta(const Config& config, const char i, const char dist, std::vector<char>& visited, std::queue<Config>& queue, const Position& delta) {
    Position newPos = config[i] + delta;

    if (std::any_of(config.begin(), config.end(), [&newPos](const Position& pos){ return pos == newPos; })) {
        newPos += delta;

        if (std::any_of(config.begin(), config.end(), [&newPos](const Position& pos){ return pos == newPos; })) {
            return;
        }
    }

    if (newPos.first < 0 || newPos.first >= 8 || newPos.second < 0 || newPos.second >= 8) {
        return;
    }

    Config newConfig = config;
    newConfig[i] = newPos;
    const int nextIndex = toIndex(newConfig);
    if (visited[nextIndex] != 0) {
        return;
    }
    visited[nextIndex] = dist + 1;
    queue.push(newConfig);
}

void enqueueNeibours(const Config& config, const char dist, std::vector<char>& visited, std::queue<Config>& queue) {
    if (dist >= STEPS_NEEDED + 1) return;

    for (int i = 0; i < PROBLEM_SIZE; ++i) {
        enqueueWithDelta(config, i, dist, visited, queue, { 1,  0});
        enqueueWithDelta(config, i, dist, visited, queue, {-1,  0});
        enqueueWithDelta(config, i, dist, visited, queue, { 0,  1});
        enqueueWithDelta(config, i, dist, visited, queue, { 0, -1});
    }
}

bool solve() {
    Config from, to;
    if (!readLine(from)) return false;
    readLine(to);

    std::vector<char> visitedFrom(8 * 8 * 8 * 8 * 8 * 8 * 8 * 8);
    std::vector<char> visitedTo  (8 * 8 * 8 * 8 * 8 * 8 * 8 * 8);

    {
        std::queue<Config> queue;
        queue.push(from);
        visitedFrom[toIndex(from)] = 1;

        while(!queue.empty()) {
            const Config config = queue.front();
            queue.pop();

            const int index = toIndex(config);
            const char dist = visitedFrom[index];

            // std::cout << "Fr: " << (int) dist << " ";
            // for (int i = 0; i < 4; ++i) {
            //     std::cout << "[" << (int) config[i].first <<  "," << (int) config[i].second << "] ";
            // }
            // std::cout << std::endl;

            enqueueNeibours(config, dist, visitedFrom, queue);
        }
    }

    {
        std::queue<Config> queue;
        queue.push(to);
        visitedTo[toIndex(to)] = 1;

        while(!queue.empty()) {
            const Config config = queue.front();
            queue.pop();

            const int index = toIndex(config);
            const char dist = visitedTo[index];

            // std::cout << "To: " << (int) dist << " ";
            // for (int i = 0; i < 4; ++i) {
            //     std::cout << "[" << (int) config[i].first <<  "," << (int) config[i].second << "] ";
            // }
            // std::cout << std::endl;

            if (visitedFrom[index] != 0) {
                printf("YES\n");
                return true;
            }

            enqueueNeibours(config, dist, visitedTo, queue);
        }
    }

    printf("NO\n");
    return true;
}

int main(void) {
    while(solve());
    return 0;
}

