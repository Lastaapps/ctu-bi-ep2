#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <ios>

using Graph = std::vector<std::vector<int>>;

struct SearchDfs {
    private:
    const Graph& graph;
    std::vector<int> ins;
    int counter = 1;
    int artFound = 0;

    public:
    SearchDfs(const Graph& graph) : graph(graph), ins(graph.size()) {}

    private:
    std::pair<int, bool> searchArticulations(const int parent, const int node) {
        int id = counter++;
        ins[node] = id;

        printf("[%d] Got id %d\n", node, id);
        int low = id;
        bool isArt = false;

        for (const auto target : graph[node]) {
            if (target == parent) continue;
            if (ins[target] == 0) {
                const auto [childLow, wasArc] = searchArticulations(node, target);

                if (wasArc) { artFound++; }
                low = std::min(low, childLow);

                if (childLow >= id) {
                    isArt = true;
                }
            } else {
                low = std::min(low, ins[target]);
            }
        }

        printf("[%d] Got low %d\n", node, low);
        if (isArt) {
            printf("[%d] Is art %d\n", node, id);
        }

        return {low, isArt};
    }

    public:
    int run() {
        for (size_t i = 0; i < graph.size(); ++i) {
            if (ins[i] != 0) continue;
            searchArticulations((int) -1, i);
            if (graph[i].size() > 1) {
                artFound++;
            }
        }
        return artFound;
    }
};

bool solve() {
    int x;
    std::cin >> x;
    if (x == 0) return false;

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
    printf("%d\n", dfs.run());



    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    while(solve());
    return 0;
}

