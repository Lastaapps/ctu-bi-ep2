#include <vector>
#include <iostream>
#include <queue>

using namespace std;

struct Flow {
    vector<vector<size_t>> capacities;
    vector<vector<size_t>> flows;
    Flow(size_t items) : capacities(items, vector<size_t>(items)), flows(items, vector<size_t>(items)) {}

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

struct Edge {
    size_t u1, u2;
};

#define FAIL cout << "No euler circuit exist\n\n"; return

void solve() {

    size_t n, m;
    cin >> n >> m;
    n += 2;

    size_t undirectedCnt = 0;
    vector<Edge> undirected;
    vector<vector<size_t>> undirectedMatrix(n, vector<size_t>(n));
    Flow flow(n);
    vector<vector<size_t>> graph(n);
    vector<vector<size_t>> originalGraph(n);

    // cout << "Creating graph" << endl;
    // create graph
    for (size_t i = 0; i < m; ++i) {
        size_t u, v;
        char t;
        cin >> u >> v >> t;
        if (t == 'D') {
            {
                size_t cap = flow.getCapacity(0, v);
                if (cap == 0) {
                    graph[0].emplace_back(v);
                }
                flow.setCapacity(0, v, cap + 1);
            }
            {
                size_t cap = flow.getCapacity(u, n - 1);
                if (cap == 0) {
                    graph[u].emplace_back(n - 1);
                }
                flow.setCapacity(u, n - 1, cap + 1);
            }
            originalGraph[u].emplace_back(v);
        } else if (t == 'U') {
            size_t cap = flow.getCapacity(u, v);
            if (cap == 0) {
                undirected.emplace_back(Edge{u, v});
                graph[u].emplace_back(v);
                graph[v].emplace_back(u);
            }
            ++undirectedCnt;
            ++undirectedMatrix[u][v];
            ++undirectedMatrix[v][u];
            flow.setCapacity(u, v, cap + 1);
            flow.setCapacity(v, u, cap + 1);
        } else {
        }
    }

    // cout << "Solving flows" << endl;
    // solve flows
    while(true) {
        queue<size_t> queue;
        vector<size_t> parents(n);
        parents[0] = 1;
        queue.push(0);

        while(!queue.empty()) {
            size_t u = queue.front();
            queue.pop();

            for (auto v : graph[u]) {
                if (parents[v] != 0) { continue; }

                const size_t reserve = flow.getReserve(u, v);
                if (reserve == 0) { continue; }

                parents[v] = u + 1;
                queue.emplace(v);

                if (v == n - 1) {
                    size_t to = v;
                    size_t minFlow = ~0u;
                    while (to != 0) {
                        size_t from = parents[to] - 1;
                        minFlow = min(minFlow, flow.getReserve(from, to));
                        to = from;
                    }

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

    // cout << "Checking" << endl;
    // check validity - all mins accomplished
    size_t flowSum = 0;
    for (const auto& v : graph[0]) {
        flowSum += flow.getFlow(0, v);
    }

    // TODO validate
    if (flowSum + undirectedCnt != m) { FAIL; }

    // check validity - all undirected got directed
    for (auto [u, v] : undirected) {
        const size_t edgeCnt = undirectedMatrix[u][v];
        size_t f1 = flow.getFlow(u, v);
        size_t f2 = flow.getFlow(v, u);
        if (f1 < f2) {
            std::swap(f1, f2);
            std::swap(u, v);
        }

        if ((edgeCnt - f1) % 2 != 0) { FAIL; }

        size_t split = (edgeCnt - f1) / 2;
        for (size_t i = 0; i < f1 + split; ++i) {
            // cout << "Oriented: " << u << " -> " << v << endl;
            originalGraph[u].emplace_back(v);
        }

        for (size_t i = f1 + split; i < edgeCnt; ++i) {
            // cout << "Oriented: " << v << " -> " << u << endl;
            originalGraph[v].emplace_back(u);
        }
    }

    // TODO maybe check degrees

    // cout << "Finding move" << endl;
    vector<vector<size_t>> parents(n);
    for(size_t usedCnt = 0; usedCnt != m; ) {
        size_t startFrom = 1;
        while (true) {
            // cout << "Trying start from " << startFrom << endl;
            if (!originalGraph[startFrom].empty()) {
                break;
            }
            startFrom = parents[startFrom].back();
        }

        // cout << "Starting from " << startFrom << endl;
        size_t u = startFrom;
        do {
            // cout << "Running for " << u << endl;
            auto& orgRef = originalGraph[u];
            const size_t v = orgRef.back();
            orgRef.pop_back();
            ++usedCnt;
            parents[v].emplace_back(u);
            u = v;
        } while(u != startFrom);
    }

    // cout << "Outputting" << endl;
    bool isFirst = true;
    for(size_t u = 1; ;) {
        cout << (isFirst ? "" : " ") << u;
        isFirst = false;

        auto& par = parents[u];
        if (par.empty()) { break; }
        u = par.back();
        par.pop_back();
    }
    cout << "\n\n";
}

int main(void) {
    size_t cases;
    cin >> cases;
    for (size_t i = 0; i < cases; ++i) {
        solve();
    }
    return 0;
}

