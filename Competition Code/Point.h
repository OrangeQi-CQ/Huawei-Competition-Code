/**
 * 文件说明：
 * 这个文件实现平面上点。
*/
#pragma once
#include <math.h>


typedef double angle;
const int PI = acos(-1);


// 结构体：平面上的点
struct Point {
    double x;
    double y;

    bool operator== (Point t) {
        return x == t.x and y == t.y;
    }
};


double cal_distance(Point, Point);


/**
 * 功能：计算向量的角度
 * 返回范围是 [0, 2 * PI]
*/
angle cal_dir(Point);

/**
 * 功能：计算 a->b 的角度
 * 返回范围是 [0, 2 * PI]
*/
angle cal_dir(Point a, Point b);



typedef Point Direction;
typedef Point Speed;


