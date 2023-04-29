#include <ctime>
#include <iostream>
#include <cassert>
#include <iterator>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>

using namespace std;

struct Coord {
    int16_t x, y;
};
struct Edge {
    uint16_t v;
    double time;
    Edge(uint16_t v, double time) : v(v), time(time) {}
};

struct Flow {
    vector<vector<size_t>> capacities;
    vector<vector<size_t>> flows;
    Flow(size_t size) : capacities(size, vector<size_t>(size)), flows(size, vector<size_t>(size)) {}

    void setCapacity(size_t u, size_t v, size_t cap) noexcept {
        capacities[u][v] = cap;
    }
    size_t getCapacity(size_t u, size_t v) const noexcept {
        return capacities[u][v];
    }
    size_t getReserve(size_t u, size_t v) const noexcept {
        return flows[v][u] + capacities[u][v] - flows[u][v];
    }
    size_t getFlow(size_t u, size_t v) const noexcept {
        return flows[u][v];
    }
    void sendFlow(size_t u, size_t v, size_t flow) noexcept {
        const size_t opposite = flows[v][u];
        if (opposite >= flow) {
            flows[v][u] -= flow;
        } else {
            flows[v][u] = 0;
            flows[u][v] += flow - opposite;
        }
    }
};

size_t findForTime( vector<vector<Edge>> graph, const size_t holesCnt, double maxTime) {
    const size_t gopherCnt = graph.size() - 2 - holesCnt;
    Flow flow(graph.size());

    // yeah, this is slow, I don't care now
    for (size_t hole = 2; hole < holesCnt + 2; ++hole) {
        flow.setCapacity(0, hole, 1);
        for (size_t gopher = 2 + holesCnt; gopher < graph.size(); ++gopher) {
            flow.setCapacity(hole, gopher, 1);
        }
    }
    for (size_t gopher = 2 + holesCnt; gopher < graph.size(); ++gopher) {
        flow.setCapacity(gopher, 1, 1);
    }
    
    while(true) {
        queue<size_t> queue;
        vector<size_t> parents(graph.size());
        parents[0] = 1;
        queue.push(0);

        while(!queue.empty()) {
            size_t u = queue.front();
            queue.pop();

            for (const auto [v, time] : graph[u]) {
                if (time > maxTime) { continue; }
                if (parents[v] != 0) { continue; }

                const size_t reserve = flow.getReserve(u, v);
                if (reserve == 0) { continue; }

                parents[v] = u + 1;
                queue.emplace(v);

                if (v == 1) {
                    size_t to = v;
                    size_t minFlow = ~0u;
                    while (to != 0) {
                        size_t from = parents[to] - 1;
                        minFlow = min(minFlow, flow.getReserve(from, to));
                        to = from;
                    }
                    // size_t minFlow = 1u;

                    to = v;
                    while (to != 0) {
                        size_t from = parents[to] - 1;
                        flow.sendFlow(from, to, minFlow);
                        to = from;
                    }

                    goto end;
                }
            }

        }
        break;
end:;
    }

    size_t resFlow = 0;
    for (size_t u = 2; u < holesCnt + 2; ++u) {
        resFlow += flow.getFlow(0, u);
    }

    // for (size_t hole = 0; hole < holesCnt; ++hole) {
    //     for (size_t gopher = 0; gopher < graph.size() - holesCnt - 2; ++gopher) {
    //         size_t aHole = hole + 2;
    //         size_t aGopher = gopher + 2 + holesCnt;
    //         if (flow.getFlow(aHole, aGopher) || flow.getFlow(aGopher, aHole)) {
    //             cerr << gopher << " -> " << hole << endl;
    //         }
    //     }
    // }

    return resFlow;
}

#define SUCC(time) cout << std::fixed << std::setprecision(3) << ceil((time) * 1000) / 1000 << "\n\n"; return
#define FAIL cout << "Too bad.\n\n"; return

void solve(size_t caseId) {
    cout << "Case #" << caseId << ":" << endl;

    uint32_t gopherCnt, holesCnt, canLooseCnt;
    cin >> gopherCnt >> holesCnt >> canLooseCnt;

    std::set<double> uniqueTimesSet;

    // 0    - source
    // 1    - target
    // 2..x - holes
    // x..  - gophers
    const size_t total = 2 + gopherCnt + holesCnt;
    vector<vector<Edge>> graph(total);

    {
        std::vector<Coord> gophers;
        gophers.reserve(gopherCnt);

        for (size_t i = 0; i < gopherCnt; ++i) {
            int16_t x, y;
            cin >> x >> y;
            gophers.emplace_back(Coord{x, y});
            graph[2 + holesCnt + i].push_back(Edge(1, 0.0)); // can be faster, I don't care now
        }
        for (uint32_t hole = 0; hole < holesCnt; ++hole) {
            int16_t x, y;
            cin >> x >> y;
            for (uint32_t gopher = 0; gopher < gopherCnt; ++gopher) {
                const auto [gx, gy] = gophers[gopher];
                const double time = hypot(x - gx, y - gy);
                uniqueTimesSet.insert(time);
                graph[2 + hole].push_back(Edge(2 + holesCnt + gopher, time));
                graph[2 + holesCnt + gopher].push_back(Edge(2 + hole, time));
            }
            graph[0].push_back(Edge(hole + 2u, 0.0)); // can be faster, I don't care now
        }
    }

    vector<double> uniqueTimes {uniqueTimesSet.begin(), uniqueTimesSet.end()};

    int32_t toCover = gopherCnt - canLooseCnt;
    if (toCover > (int32_t) holesCnt) { FAIL; }

    // int asdf = 0;
    for (const auto& time : uniqueTimes) {
        size_t saved = findForTime(graph, holesCnt, time);
        // cerr << "Saved " << saved << " for time " << time << endl;
        if (saved >= toCover) {
            // cerr << "Answer is " << asdf << endl;
            // break;
            SUCC(time);
        }
        // asdf++;
    }
    // assert(false);

    // lo -> including
    // hi -> excluding
    // for(size_t lo = 0, hi = uniqueTimes.size(); lo < hi;) {
    //     size_t mid = (lo + hi) / 2;
    //     size_t time = uniqueTimes[mid];
    //     size_t saved = findForTime(graph, holesCnt, time);

    //     cerr << "[lo, hi, mid] = ["<< lo<< ","<< hi << "->" <<mid <<  "], Time: " << time <<", Saved: "<< saved << ", Req: " << toCover << endl;;

    //     if (saved >= toCover) {
    //         hi = mid;
    //     } else {
    //         lo = mid + 1;
    //     }
    //     if (lo >= hi) {
    //         SUCC(uniqueTimes[lo]);
    //     }

    //     // if (toCover < saved) {
    //     //     lo = mid + 1;
    //     // } else if (toCover > saved) {
    //     //     hi = mid;
    //     // } else {
    //     //     SUCC(uniqueTimes[mid]);
    //     // }
    // }

    FAIL;
    if (toCover > 0) { FAIL; }
}

int main(void) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    size_t cases;
    cin >> cases;
    for (size_t i = 0; i < cases; ++i) {
        solve(i + 1);
    }
}

