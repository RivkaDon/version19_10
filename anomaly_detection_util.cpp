// Hodaya Raz and Rivka Doniger

#include <iostream>
#include "anomaly_detection_util.h"
#include "math.h"

//calculates average of values in x
float avg(float* x, int size) {
    if (x == nullptr)
        return 0;
    if (size == 0)
        return 0;
    float u = 0.0;
    for (int i = 0; i < size; i++) {
        u += x[i];
    }
    return u*(1/(float)size);
}

// returns the variance of X and Y
float var(float* x, int size){
    if (size == 0)
        return 0;
    if (x == nullptr)
        return 0;
    float sum = 0.0;
    float u = avg(x, size);
    for (int i = 0; i < size; i++) {
        sum += pow(x[i], 2);
    }
    sum = (1/(float)size)*sum;
    return (sum - pow(u, 2));
}
// returns the covariance of X and Y
float cov(float* x,  float* y, int size) {
    if ((x == nullptr) || (y == nullptr) || (size == 0))
            return 0;
    int i;
    float mult = 0.0;
    for (i = 0; i < size; i++) {
        mult += x[i]*y[i];
    }
    mult = mult*(1/(float)size);
    float avgX = avg(x, size);
    float avgY = avg(y, size);
    return mult - (avgX * avgY);
}
// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {
    if ((x == nullptr) || (y == nullptr) || (size == 0))
        return 0;
    float vx = sqrt(var(x, size));
    float vy = sqrt(var(y, size));
    if ((vx == 0) || (vy == 0))
        return 0;
    return cov(x, y, size) / (vx * vy);
}
// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size) {
    if (points == nullptr) {
        Line *line = new Line(0, 0);
        return *line;
    }
    Point* looping = *points;
    float a = 0.0, b = 0.0;
    float* x = new float[size] ;
    float* y = new float [size];
    for (int i = 0; i < size; i++) {
        x[i] = looping->x;
        y[i] = looping->y;
        looping = looping + sizeof(a);
    }
    a = cov(x, y, size)/var(x,size);
    b = avg(y, size) - a*avg(x, size);
    Line* l = new Line(a,b);
    return *l;
}
// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size) {
    if (points == nullptr)
        return 0;
    Line l = linear_reg(points,size);
    return dev(p, l);
}
// returns the deviation between point p and the line
float dev(Point p,Line l) {
    float num = l.a*p.x + l.b - p.y;
    if (num < 0)
        return p.y - l.a*p.x + l.b;
    return num;
}
//int main()
//{
//    Point* p1 = new Point(3,4);
//    Point* p2 = new Point(2,5);
//    Point* p3 = new Point(6,9);
//    Point* p4 = new Point(7,6);
//    Point* p5 = new Point(15,5);
//    Point* pPoint[5];
//    pPoint[0] = p1;
//    pPoint[1] = p2;
//    pPoint[2] = p3;
//    pPoint[3] = p4;
//    pPoint[4] = p5;
//    Point** toPpoint = pPoint;
//    Line l = linear_reg(toPpoint, 5);
//    float y[] = {13,48,65};
//    int size = 3;
//    float num = var(y, size);
//    float x[] = {30, 94, 7};
//    float num1 = cov(x, y, size);
//    std::cout<<l.a<<std::endl;
//    std::cout<<l.b<<std::endl;
//}