//
// Created by beso on 04/06/23.
//

#include "Cube.h"
#include "Point2D.h"
#include "Point3D.h"

float Cube::degreesToRadians(float degrees) {
    return degrees * (M_PI / 180.0);
}

Cube::Cube(int x1, int y1, int z1, int index1) {
    x = x1;
    y = y1;
    z = z1;
    index = index1;
}

void Cube::add_square(Square square) {
    squares.push_back(square);
}

void Cube::apply_matrix(float matrix[3][3]) {
    for (int k = 0; k < squares.size(); k++) {
        Point3D p[4] = {squares[k].p1, squares[k].p2, squares[k].p3, squares[k].p4};
        for (int i = 0; i < 4; i++) {
            float x = p[i].x, y = p[i].y, z = p[i].z;
            float nx = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z;
            float ny = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z;
            float nz = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z;
            p[i] = Point3D(nx, ny, nz);
        }
        squares[k].p1 = p[0];
        squares[k].p2 = p[1];
        squares[k].p3 = p[2];
        squares[k].p4 = p[3];
    }
}

void Cube::multiply_matrix_by_matrix(const float A[3][3], const float B[3][3], float result[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result[i][j] = 0.0f;
            for (int k = 0; k < 3; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void Cube::multiply_matrix_by_number(float A[3][3], float number) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            A[i][j] *= number;
        }
    }
}

void Cube::add_matrix_to_matrix(const float A[3][3], const float B[3][3], float result[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }
}


void Cube::rotate_on_vector(float alpha, float x, float y, float z) {
    float rad = degreesToRadians(alpha);

    float vlength = sqrt(x * x + y * y + z * z);
    x /= vlength;
    y /= vlength;
    z /= vlength;
    float I[3][3] = {
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
    };
    float W[3][3] = {
            0, -z, y,
            z, 0, -x,
            -y, x, 0
    };
    float W2[3][3];
    multiply_matrix_by_matrix(W, W, W2);
    float rotationMatrix[3][3], sum_of_two[3][3];
    multiply_matrix_by_number(W, sin(rad));
    multiply_matrix_by_number(W2, sin(rad / 2) * sin(rad / 2) * 2);
    add_matrix_to_matrix(I, W, sum_of_two);
    add_matrix_to_matrix(sum_of_two, W2, rotationMatrix);
    apply_matrix(rotationMatrix);
}


void Cube::move(float dx, float dy, float dz) {

    for (int k = 0; k < squares.size(); k++) {
        Point3D p[4] = {squares[k].p1, squares[k].p2, squares[k].p3, squares[k].p4};
        for (int i = 0; i < 4; i++) {
            float x = p[i].x, y = p[i].y, z = p[i].z;
            float nx = x + dx;
            float ny = y + dy;
            float nz = z + dz;
            p[i] = Point3D(nx, ny, nz);
        }
        squares[k].p1 = p[0];
        squares[k].p2 = p[1];
        squares[k].p3 = p[2];
        squares[k].p4 = p[3];
    }


}

void Cube::rotate_on_X(float alpha) {
    float deg = degreesToRadians(alpha);
    float rot[3][3] = {
            1, 0, 0,
            0, cos(deg), -sin(deg),
            0, sin(deg), cos(deg)
    };
    apply_matrix(rot);
}

void Cube::rotate_on_Y(float alpha) {
    float deg = degreesToRadians(alpha);
    float rot[3][3] = {
            cos(deg), 0, sin(deg),
            0, 1, 0,
            -sin(deg), 0, cos(deg)
    };
    apply_matrix(rot);
}

void Cube::rotate_on_Z(float alpha) {
    float deg = degreesToRadians(alpha);
    float rot[3][3] = {
            cos(deg), -sin(deg), 0,
            sin(deg), cos(deg), 0,
            0, 0, 1
    };
    apply_matrix(rot);
}
