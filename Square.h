//
// Created by beso on 04/06/23.
//

#ifndef RUBIKCUBE_SQUARE_H
#define RUBIKCUBE_SQUARE_H

#include "Color.h"
#include "Point3D.h"

class Square {
public:
    enum faceColor {
        WHITE, YELLOW, RED, ORANGE, BLUE, GREEN, PURPLE
    };
    faceColor color;
    Point3D p1, p2, p3, p4;
    int index;

    Square(Point3D p11, Point3D p21, Point3D p31, Point3D p41, faceColor facecolor, int index1) : p1(p11), p2(p21),
                                                                                                  p3(p31),
                                                                                                  p4(p41),
                                                                                                  color(facecolor),
                                                                                                  index(index1) {}

    Color getColor();
};


#endif //RUBIKCUBE_SQUARE_H
