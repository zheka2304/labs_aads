#include <cmath>
#include "canvas.h"

Canvas::Canvas(int width, int height) : width(width), height(height) {
    canvas = new char[height * (width + 1) + 1];
    memset(canvas, ' ', (size_t) (height * (width + 1) + 1));
    for (int i = 0; i < height; i++) {
        canvas[i * (width + 1) + width] = '\n';
    }
    canvas[height * (width + 1)] = '\0';
}

void Canvas::print() {
    std::cout << canvas;
}

void Canvas::set_abs(int x, int y, char c, bool keepArrow) {
    if (x < 0 || y < 0 || x > width || y > width) {
        return;
    }
    if (keepArrow) {
        char check = canvas[y * (width + 1) + x];
        if (check == 'V' || check == '^' || check == '>' || check == '<') {
            return;
        }
    }
    canvas[y * (width + 1) + x] = c;
}

void Canvas::set(float x, float y, char c, bool keepArrow) {
    set_abs((int) lroundf(x * (width - 1)), (int) lroundf(y * (height - 1)), c, keepArrow);
}

Canvas::Point Canvas::line_abs(int ix1, int iy1, int ix2, int iy2, bool keepArrow) {
    int ixx1 = ix1;
    int iyy1 = iy1;
    while (ix1 < ix2 - abs(iy1 - iy2)) {
        set_abs(ix1++, iy1, '-', keepArrow);
    }
    while (ix1 > ix2 + abs(iy1 - iy2)) {
        set_abs(ix1--, iy1, '-', keepArrow);
    }
    while (iy1 < iy2 - abs(ix1 - ix2)) {
        set_abs(ix1, iy1++, '|', keepArrow);
    }
    while (iy1 > iy2 + abs(ix1 - ix2)) {
        set_abs(ix1, iy1--, '|', keepArrow);
    }

    int lx = ix1, ly = iy1;
    while (iy1 != iy2) {
        if (iy1 < iy2) {
            if (ix1 < ix2) {
                set_abs(ix1++, iy1++, '\\', keepArrow);
            } else if (ix1 == ix2) {
                set_abs(ix1, iy1++, '-', keepArrow);
            } else {
                set_abs(ix1--, iy1++, '/', keepArrow);
            }
        } else {
            if (ix1 < ix2) {
                set_abs(ix1++, iy1--, '/', keepArrow);
            } else if (ix1 == ix2) {
                set_abs(ix1, iy1--, '-', keepArrow);
            } else {
                set_abs(ix1--, iy1--, '\\', keepArrow);
            }
        }
    }

    if (pow(lx - ix2, 2) + pow(ly - iy2, 2) > pow(lx - ixx1, 2) + pow(ly - iyy1, 2)) {
        return {(lx * 0.5f + ix2 * 0.5f) / (width - 1), (ly * 0.5f + iy2 * 0.5f) / (height - 1)};
    } else {
        return {(lx * 0.5f + ixx1 * 0.5f) / (width - 1), (ly * 0.5f + iyy1 * 0.5f) / (height - 1)};
    }
}

Canvas::Point Canvas::line(float x1, float y1, float x2, float y2,bool keepArrow) {
    int ix1 = (int) lroundf(x1 * (width - 1));
    int ix2 = (int) lroundf(x2 * (width - 1));
    int iy1 = (int) lroundf(y1 * (height - 1));
    int iy2 = (int) lroundf(y2 * (height - 1));
    return line_abs(ix1, iy1, ix2, iy2, keepArrow);
}


Canvas::Point Canvas::arrow(float x1, float y1, float x2, float y2, bool offset) {
    int ix1 = (int) lroundf(x1 * (width - 1));
    int ix2 = (int) lroundf(x2 * (width - 1));
    int iy1 = (int) lroundf(y1 * (height - 1));
    int iy2 = (int) lroundf(y2 * (height - 1));

    if (offset) {
        if (ix1 + 1 < ix2) {
            ix1++;
            ix2--;
        } else if (ix1 - 1 > ix2) {
            ix1--;
            ix2++;
        }
        if (iy1 + 1 < iy2) {
            iy1++;
            iy2--;
        } else if (iy1 - 1 > iy2) {
            iy1--;
            iy2++;
        }
    }

    auto result = line_abs(ix2, iy2, ix1, iy1, true);

    if (abs(ix1 - ix2) < abs(iy1 - iy2)) {
        if (iy1 > iy2) {
            set_abs(ix2, iy2 + 1, '^');
        } else {
            set_abs(ix2, iy2 - 1, 'V');
        }
    } else if (ix2 > ix1) {
        set_abs(ix2 - 1, iy2, '>');
    } else {
        set_abs(ix2 + 1, iy2, '<');
    }

    return result;
}


void Canvas::text(float fx, float fy, std::string text, bool center, bool vertical) {
    int x = (int) lroundf(fx * (width - 1));
    int y = (int) lroundf(fy * (height - 1));
    if (center) {
        if (vertical) {
            y -= text.length() / 2;
        } else {
            x -= text.length() / 2;
        }
    }
    for (int i = 0; i < text.length(); i++) {
        set_abs(x + (vertical ? 0 : i), y + (vertical ? i : 0), text[i]);
    }
}

Canvas::~Canvas() {
    delete[] (canvas);
}
