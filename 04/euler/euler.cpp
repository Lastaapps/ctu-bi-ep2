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

    vector<Edge> undirected;
    Flow flow(n);
    vector<vector<size_t>> graph(n);
    vector<vector<size_t>> originalGraph(n);

    cout << "Creating graph" << endl;
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
            undirected.emplace_back(Edge{u, v});
            graph[u].emplace_back(v);
            graph[v].emplace_back(u);
            flow.setCapacity(u, v, 1);
            flow.setCapacity(v, u, 1);
        } else {
        }
    }

    cout << "Solving flows" << endl;
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

    cout << "Checking" << endl;
    // check validity - all mins accomplished
    size_t flowSum = 0;
    for (const auto& v : graph[0]) {
        flowSum += flow.getFlow(0, v);
    }

    if (flowSum + undirected.size() != m) { FAIL; }

    // check validity - all undirected got directed
    for (const auto& edge : undirected) {
        size_t f1 = flow.getFlow(edge.u1, edge.u2);
        size_t f2 = flow.getFlow(edge.u2, edge.u1);
        if (f1 == 1 && f2 == 0) {
            cout << "Oriented: " << edge.u1 << " -> " << edge.u2 << endl;
            originalGraph[edge.u1].emplace_back(edge.u2);
        } else 
        if (f1 == 0 && f2 == 1) {
            cout << "Oriented: " << edge.u2 << " -> " << edge.u1 << endl;
            originalGraph[edge.u2].emplace_back(edge.u1);
        } else { FAIL; }
    }

    // TODO maybe check degrees

    cout << "Finding move" << endl;
    vector<vector<size_t>> parents(n);
    {
        // find sequence
        vector<vector<bool>> used(m, vector<bool>(m));
        // TODO decrease degrees
        vector<bool> saturated(n);

        size_t usedCnt = 0;
        while(usedCnt != m) {
            size_t startFrom = 1;
            while (true) {
                cout << "Trying start from " << startFrom << endl;
                if (!saturated[startFrom]) {
                    for (const auto& to : originalGraph[startFrom]) {
                        if (used[startFrom][to]) { continue; }
                        goto startExecution;
                    }
                    saturated[startFrom] = true;
                }
                for (const auto& u : parents[startFrom]) {
                    if (saturated[u]) { continue; }
                    startFrom = u;
                    break;
                }
            }
startExecution:
            cout << "Starting from " << startFrom << endl;
            size_t u = startFrom;
            do {
                cout << "Running for " << u << endl;
                const size_t oldU = u;
                for (const auto& v : originalGraph[u]) {
                    if (used[u][v]) { continue; }
                    ++usedCnt;
                    used[u][v] = true;
                    parents[v].emplace_back(u);
                    u = v;
                    break;
                }
                size_t usedEdges = 0;
                for (const auto& v : originalGraph[oldU]) {
                    if (used[oldU][v]) { ++usedEdges; }
                }
                if (usedEdges == originalGraph[oldU].size()) {
                    saturated[oldU] = true;
                }
            } while(u != startFrom);
        }
    }

    cout << "Outputting" << endl;
    bool isFirst = true;
    for(size_t u = 1; ;) {
        cout << (isFirst ? "" : " ") << u;
        isFirst = false;

        auto& par = parents[u];
        if (par.empty()) { break; }
        u = par.back();
        par.pop_back();
    }
    cout << "\n";
}

int main(void) {
    size_t cases;
    cin >> cases;
    for (size_t i = 0; i < cases; ++i) {
        solve();
    }
    return 0;
}

