#include <cstdio>
#include <iostream>
#include <vector>
#include <queue>

const char NOPE = 0;
const char RED = 1;
const char BLACK = ~RED;

bool solve() {
    int n, e;
    scanf("%d", &n);
    if (n == 0) {
        return false;
    }

    scanf("%d", &e);

    std::vector<std::vector<int>> graph(n);
    for (int i = 0; i < e; ++i) {
        int x, y;
        std::scanf("%d %d", &x, &y);
        graph[x].push_back(y);
    }


    std::queue<int> queue;
    std::vector<char> colors(n, NOPE);

    // so you can be happy about the code
    auto enqueue = [&queue, &colors](int point, char color) {
        const char current = colors[point];
        switch(current) {
            case NOPE:
                break;
            case RED:
                if (color != RED) {
                    return false;
                }
                return true;
            case BLACK:
                if (color != BLACK) {
                    return false;
                }
                return true;
        }
        colors[point] = color;
        queue.push(point);
        return true;
    };

    for (int i = 0; i < n; ++i) {
        if (colors[i] != NOPE) {
            continue;
        }
        enqueue(i, RED);

        while(!queue.empty()) {
            const int point = queue.front();
            queue.pop();
            const char color = colors[point];
            const char next = ~color;

            for (int n : graph[point]) {
                if (!enqueue(n, next)) {
                    std::printf("NO\n");
                    return true;
                }
            }
        }
    }
    std::printf("YES\n");


    return true;
}

int main() {

    while(solve());

    return 0;
}

