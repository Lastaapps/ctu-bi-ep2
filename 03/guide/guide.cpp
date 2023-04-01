#include <ostream>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <ios>
#include <set>
#include <algorithm>

using Graph = std::vector<std::vector<int>>;

struct SearchDfs {
    private:
    const Graph& graph;
    std::vector<int> ins;
    int counter = 1;
    std::set<int> artFound;

    public:
    SearchDfs(const Graph& graph) : graph(graph), ins(graph.size()) {}

    private:
    int searchArticulations(const int parent, const int node) {
        int id = counter++;
        ins[node] = id;

        // printf("[%d] Got id %d\n", node, id);
        int low = id;

        for (const auto target : graph[node]) {
            if (target == parent) continue;
            if (ins[target] == 0) {
                const int childLow = searchArticulations(node, target);

                low = std::min(low, childLow);

                if (childLow >= id) {
                    if (parent != -1 || graph[node].size() > 1) {
                        // printf("[%d] Marked as articulation\n", node);
                        artFound.insert(node);
                    }
                }
            } else {
                low = std::min(low, ins[target]);
            }
        }

        // printf("[%d] Low is %d\n", node, low);
        return low;
    }

    public:
    const std::set<int>& run() {
        for (size_t i = 0; i < graph.size(); ++i) {
            if (ins[i] != 0) continue;
            searchArticulations(-1, i);
        }
        // TODO remove
        fflush(stdout);
        return artFound;
    }
};

bool solve(int mapCnt) {
    int x;
    std::cin >> x;
    if (x == 0) return false;

    if (mapCnt != 1) {
        std::cout << std::endl;
    }

    std::unordered_map<std::string, int> translation;
    Graph graph(x);
    std::vector<std::string> places;

    for (int i = 0; i < x; ++i) {
        std::string name;
        std::cin >> name;
        translation[name] = i;
        places.push_back(name);
    }
    std::cin >> x;
    for (int i = 0; i < x; ++i) {
        std::string from, to;
        std::cin >> from >> to;
        int f = translation[from];
        int t = translation[to];
        graph[f].push_back(t);
        graph[t].push_back(f);
    }

    auto dfs = SearchDfs(graph);
    const auto& found = dfs.run();
    std::cout << "City map #" << mapCnt << ": " << found.size() << " camera(s) found" << std::endl;

    std::vector<std::string> names;
    names.reserve(found.size());

    for (const auto& art : found) {
        names.push_back(places[art]);
    }
    std::sort(names.begin(), names.end());
    for (const std::string& name : names) {
        std::cout << name << std::endl;
    }

    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int cnt = 1;

    while(solve(cnt++));

    return 0;
}

