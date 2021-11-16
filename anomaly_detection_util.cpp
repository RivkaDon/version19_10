// Hodaya Raz 211540174, Rivka Doniger 324584531

#include <iostream>
#include "anomaly_detection_util.h"
#include "math.h"

//calculates average of values in x
float avg(float* x, int size) {
    if (x == nullptr)
        throw std::runtime_error("null pointer error");
    if (size == 0)
        throw std::runtime_error("no input data");
    float u = 0.0;
    for (int i = 0; i < size; i++) {
        u += x[i];
    }
    return u*(1/(float)size);
}

// returns the variance of X and Y
float var(float* x, int size){
    if (size == 0)
        throw std::runtime_error("no input data");
    if (x == nullptr)
        throw std::runtime_error("null pointer error");
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
    if ((x == nullptr) || (y == nullptr))
        throw std::runtime_error("null pointer error");
    if (size == 0)
        throw std::runtime_error("no input data");
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
    if ((x == nullptr) || (y == nullptr))
        throw std::runtime_error("null pointer error");
    if (size == 0)
        throw std::runtime_error("no input data");
    float vx = sqrt(var(x, size));
    float vy = sqrt(var(y, size));
    if ((vx == 0) || (vy == 0))
        return 0;
    return cov(x, y, size) / (vx * vy);
}
// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size) {
    if (points == nullptr) {
        return Line(0, 0);
    }
    float a = 0.0, b = 0.0;
    float x[size];
    float y[size];
    for (int i = 0; i < size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    a = cov(x, y, size)/var(x,size);
    b = avg(y, size) - a*(avg(x, size));
    return Line(a,b);
}
// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size) {
    if (points == nullptr)
        throw std::runtime_error("null pointer error");
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