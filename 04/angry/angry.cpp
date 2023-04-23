#include <vector>
#include <queue>
#include <iostream>

using namespace std;

struct Flow {
    vector<vector<size_t>> capacities;
    vector<vector<size_t>> flows;
    Flow(size_t items) : capacities(items, vector<size_t>(items)), flows(items, vector<size_t>(items)) {}

    void setCapacity(size_t u, size_t v, size_t cap) {
        capacities[u][v] = cap;
    }
    size_t getReserve(size_t u, size_t v) const {
        return flows[v][u] + capacities[u][v] - flows[u][v];
    }
    size_t getFlow(size_t u, size_t v) const {
        return flows[u][v];
    }
    void sendFlow(size_t u, size_t v, size_t flow) {
        const size_t oposite = flows[v][u];
        if (oposite >= flow) {
            flows[v][u] -= flow;
        } else {
            flows[v][u] = 0;
            flows[u][v] += flow - oposite;
        }
    }
};

bool solve() {
    size_t computers, wires;
    cin >> computers >> wires;
    if (computers == 0) { return false; }

    vector<vector<size_t>> graph(computers * 2);
    Flow flow(computers * 2);

    for (size_t i = 1; i < computers - 1; ++i) {
        size_t id, cost;
        cin >> id >> cost;
        --id;

        graph[id + computers].push_back(id);
        flow.setCapacity(id + computers, id, cost);
    }

    for (size_t i = 0; i < wires; ++i) {
        size_t from, to, cost;
        cin >> from >> to >> cost;
        --from; --to;

        graph[from].push_back(to + computers);
        graph[to].push_back(from + computers);
        flow.setCapacity(from, to + computers, cost);
        flow.setCapacity(to, from + computers, cost);
    }

    while(true) {
        queue<size_t> queue;
        vector<size_t> parents(computers * 2);
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

                if (v == computers * 2 - 1) {
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

    size_t flowSum = 0;
    for (const auto& v : graph[0]) {
        flowSum += flow.getFlow(0, v);
    }

    cout << flowSum << "\n";

    return true;
}


int main(void) {
    while(solve());
    return 0;
}

