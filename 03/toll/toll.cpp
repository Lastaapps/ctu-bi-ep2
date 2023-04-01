#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>

const size_t charCnt = 'z' - 'a' + 1;

int toIndex(const char c) {
    if ('a' <= c && c <= 'z') {
        return c - 'a';
    } else {
        return c - 'A' + charCnt;
    }
}

char toChar(const int ind) {
    if (ind < (int) charCnt) {
        return 'a' + ind;
    } else {
        return 'A' + ind - charCnt;
    }
}

bool isTown(int index) {
    return index >= (int) charCnt;
}

int requiredToEnter(int index, int cnt) {
    if (isTown(index)) {
        return (cnt - 1) / 19 + 1 + cnt;
    } else {
        return cnt + 1;
    }
}

void addEdge(std::vector<int>& v, int i) {
    auto itr = std::find(v.begin(), v.end(), i);
    if (itr == v.end()) {
        v.push_back(i);
    }
}

bool solve(int caseCnt) {
    int edges;
    scanf(" %d", &edges);
    if (edges == -1) return false;

    std::vector<std::vector<int>> graph(2 * charCnt);

    for (int i = 0; i < edges; ++i) {
        char f, t;
        scanf(" %c %c", &f, &t);
        const int fi = toIndex(f), ti = toIndex(t);
        addEdge(graph[fi], ti);
        addEdge(graph[ti], fi);
    }

    char startChar, endChar;
    int price;
    scanf(" %d %c %c", &price, &startChar, &endChar);
    const int start = toIndex(startChar), end = toIndex(endChar);

    using QItem = std::pair<int, int>;
    std::priority_queue<QItem, std::vector<QItem>, std::greater<QItem>> queue;
    queue.push({requiredToEnter(end, price), end});
    std::vector<bool> visited(2 * charCnt);

    while(!queue.empty()) {
        auto [currPrice, currInd] = queue.top();
        queue.pop();

        if (visited[currInd]) {
            continue;
        }
        visited[currInd] = true;

        // printf("Pop:  %d %c\n", currPrice, toChar(currInd));

        if (currInd == start) {
            int p;
            if (isTown(start)) {
                p = currPrice - (currPrice + 19) / 20;
            } else {
                p = currPrice - 1;
            }

            printf("Case %d: %d\n", caseCnt, p);;
            return true;
        }

        for (auto edge : graph[currInd]) {
            int p = requiredToEnter(edge, currPrice);
            queue.push({p, edge});
            // printf("Push: %d %c\n", p, toChar(edge));
        }
    }

    return true;
}

int main(void) {
    int cnt = 1;
    while(solve(cnt++));

    return 0;
}
