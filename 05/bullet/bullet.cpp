#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <utility>
#include <cstdint>
#include <ios>
#include <iomanip>
#include <float.h>

using namespace std;

#define mp(x) ((x) * (x))

struct Vec2 {
    double x, y;
    Vec2() : x(0), y(0) {}
    Vec2(double x, double y) : x(x), y(y) {}

    double operator*(const Vec2& other) const {
        return x * other.x + y * other.y;
    }

    Vec2 operator*(const double other) const {
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

struct Circle {
    Vec2 v;
    double r;
};

struct Line {
    Vec2 v1, v2;
};

bool dblEq(const double d1, const double d2) {
    return abs(d1 - d2) < DBL_EPSILON * (d1 + d2);
}

double vectAngle(const Vec2& v1, const Vec2& v2) {
    return acos((v1 * v2) / (v1.norm() * v2.norm()));
}

bool isOnLine(const Line& l, const Vec2& v) {
    return dblEq((v - l.v1).norm() + (v - l.v2).norm(), (l.v2 - l.v1).norm());
}

/**
 * Has an unknown error inside, idk where
 * @return <direct, around>
 */
pair<double, double> intersectsAnalytic(const Circle& c, const Line& l) {
    const Vec2 v = l.v2 - l.v1;
    const double A = mp(v.x) + mp(v.y);
    const double B = 2 * (l.v1 * v - c.v * v);
    const double C = mp(l.v1.x - c.v.x) + mp(l.v1.y - c.v.y) - mp(c.r);

    const int32_t D = B * B - 4 * A * C;
    if (D < 0 || D == 0) { return {0, 0}; } 

    const double t1 = ((double) -B + sqrt(D)) / (2 * A);
    const double t2 = ((double) -B - sqrt(D)) / (2 * A);
    if (t1 < 0 || t1 > 1 || t2 < 0 || t1 > 1) { return {0, 0}; }

    const Vec2 v1(l.v1.x + v.x * t1, l.v1.y + v.y * t1);
    const Vec2 v2(l.v1.x + v.x * t2, l.v1.y + v.y * t2);

    const double dist = (v1 - v2).norm();

    const double angle = vectAngle(v1 - c.v, v2 - c.v);
    const double around = c.r * angle;

    return {dist, around};
}

pair<double, double> intersectsVectors(const Circle& c, const Line& l) {
    const Vec2 x = c.v - l.v1;
    const Vec2 y = l.v2 - l.v1;
    
    const Vec2 proj = y * ((x * y) / (y * y));
    const Vec2 diff = x - proj;
    const double dist = diff.norm();

    if (dist >= c.r) { return {0, 0}; }

    const double angle = 2 * acos(dist / c.r);
    const double secant = 2 * sqrt(mp(c.r) - mp(dist));
    const double around = c.r * angle;

    if (!isOnLine(l, proj + l.v1)) { return {0, 0}; }

    return {secant, around};
}

bool solve() {
    int32_t circleCnt;
    cin >> circleCnt;
    if (circleCnt == -1) { return false; }

    std::vector<Circle> circles;
    circles.reserve(circleCnt);

    for (int32_t i = 0; i < circleCnt; ++i) {
        Circle c;
        cin >> c.v.x >> c.v.y >> c.r;
        circles.emplace_back(c);
    }

    Line l;
    cin >> l.v1.x >> l.v1.y >> l.v2.x >> l.v2.y;

    {
        double direct = 0.0, around = (l.v1 - l.v2).norm();
        for (const auto& circ : circles) {
            const auto [d, a] = intersectsAnalytic(circ, l);
            direct += d;
            around += a - d;
        }
        cout << "Superman sees thru " << std::fixed << std::setprecision(2) <<  direct << " units, and flies " << around << " units.\n";
    }

    {
        double direct = 0.0, around = (l.v1 - l.v2).norm();
        for (const auto& circ : circles) {
            const auto [d, a] = intersectsVectors(circ, l);
            direct += d;
            around += a - d;
        }
        cout << "Superman sees thru " << std::fixed << std::setprecision(2) <<  direct << " units, and flies " << around << " units.\n";
    }

    return true;
}

int main(void) {
    while(solve());
    return 0;
}

