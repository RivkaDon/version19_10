// Hodaya Raz 211540174, Rivka Doniger 324584531
#include "minCircle.h"
#include "anomaly_detection_util.h"
#include <algorithm>
#include <math.h>
#include <vector>
using namespace std;

// function returns the distance between point a and b
double distance(const Point& a, const Point& b)
{
    return sqrt(pow(a.x - b.x, 2)
                + pow(a.y - b.y, 2));
}

// function returns true is point p is in circle or on circle border, or false otherwise
bool is_in_circle(const Circle& c, const Point& p)
{
    double dist = distance(c.center, p);
    if (dist <= c.radius) {
        return true;
    }
    return false;
}

// function returns the center of a circle defined by 3 points
Point get_circle_center(double bx, double by,
                        double cx, double cy)
{
    double B = bx * bx + by * by;
    double C = cx * cx + cy * cy;
    double D = bx * cy - by * cx;
    return Point((cy * B - by * C) / (2 * D),
             (bx * C - cx * B) / (2 * D));
}

// function returns minimal circle that includes 3 points a, b, c
Circle circle_from(const Point& A, const Point& B,
                   const Point& C)
{
    Point j = get_circle_center(B.x - A.x, B.y - A.y,
                                C.x - A.x, C.y - A.y);

    j.x += A.x;
    j.y += A.y;
    return Circle(j, distance(j, A));
}

// function returns smallest circle from 2 points
Circle circle_from(const Point& A, const Point& B)
{
    // find the average of x and y values of points to get the center point
    Point C( (A.x + B.x) / 2.0, (A.y + B.y) / 2.0 );

    // ********** radius is equal to half the distance between point a and b
    Circle C1(C, distance(A, B) / 2.0);
    return C1;
}

// function returns if all the points in vector p are included in the circle, and false otherwise
bool is_valid_circle(const Circle& c,
                     const vector<Point>& P)
{
    for (const Point& p : P)
        if (is_in_circle(c, p) == false)
            return false;
    return true;
}

// function returns minimal inclosing circle created from 3 or less points
Circle min_circle_trivial(vector<Point>& P)
{
    // assert(P.size() <= 3);
    if (P.empty()) {
        Circle C0({ 0, 0 }, 0 );
        return C0;
    }
    else if (P.size() == 1) {
        Circle C1(P[0], 0);
        return C1;
    }
    else if (P.size() == 2) {
        Circle C2 = circle_from(P[0], P[1]);
        return C2;
    }
    // checking to see if circle can be created by using only two points out of the three
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            Circle c = circle_from(P[i], P[j]);
            if (is_valid_circle(c, P))
                return c;
        }
    }
    Circle C3 = circle_from(P[0], P[1], P[2]);
    return C3;
}

// function calculates the MEC for the points in vector p recursively
Circle calculate_MEC(vector<Point>& Points,
                     vector<Point> Result_Points, int size)
{
    if (size == 0 || Result_Points.size() == 3) {
        return min_circle_trivial(Result_Points);
    }

    // Pick a random point randomly
    int idx = rand() % size;
    Point point = Points[idx];

    // switch the point at the end of the vector and put instead of the point we removed instead of having to erase
    // from the middle
    swap(Points[idx], Points[size - 1]);

    // calculates the circle recursively
    Circle circle = calculate_MEC(Points, Result_Points, size - 1);

    // If the circle contains point, return circle
    if (is_in_circle(circle, point)) {
        return circle;
    }

    // Otherwise point must be on the circumfrence of the MEC
    Result_Points.push_back(point);

    // Return the MEC while point is no longer in Points, but is in Result_Points and size is decreased by one
    return calculate_MEC(Points, Result_Points, size - 1);
}

// returns the MEC using the calculate_MEC function
Circle findMinCircle(Point** points,size_t size) {
    std::vector<Point> result;
    // move points to vector from array
    for( int i = 0; i < size; i++) {
        result.push_back(*points[i]);
    }
    return calculate_MEC(result, {}, size);
}