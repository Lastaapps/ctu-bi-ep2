#include <functional>
#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <cstring>

// --- Algorithm --------------------------------------------------------------
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
        vector[to].emplace_back(Dest{distance * multiplier, from, isTram});
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

    std::vector<size_t> dists(total * 2, (size_t) -1);
    using QueueItem = std::tuple<size_t, size_t, bool>; // <time, station, used tram>
    std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<>> queue;
    queue.push({0, from, false});

    while(!queue.empty()) {
        auto [time, station, wasTram] = queue.top();
        queue.pop();
        // std::cout << "Popping [" << time << "," << station << "," << wasTram << "]" << std::endl;

        if (dists[station + (wasTram ? total : 0)] < time) {
            // std::cout << "Dropping" << std::endl;
            continue;
        }

        if (station == to) {
            std::cout << "Za " << time << " min. tam budu." << std::endl;
            return;
        }

        for (auto& neighbour : graph[station]) {
            if (wasTram && neighbour.isTram) continue;

            const size_t newTime = time + neighbour.time;
            size_t& destTime = dists[neighbour.to + (neighbour.isTram ? total : 0)];
            if (destTime <= newTime) {
                continue;
            }
            // std::cout << "Enqueue [" << newTime << "," << neighbour.to << "," << neighbour.isTram << "]" << std::endl;
            destTime = newTime;
            queue.push({newTime, neighbour.to, neighbour.isTram});
        }
    }

    std::cout << "O beda, o beda, co ja budu ted delat?" << std::endl;
}

// --- Generating data --------------------------------------------------------
bool nextBool() {
  return rand() % 2 == 0;
}

int nextInt(int min, int max) {
  return min + (rand() % (max - min));
}

void generateGraphs() {
    const int testCount = 10;
    std::cout << testCount << "\n\n";

    for (int i = 0; i < testCount; ++i) {
        std::cerr << "Creating test " << (i + 1) << std::endl;

        const int stations = nextInt(10, 100'000);
        const int parks = nextInt(stations * .2, stations * 2);
        const int trams = nextInt(stations * .2, stations * 2);

        {
            const int from = nextInt(0, stations);
            const int to   = nextInt(0, stations);
            std::cout << stations << " " << from << " " << to << "\n";
        }


        std::cout << parks << std::endl;
        for (int j = 0; j < parks; ++j) {
            const int from = nextInt(0, stations);
            const int to   = nextInt(0, stations);
            const int duration = nextInt(1, 20);
            std::cout << from << " " << to << " " << duration << "\n";
        }
        std::cout << trams << std::endl;
        for (int j = 0; j < trams; ++j) {
            const int from = nextInt(0, stations);
            const int to   = nextInt(0, stations);
            const int duration = nextInt(1, 200);
            std::cout << from << " " << to << " " << duration << "\n";
        }
    }
}

int main(int argc, char *argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    if (argc == 2 && strcmp(argv[1], "--generate") == 0) {
        generateGraphs();
        return 0;
    } else if (argc != 1) {
        std::cerr << "Invalid input" << std::endl;
        return 1;
    }

    int cases;
    std::cin >> cases;

    for (int i = 0; i < cases; ++i) {
        solve();
    }

    return 0;
}
