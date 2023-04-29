#include <iostream>
#include <cassert>
#include <iterator>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

struct Coord {
    int16_t x, y;
};
struct Edge {
    uint32_t gopher, hole;
    double length;

    bool operator<(const Edge& other) const noexcept {
        if (length != other.length) { return length < other.length; }
        if (gopher != other.gopher) { return gopher < other.gopher; }
        if (length != other.hole  ) { return hole   < other.hole;   }
        return false;
    }
    bool operator>(const Edge& other) const noexcept {
        return other < *this;
    }
};

ostream& operator<<(ostream& out, const Edge& edge) noexcept {
    return out << "[" << edge.gopher << "->" << edge.hole << " = " << edge.length << "]";
}

#define SUCC(time) cout << std::fixed << std::setprecision(3) << ceil((time) * 1000) / 1000 << "\n\n"; return
#define FAIL cout << "Too bad.\n\n"; return

struct DfsG final {
    private:
    const vector<vector<Edge>>& gopherEdges;
    const vector<vector<Edge>>& holesEdges;
    std::vector<std::vector<bool>>& pairedMatrix;
    std::vector<bool>& gopherPaired;
    std::vector<bool>& holesPaired;
    vector<bool> visitedGophers;
    vector<bool> visitedHoles;

    public:
    DfsG(
    const vector<vector<Edge>>& gopherEdges,
    const vector<vector<Edge>>& holesEdges,
    std::vector<std::vector<bool>>& pairedMatrix,
    std::vector<bool>& gopherPaired,
    std::vector<bool>& holesPaired) noexcept :
        gopherEdges(gopherEdges),
        holesEdges(holesEdges),
        pairedMatrix(pairedMatrix),
        gopherPaired(gopherPaired),
        holesPaired(holesPaired),
        visitedGophers(gopherEdges.size()),
        visitedHoles(holesEdges.size())
    {}

    private:
    bool processGopher(const Edge& addedEdge, uint32_t id) noexcept {
        // if (addedEdge.gopher == id) {
        //     cerr << "Reached starting point" << endl;
        //     return true;
        // }

        if (visitedGophers[id]) { return false; }
        visitedGophers[id] = true;

        if (gopherPaired[id]) {
            for (const auto& edge : gopherEdges[id]) {
                if (edge > addedEdge) { break; }
                assert(!pairedMatrix[edge.gopher][edge.hole]);

                cerr << "G -> H: Trying for " << edge << endl;
                const bool res = processHole(addedEdge, edge.hole);
                if (res) {
                    cerr << "G -> H: Success for " << edge << endl;
                    pairedMatrix[edge.gopher][edge.hole] = true;
                    return true;
                }
            }
            return false;
        } else {
            assert(false);
        }
    }
    bool processHole(const Edge& addedEdge, uint32_t id) noexcept {
        if (visitedHoles[id]) { return false; }
        visitedHoles[id] = true;

        if (holesPaired[id]) {
            for (const auto& edge : holesEdges[id]) {
                if (edge > addedEdge) { break; }
                if (!pairedMatrix[edge.gopher][edge.hole]) { continue; }

                pairedMatrix[edge.gopher][edge.hole] = false;
                cerr << "H -> G: Trying for " << edge << endl;
                const bool res = processGopher(addedEdge, edge.gopher);
                if (res) {
                    cerr << "H -> G: Success for " << edge << endl;
                    return true;
                } else {
                    pairedMatrix[edge.gopher][edge.hole] = true;
                }
                break;
            }
            return false;
        } else {
            holesPaired[id] = true;
            return true;
        }
    }
    public:
    bool findPath(const Edge& addedEdge) noexcept {
        if (gopherPaired[addedEdge.gopher]) { return false; }
        visitedGophers[addedEdge.gopher] = true;

        cerr << "  -> H: Success for " << addedEdge << endl;
        const bool res = processHole(addedEdge, addedEdge.hole);
        if (res) {
            pairedMatrix[addedEdge.gopher][addedEdge.hole] = true;
            gopherPaired[addedEdge.gopher] = true;
            return true;
        }
        return false;
    }
};

struct DfsH final {
    private:
    const vector<vector<Edge>>& gopherEdges;
    const vector<vector<Edge>>& holesEdges;
    std::vector<std::vector<bool>>& pairedMatrix;
    std::vector<bool>& gopherPaired;
    std::vector<bool>& holesPaired;
    vector<bool> visitedGophers;
    vector<bool> visitedHoles;

    public:
    DfsH(
    const vector<vector<Edge>>& gopherEdges,
    const vector<vector<Edge>>& holesEdges,
    std::vector<std::vector<bool>>& pairedMatrix,
    std::vector<bool>& gopherPaired,
    std::vector<bool>& holesPaired) noexcept :
        gopherEdges(gopherEdges),
        holesEdges(holesEdges),
        pairedMatrix(pairedMatrix),
        gopherPaired(gopherPaired),
        holesPaired(holesPaired),
        visitedGophers(gopherEdges.size()),
        visitedHoles(holesEdges.size())
    {}

