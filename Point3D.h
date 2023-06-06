//
// Created by beso on 04/06/23.
//

#ifndef RUBIKCUBE_POINT3D_H
#define RUBIKCUBE_POINT3D_H


#include <cmath>

class Point3D {

public:
    float x;
    float y;
    float z;

    Point3D() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    Point3D(float x1, float y1, float z1) {
        x = x1;
        y = y1;
        z = z1;
    }

    float get_dist(Point3D A) {
        float dx = x - A.x;
        float dy = y - A.y;
        float dz = z - A.z;

        return sqrt(dx * dx + dy * dy + dz * dz);
    }
};


#endif //RUBIKCUBE_POINT3D_H
