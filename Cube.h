//
// Created by beso on 04/06/23.
//

#ifndef RUBIKCUBE_CUBE_H
#define RUBIKCUBE_CUBE_H

#include <algorithm>
#include <vector>
#include <math.h>
#include "Square.h"

using namespace std;

class Cube {
public:
    int x, y, z, index;
    vector<Square> squares;

    static float degreesToRadians(float degrees);

    Cube(int x1, int y1, int z1, int index1);

    void apply_matrix(float matrix[3][3]);

    void add_square(Square square);

    void multiply_matrix_by_matrix(const float A[3][3], const float B[3][3], float result[3][3]);

    void multiply_matrix_by_number(float A[3][3], float number);

    void add_matrix_to_matrix(const float A[3][3], const float B[3][3], float result[3][3]);

    void rotate_on_vector(float alpha, float x, float y, float z);

    void move(float dx, float dy, float dz);

    void rotate_on_X(float alpha);

    void rotate_on_Y(float alpha);

    void rotate_on_Z(float alpha);
};


#endif //RUBIKCUBE_CUBE_H