    private:
    bool processGopher(const Edge& addedEdge, uint32_t id, bool wasStartPaired) noexcept {
        if (visitedGophers[id]) { return false; }
        visitedGophers[id] = true;

        if (gopherPaired[id]) {
            for (const auto& edge : gopherEdges[id]) {
                if (edge > addedEdge) { break; }
                if (!pairedMatrix[edge.gopher][edge.hole]) { continue; }

                pairedMatrix[edge.gopher][edge.hole] = false;
                cerr << "G -> H: Trying for " << edge << endl;
                const bool res = processHole(addedEdge, edge.hole, wasStartPaired);
                if (res) {
                    cerr << "G -> H: Success for " << edge << endl;
                    return true;
                } else {
                    pairedMatrix[edge.gopher][edge.hole] = true;
                }
                break;
            }
            return false;
        } else {
            // if (wasStartPaired) { return false; }
            gopherPaired[id] = true;
            return true;
        }
    }
    bool processHole(const Edge& addedEdge, uint32_t id, bool wasStartPaired) noexcept {
        // if (addedEdge.hole == id) {
        //     if (wasStartPaired) {
        //         cerr << "Reached starting point" << endl;
        //         return true;
        //     } else {
        //         return false;
        //     }
        // }

        if (visitedHoles[id]) { return false; }
        visitedHoles[id] = true;

        if (holesPaired[id]) {
            for (const auto& edge : holesEdges[id]) {
                if (edge > addedEdge) { break; }
                assert(!pairedMatrix[edge.gopher][edge.hole]);

                cerr << "H -> G: Trying for " << edge << endl;
                const bool res = processGopher(addedEdge, edge.gopher, wasStartPaired);
                if (res) {
                    cerr << "H -> G: Success for " << edge << endl;
                    pairedMatrix[edge.gopher][edge.hole] = true;
                    return true;
                }
            }
            return false;
        } else {
            assert(false);
        }
    }
    public:
    bool findPath(const Edge& addedEdge) noexcept {
        const bool wasPaired = holesPaired[addedEdge.hole];
        // if (holesPaired[addedEdge.hole]) { return false; }
        if (holesPaired[addedEdge.hole]) { return false; }
        visitedHoles[addedEdge.hole] = true;

        cerr << "  -> G: Success for " << addedEdge << endl;
        const bool res = processGopher(addedEdge, addedEdge.gopher, wasPaired);
        if (res) {
            pairedMatrix[addedEdge.gopher][addedEdge.hole] = true;
            holesPaired[addedEdge.hole] = true;
            return true;
        }
        return false;
    }
};

void solve(size_t caseId) {
    cout << "Case #" << caseId << ":" << endl;

    uint32_t gopherCnt, holesCnt, canLooseCnt;
    cin >> gopherCnt >> holesCnt >> canLooseCnt;

    std::vector<Coord> gophers;
    gophers.reserve(gopherCnt);
    std::vector<Edge> edges;
    edges.reserve(gopherCnt * holesCnt);

    for (size_t i = 0; i < gopherCnt; ++i) {
        int16_t x, y;
        cin >> x >> y;
        gophers.emplace_back(Coord{x, y});
    }
    for (uint32_t i = 0; i < holesCnt; ++i) {
        int16_t x, y;
        cin >> x >> y;
        for (uint32_t j = 0; j < gopherCnt; ++j) {
            const auto& gopher = gophers[j];
            const auto edge = Edge{j, i, hypot(x - gopher.x, y - gopher.y)};
            edges.emplace_back(edge);
        }
    }

    std::sort(edges.begin(), edges.end());

    std::vector<std::vector<Edge>> gopherEdges(gopherCnt, std::vector<Edge>());
    std::vector<std::vector<Edge>> holesEdges(holesCnt, std::vector<Edge>());

    for (const auto& edge : edges) {
        gopherEdges[edge.gopher].emplace_back(edge);
        holesEdges[edge.hole].emplace_back(edge);
    }

    std::vector<bool> gopherPaired(gopherCnt);
    std::vector<bool> holesPaired (holesCnt);
    std::vector<std::vector<bool>> pairedMatrix(gopherCnt, std::vector<bool>(holesCnt));
    int32_t toCover = gopherCnt - canLooseCnt;

    if (toCover > (int32_t) holesCnt) { FAIL; }

    for (const auto& edge : edges) {
        cout << edge << endl;
    }

    auto edgeItr = edges.begin();
    for (; toCover > 0 && edgeItr != edges.end(); edgeItr++) {
        const auto& edge = *edgeItr;
        auto dfsG = DfsG(gopherEdges, holesEdges, pairedMatrix, gopherPaired, holesPaired);
        auto dfsH = DfsH(gopherEdges, holesEdges, pairedMatrix, gopherPaired, holesPaired);

        cerr << "Trying edge " << edge << endl;
        if (dfsG.findPath(edge)) {
            --toCover;
            cerr << "Success, added from G" << endl;
        } else if (dfsH.findPath(edge)) {
            --toCover;
            cerr << "Success, added from H" << endl;
        }

        auto gCnt = count(gopherPaired.begin(), gopherPaired.end(), true);
        auto hCnt = count(holesPaired.begin(), holesPaired.end(), true);
        cerr << "Paired " << gCnt << ", " << hCnt << endl;
    }

    if (toCover > 0) { FAIL; }
    SUCC((--edgeItr)->length);
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

