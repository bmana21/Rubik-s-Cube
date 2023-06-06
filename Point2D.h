//
// Created by beso on 04/06/23.
//

#ifndef RUBIKCUBE_POINT2D_H
#define RUBIKCUBE_POINT2D_H


class Point2D {

public:
    float x;
    float y;

    Point2D() {
        x = 0.0f;
        y = 0.0f;
    }

    Point2D(float x1, float y1) {
        x = x1;
        y = y1;
    }
};


#endif //RUBIKCUBE_POINT2D_H
