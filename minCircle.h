// Hodaya Raz 211540174, Rivka Doniger 324584531
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
    Circle():center(Point(0,0)),radius(0){}
};
// --------------------------------------

Circle findMinCircle(Point** points,size_t size);

Circle calculate_MEC(vector<Point>& Points, vector<Point> Result_Points, int size);

Circle min_circle_trivial(vector<Point>& P);

bool is_valid_circle(const Circle& c, const vector<Point>& P);

Circle circle_from(const Point& A, const Point& B);

Circle circle_from(const Point& A, const Point& B, const Point& C);

Point get_circle_center(double bx, double by, double cx, double cy);

Circle circle_from(const Point& A, const Point& B, const Point& C);

Point get_circle_center(double bx, double by, double cx, double cy);

bool is_in_circle(const Circle& c, const Point& p);

double distance(const Point& a, const Point& b);
#endif /* MINCIRCLE_H_ */
