//
// Created by beso on 04/06/23.
//

#ifndef RUBIKCUBE_COLOR_H
#define RUBIKCUBE_COLOR_H


class Color {

public:
    int R;
    int G;
    int B;

    Color(int r, int g, int b) : R(r), G(g), B(b) {}

    static Color WHITE() { return {255, 255, 255}; }

    static Color ORANGE() { return {255, 165, 0}; }

    static Color BLUE() { return {0, 0, 255}; }

    static Color RED() { return {255, 0, 0}; }

    static Color GREEN() { return {0, 255, 0}; }

    static Color YELLOW() { return {255, 255, 0}; }

    static Color PURPLE() { return {160, 0, 255}; }
};


#endif //RUBIKCUBE_COLOR_H
