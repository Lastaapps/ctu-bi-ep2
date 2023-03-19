#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

bool solve() {
    int cnt;
    std::scanf("%d", &cnt);
    if (cnt == 0) {
        return false;
    }

    std::unordered_map<std::string, int> mapping;
    std::vector<std::string> reverse(cnt);

    int idCnt = 0;
    for (int i = 0; i < cnt; ++i) {
        std::string name;
        const int id = idCnt++;
        std::cin >> name;
        mapping.emplace(name, id);
        reverse[id] = name;
    }

    int edgesCnt;
    scanf("%d", &edgesCnt);
    std::vector<std::vector<int>> edges(cnt);
    std::vector<int> degrees(cnt);

    for (int i = 0; i < edgesCnt; ++i) {
        std::string lower, greater;
        std::cin >> lower >> greater;

        int id1 = mapping[lower];
        int id2 = mapping[greater];

        edges[id1].push_back(id2);
        ++degrees[id2];
    }

    static int caseNum = 0;
    printf("Case #%d: Dilbert should drink beverages in this order:", ++caseNum);

    std::priority_queue<int, std::vector<int>, std::greater<int>> topQueue;
    for (int i = 0; i < cnt; ++i) {
        if (degrees[i] == 0) {
            topQueue.push(i);
            // printf("Preparing: %d\n", i);
        }
    }

    while(!topQueue.empty()) {
        int point = topQueue.top();
        topQueue.pop();
        // printf("Poping: %d\n", point);
        std::printf(" %s", reverse[point].c_str());

        for (auto n : edges[point]) {
            // printf("Decing: %d\n", n);
            if (--degrees[n] == 0) {
                // printf("Pushing: %d\n", n);
                topQueue.push(n);
            }
        }
    }
    printf(".\n\n");

    return true;
}

int main() {
    while(solve());
    return 0;
}
