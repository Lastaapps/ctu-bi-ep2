#include <functional>
#include <iostream>
#include <vector>
#include <tuple>
#include <queue>

typedef struct Dest_t {
    size_t time;
    size_t to;
    bool isTram;
} Dest;

using DestVector = std::vector<std::vector<Dest>>;

template<bool isTram>
void readConnection(DestVector& vector) {
    const size_t multiplier = isTram ? 2 : 15;

    size_t cases;
    std::cin >> cases;
    for (size_t i = 0; i < cases; ++i) {
        size_t from, to, distance;
        std::cin >> from >> to >> distance;
        vector[from].emplace_back(Dest{distance * multiplier, to, isTram});
    }
}

std::tuple<DestVector, size_t, size_t> readGraph() {
    size_t stationCnt, from, to;
    std::cin >> stationCnt >> from >> to;
    DestVector data(stationCnt);

    readConnection<false>(data);
    readConnection<true> (data);

    return {std::move(data), from, to};
}

void solve() {
    const auto [graph, from, to] = readGraph();
    const size_t total = graph.size();

    std::vector<size_t> dists(total * 2);
    using QueueItem = std::tuple<size_t, size_t, bool>; // <time, station, used tram>
    std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<QueueItem>> queue;
    queue.push({0, from, false});

    while(!queue.empty()) {
        auto [time, station, wasTram] = queue.top();
        queue.pop();

        if (dists[station + (wasTram ? total : 0)] <= time) {
            continue;
        }

        if (station == to) {
            std::cout << "Za " << time << " min tam budu." << std::endl;
            return;
        }

        for (auto& neighbour : graph[station]) {
            if (wasTram && neighbour.isTram) continue;

            const size_t newTime = time + neighbour.time;
            if (dists[neighbour.to + (neighbour.isTram ? total : 0)] <= newTime) {
                continue;
            }
            queue.push({newTime, neighbour.to, neighbour.isTram});
        }
    }

    std::cout << "O beda, o beda, co ja budu ted delat?" << std::endl;
}

/*
 * Edge cases
 * no path
 * trams only
 * to park and back
 * multigraph
 * same station
 */
int main(void) {
    int cases;
    std::cin >> cases;

    for (int i = 0; i < cases; ++i) {
        solve();
    }
}
