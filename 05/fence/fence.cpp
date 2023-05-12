#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <stdint.h>
#include <cassert>

using namespace std;

struct Vec2 {
    int32_t x, y;

    Vec2() : x(0), y(0) {}
    Vec2(int32_t x, int32_t y) : x(x), y(y) {}

    bool operator==(const Vec2& t) const noexcept {
        return x == t.x && y == t.y;
    }
    bool operator!=(const Vec2& t) const noexcept {
        return !(*this == t);
    }
    bool operator<(const Vec2& t) const noexcept {
        if (x != t.x) { return x < t.x; }
        if (y != t.y) { return y < t.y; }
        return false;
    }
    double operator*(const Vec2& other) const {
        return x * other.x + y * other.y;
    }
    Vec2 operator*(const int32_t other) const {
        return Vec2{other * x, other * y};
    }
    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }
    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }
    double norm() const {
        return hypot(x, y);
    }
};

std::ostream& operator<<(ostream& out, const Vec2& v) {
    return out << "[" << v.x << "," << v.y << "]";
}

double vectAngle(const Vec2& v1, const Vec2& v2) {
    return acos((v1 * v2) / (v1.norm() * v2.norm()));
}

enum class Dir { LEFT, RIGHT, SAME };
Dir vectDir(const Vec2& v1, const Vec2& v2) {
    const int32_t prod = v1.x * v2.y - v2.x * v1.y;
    if (prod == 0) {
        // cerr << "Same" << endl;
        return Dir::SAME;
    } else if (prod > 0) {
        // cerr << "Left" << endl;
        return Dir::LEFT;
    } else {
        // cerr << "Right" << endl;
        return Dir::RIGHT;
    }
}

struct Tree {
    Vec2 p;
    uint32_t v;
    uint32_t l;

    bool operator<(const Tree& t) const noexcept {
        if (p != t.p) { return p < t.p; }
        if (v != t.v) { return v < t.v; }
        if (l != t.l) { return l < t.l; }
        return false;
    }
};

using Trees = vector<Tree>;

/** <value, length> */
tuple<uint32_t, uint32_t> countLost(const Trees& trees, const uint32_t mask) {
    uint32_t value = 0;
    uint32_t length = 0;
    for (size_t i = 0; i < trees.size(); ++i) {
        if (mask & (1 << i)) {
            const Tree& tree = trees[i];
            length += tree.l;
            value += tree.v;
        }
    }
    return {value, length};
}

template<Dir illegalDir>
double recoverHull(vector<reference_wrapper<const Vec2>>& used, const Vec2& tree) {
    double sum = 0;
    while (used.size() > 1) {
        const Vec2& last = *  used.rbegin();
        const Vec2& prev = *++used.rbegin();
        const Vec2  x = last - prev;
        const Vec2  y = tree - last;
        if (vectDir(x, y) != illegalDir) { break; }
        // cerr << "Popping  " << last << endl;
        sum -= x.norm();
        used.pop_back();
    }
    // cerr << "Accepted " << tree << endl;
    const Vec2& last = *  used.rbegin();
    const Vec2 y = tree - last;
    sum += y.norm();
    used.push_back(tree);
    return sum;
}

double convexHullLength(const Trees& trees, const uint32_t mask) {
    double sum = 0;
    // yes, should be sequence in a normal language...
    for (size_t i = 0; i < trees.size(); ++i) {
        if (mask & (1u << i)) { continue; }

        const Vec2& start = trees[i].p;

        vector<reference_wrapper<const Vec2>> top {start}, bottom{start};

        ++i;
        // cerr << "Starting with " << top[0] << ", " << top[1] << endl;
        for (; i < trees.size(); ++i) {
            if (mask & (1u << i)) { continue; }

            const Vec2& tree = trees[i].p;

            // cerr << "Adding top " << tree << endl;
            sum += recoverHull<Dir::RIGHT>(top,    tree);
            // cerr << "Adding bottom " << tree << endl;
            sum += recoverHull<Dir::LEFT >(bottom, tree);
        }
        break;
    }
    return sum;
}

bool solve() {
    size_t treeCnt;
    cin >> treeCnt;
    if (treeCnt == 0) { return false; }

    Trees trees;
    for (size_t i = 0; i < treeCnt; ++i) {
        Tree t;
        cin >> t.p.x >> t.p.y >> t.v >> t.l;
        trees.emplace_back(t);
    }
    std::sort(trees.begin(), trees.end());

    // for (const auto& tree : trees) {
    //     cerr << tree.p << "\n";
    // }

    // double res = convexHullLength(trees, 0b10000);
    // cout << "Hull res " << res << "\n";
    // return false;

    uint32_t maxLost = ~0u;
    for (uint32_t mask = 0; mask < (1u << treeCnt); ++mask) {
        const auto [value, length] = countLost(trees, mask);

        if (length >= convexHullLength(trees, mask)) {
            maxLost = std::min(maxLost, value);
        }
    }
    cout << "The lost value is " << maxLost << ".\n";

    return true;
}

int main(void) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    assert(vectDir(Vec2(1, 1), Vec2(2, 2) - Vec2(1, 1)) == Dir::SAME);
    assert(vectDir(Vec2(1, 1), Vec2(2, 1) - Vec2(1, 1)) == Dir::RIGHT);
    assert(vectDir(Vec2(1, 1), Vec2(1, 2) - Vec2(1, 1)) == Dir::LEFT);
    assert(vectDir(Vec2(1, 1), Vec2(1, 0) - Vec2(1, 1)) == Dir::RIGHT);
    assert(vectDir(Vec2(1, 1), Vec2(0, 1) - Vec2(1, 1)) == Dir::LEFT);

    while(solve()) {}
    return 0;
}
