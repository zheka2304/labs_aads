#include <iostream>
#include <string.h>

#ifndef M_CANVAS_H
#define M_CANVAS_H

class Canvas {
public:
    struct Point {
    public:
        float x, y;
    };

    int width;
    int height;
    char* canvas;

    Canvas(int width, int height);
    void print();
    void set_abs(int x, int y, char c, bool keepArrow = false);
    void set(float x, float y, char c, bool keepArrow = false);
    Point line_abs(int ix1, int iy1, int ix2, int iy2, bool keepArrow = false);
    Point line(float x1, float y1, float x2, float y2, bool keepArrow = false);
    Point arrow(float x1, float y1, float x2, float y2, bool offset = false);
    void text(float fx, float fy, std::string text, bool center = true, bool vertical = false);
    ~Canvas();
};

#endif
