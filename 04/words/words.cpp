#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

#define SUCCESS cout << "Ordering is possible.\n"; return
#define FAIL cout << "The door cannot be opened.\n"; return

void solve() {
    size_t cases;
    cin >> cases;
    if (cases == 0) { SUCCESS; }

    const size_t letterCnt = 'Z' - 'A' + 1;
    vector<int> degreesIn (letterCnt);
    vector<int> degreesOut(letterCnt);

    vector<vector<bool>> canReach(letterCnt, vector<bool>(letterCnt));

    string str;
    for (size_t i = 0; i < cases; ++i) {
        cin >> str;
        size_t indIn  = str[0] - 'a';
        size_t indOut = str[str.size() - 1] - 'a';
        ++degreesIn [indIn ];
        ++degreesOut[indOut];
        canReach[indIn][indOut] = true;
        canReach[indOut][indIn] = true;
    }

    bool oddFoundIn  = false;
    bool oddFoundOut = false;
    for (size_t i = 0; i < letterCnt; ++i) {
        int diff = degreesIn[i] - degreesOut[i];
        if (diff == 0) {
        } else if (diff * diff == 1) {
            if (diff < 0) { 
                if (oddFoundIn) { FAIL; }
                oddFoundIn = true; 
            }
            if (diff > 0) { 
                if (oddFoundOut) { FAIL; }
                oddFoundOut = true; 
            }
        } else {
            FAIL;
        }
    }

    queue<uint8_t> queue;
    vector<bool> visited(letterCnt);

    for (size_t i = 0; i < letterCnt; ++i) {
        if (degreesOut[i] != 0) {
            queue.push(i);
            visited[i] = true;
            break;
        }
    }
    for (size_t i = 0; i < letterCnt; ++i) {
        if (degreesOut[i] == 0) {
            visited[i] = true;
        }
    }

    while(!queue.empty()) {
        uint8_t u = queue.front();
        queue.pop();

        for (uint8_t v = 0; v < letterCnt; ++v) {
            if (visited[v]) { continue; }
            if (canReach[u][v]) {
                visited[v] = true;
                queue.push(v);
            }
        }
    }

    for (size_t i = 0; i < letterCnt; ++i) {
        if (visited[i] != true) { FAIL; }
    }

    SUCCESS;
}

int main(void) {
    size_t cases;
    cin >> cases;
    for (size_t i = 0; i < cases; ++i) {
        solve();
    }
}

