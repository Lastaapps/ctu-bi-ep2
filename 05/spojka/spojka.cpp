#include <iostream>
#include <vector>

using namespace std;

void solve() {
    size_t vertexCnt, edgeCnt;
    cin >> vertexCnt >> edgeCnt;

    vector<vector<size_t>> matrix(vertexCnt, vector<size_t>(vertexCnt, ~0u));

    for (size_t i = 0; i < edgeCnt; ++i) {
        size_t u, v, d;
        cin >> u >> v >> d;
        --u; --v;
        matrix[u][v] = d;
        matrix[v][u] = d;
    }

    for (size_t k = 0; k < vertexCnt; ++k) {
        for (size_t i = 0; i < vertexCnt; ++i) {
            for (size_t j = 0; j < vertexCnt; ++j) {
                const size_t sum = matrix[i][k] + matrix[k][j];
                // cout << sum << " ";
                if (matrix[i][j] > sum) {
                    matrix[i][j] = sum;
                }
            }
            // cout << "\n";
        }
    }
    
    size_t mmax = 0;
    for (size_t i = 0; i < vertexCnt; ++i) {
        for (size_t j = 0; j < vertexCnt; ++j) {
            if (i == j) { continue; }
            size_t sum = matrix[i][j];
            // cout << sum << " ";
            if (sum == ~0u) {
                cout << "Bez spojeni neni veleni!\n";
                return;
            }
            mmax = max(mmax, sum);
        }
        // cout << "\n";
    }
    cout << "Nejvetsi vzdalenost je " << mmax << ".\n";
    return;
}

int main(void) {
    size_t z;
    cin >> z;
    for (size_t i = 0; i < z; ++i) {
        solve();
    }
    return 0;
}

