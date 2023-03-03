#include <stdlib.h>
#include <iostream>
#include <array>

const size_t BASE = 101;
const size_t MAP_SIZE = (BASE + 2) * (BASE + 2) * (BASE + 2);

struct Coord {
    int x, y, z;
};
typedef std::array<bool, MAP_SIZE> Caves;

inline bool& atPos(Caves& arr, int x, int y, int z) {
    return arr[x * (BASE + 2) * (BASE + 2) + y * (BASE + 2) + z];
}

void loadInput(Coord& dimen, Caves& caves) {
    scanf("%d %d %d", &dimen.x, &dimen.y, &dimen.z);

    for (int z = 1; z < dimen.z + 1; ++z) {
        int p;
        scanf("%d", &p);

        for (int pp = 0; pp < p; ++pp) {
            int r, s;
            scanf("%d %d", &r, &s);
            atPos(caves, r, s, z) = true;
        }
    }
}

int runBfs(Caves& caves, int x, int y, int z) {
    bool& value = atPos(caves, x, y, z);
    if (!value) {
        return 0;
    }
    value = false;
    return 1
        + runBfs(caves, x + 1, y + 0, z + 0)
        + runBfs(caves, x - 1, y + 0, z + 0)
        + runBfs(caves, x + 0, y + 1, z + 0)
        + runBfs(caves, x + 0, y - 1, z + 0)
        + runBfs(caves, x + 0, y + 0, z + 1)
        + runBfs(caves, x + 0, y + 0, z - 1);
}

int process(Coord& dimen, Caves& caves) {
    int total = 0;
    for (int x = 1; x < dimen.x + 1; ++x) {
        for (int y = 1; y < dimen.y + 1; ++y) {
            total += runBfs(caves, x, y, 1);
        }
    }
    return total * 1000;
}

void printMatrix(Coord& dimen, Caves& caves) {
    int wb = false ? 1 : 0; // with border
    for (int z = 1 - wb; z < dimen.z + 1 + wb; ++z) {
        for (int x = 1 - wb; x < dimen.x + 1 + wb; ++x) {
            for (int y = 1 - wb; y < dimen.y + 1 + wb; ++y) {
                std::cout << atPos(caves, x, y, z) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void solve() {
    Coord dimen;
    Caves caves;

    loadInput(dimen, caves);
    // printMatrix(dimen, caves);

    int liters = process(dimen, caves);
    printf("Je nutne vycerpat %d litru vody.\n", liters);
}

int main() {
    int assignments;
    scanf("%d", &assignments);

    for (int i = 0; i < assignments; ++i) {
        solve();
    }
    
    return 0;
}

