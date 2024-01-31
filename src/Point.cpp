#include "Point.h"


double cal_distance(Point a, Point b) {
    return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}


angle cal_dir(Direction a) {
    if (fabs(a.x) < 1e-5) {
        return a.y > 0 ? PI / 2 : PI * 3 / 2;
    }

    if (fabs(a.y) < 1e-5) {
        return a.x > 0 ? 0 : PI;
    }

    double res = atan(a.y / a.x);

    if (a.x < 0) {
        return res + PI;
    }

    if (a.x > 0 and a.y < 0) {
        return res + 2 * PI;
    }

    return res;
}


angle cal_dir(Point a, Point b) {
    return cal_dir({b.x - a.x, b.y - a.y});
}