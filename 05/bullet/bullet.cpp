#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <utility>
#include <cstdint>
#include <ios>
#include <iomanip>

using namespace std;

struct Circle {
    int32_t x, y, r;
};

struct Line {
    int32_t x, y, sx, sy;
};

double vectAngle(const double x1, const double y1, const double x2, const double y2) {
    return acos((x1 * x2 + y1 * y2) / (hypot(x1, y1) * hypot(x2, y2)));
}

#define mp(x) (x) * (x)

/**
 * @return <direct, around>
 */
pair<double, double> intersects(const Circle& c, const Line& l) {
    const int32_t A = mp(l.sx) + mp(l.sy);
    const int32_t B = 2 * (l.x * l.sx + l.y * l.sy - c.x*l.sx - c.y*l.sy);
    const int32_t C = mp(l.x - c.x) + mp(l.y - c.y) - (c.r * c.r);

    const int32_t D = B * B - 4 * A * C;
    if (D < 0 || D == 0) { return {0, 0}; } 

    const double t1 = ((double) -B + sqrt(D)) / (2 * A);
    const double t2 = ((double) -B - sqrt(D)) / (2 * A);
    if (t1 < 0 || t1 > 1) { return {0, 0}; }

    const double x1 = l.x + l.sx * t1;
    const double y1 = l.y + l.sy * t1;
    const double x2 = l.x + l.sx * t2;
    const double y2 = l.y + l.sy * t2;

    const double dist = hypot(x1 - x2, y1 - y2);

    const double angle = vectAngle(x1 - c.x, y1 - c.y, x2 - c.x, y2 - c.y);
    const double around = c.r * angle;
    
    return {dist, around};
}

bool solve() {
    int32_t circleCnt;
    cin >> circleCnt;
    if (circleCnt == -1) { return false; }

    std::vector<Circle> circles;
    circles.reserve(circleCnt);

    for (int32_t i = 0; i < circleCnt; ++i) {
        Circle c;
        cin >> c.x >> c.y >> c.r;
        circles.emplace_back(c);
    }

    int32_t x1, x2, y1, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    Line l{x1, y1, (int32_t) (x2 - x1), (int32_t) (y2 - y1)};

    double direct = 0.0, around = hypot(l.sx, l.sy);
    for (const auto& circ : circles) {
        auto [d, a] = intersects(circ, l);
        direct += d;
        around += a;
        around -= d; 
    }

    cout << "Superman sees thru " << std::fixed << std::setprecision(2) <<  direct << " units, and flies " << around << " units.\n";

    return true;
}

int main(void) {
    while(solve());
    return 0;
}

