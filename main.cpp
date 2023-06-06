#include <SDL2/SDL.h>
#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include "Color.h"
#include "Point2D.h"
#include "Point3D.h"
#include "Cube.h"
#include "Square.h"

using namespace std;
const int DELAY = 100;
const int spin_speed = 5;
SDL_Window *window;
int window_width = 600, window_height = 600;

int cube_matrix[3][3][3];

Point2D
perspectiveProjection(const Point3D &point, float screenWidth, float screenHeight, float fov, float cameraDistance) {

    float aspectRatio = screenWidth / screenHeight;

    float projectionPlaneDistance = screenHeight / (2.0f * tan(fov / 2.0f));

    Point2D projectedPoint(0, 0);
    projectedPoint.x = (point.x * projectionPlaneDistance) / (point.z + cameraDistance);
    projectedPoint.y = (point.y * projectionPlaneDistance * aspectRatio) / (point.z + cameraDistance);

    projectedPoint.x = (projectedPoint.x + 1.0f) * (screenWidth / 2.0f);
    projectedPoint.y = (1.0f - projectedPoint.y) * (screenHeight / 2.0f);

    return projectedPoint;
}

Point2D get2DCoordinates(Point3D point3D) {
    float fov = 90.0f * (M_PI / 180.0f);
    float cameraDistance = 10.0f;
    Point2D projectedPoint = perspectiveProjection(point3D, window_width, window_height, fov, cameraDistance);
    return projectedPoint;
}

vector<Cube> cubes;

void drawFilledTriangle(SDL_Renderer *renderer, float x1, float y1, float x2, float y2, float x3, float y3, Color col) {
    const std::vector<SDL_Vertex> verts =
            {
                    {SDL_FPoint{x1, y1}, SDL_Color{(Uint8) col.R, (Uint8) col.G, (Uint8) col.B, 255}, SDL_FPoint{0},},
                    {SDL_FPoint{x2, y2}, SDL_Color{(Uint8) col.R, (Uint8) col.G, (Uint8) col.B, 255}, SDL_FPoint{0},},
                    {SDL_FPoint{x3, y3}, SDL_Color{(Uint8) col.R, (Uint8) col.G, (Uint8) col.B, 255}, SDL_FPoint{0},},

            };
    SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
}

void
drawFilledSquare(SDL_Renderer *renderer, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
                 Color col) {
    drawFilledTriangle(renderer, x1, y1, x2, y2, x3, y3, col);
    drawFilledTriangle(renderer, x4, y4, x3, y3, x1, y1, col);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Point points[5] = {
            {static_cast<int>(x1), static_cast<int>(y1)},
            {static_cast<int>(x2), static_cast<int>(y2)},
            {static_cast<int>(x3), static_cast<int>(y3)},
            {static_cast<int>(x4), static_cast<int>(y4)},
            {static_cast<int>(x1), static_cast<int>(y1)}
    };

    SDL_RenderDrawLines(renderer, points, 5);
}

bool view_sort_Z(Square A, Square B) {
    Point3D A_mid = Point3D((A.p1.x + A.p2.x + A.p3.x + A.p4.x) / 4, (A.p1.y + A.p2.y + A.p3.y + A.p4.y) / 4,
                            (A.p1.z + A.p2.z + A.p3.z + A.p4.z) / 4);
    Point3D B_mid = Point3D((B.p1.x + B.p2.x + B.p3.x + B.p4.x) / 4, (B.p1.y + B.p2.y + B.p3.y + B.p4.y) / 4,
                            (B.p1.z + B.p2.z + B.p3.z + B.p4.z) / 4);
    Point3D p1(A_mid.x, A_mid.y, 10);
    Point3D p2(B_mid.x, B_mid.y, 10);
    float distA = A_mid.get_dist(p1);
    float distB = B_mid.get_dist(p2);
    return distB < distA;

}

bool point_inside_triangle(float x1, float y1, float x2, float y2, float x3, float y3, float px, float py) {
    float u, v, w;
    u = ((x2 - x1) * (py - y1) - (y2 - y1) * (px - x1)) / ((x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1));
    v = ((x3 - x2) * (py - y2) - (y3 - y2) * (px - x2)) / ((x3 - x2) * (y1 - y2) - (y3 - y2) * (x1 - x2));
    w = 1.0f - u - v;

    if (u >= 0 && u <= 1 && v >= 0 && v <= 1 && w >= 0 && w <= 1) {
        return true;
    } else {
        return false;
    }
}

bool point_inside_square(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
                         float px, float py) {
    return point_inside_triangle(x1, y1, x2, y2, x3, y3, px, py) ||
           point_inside_triangle(x1, y1, x3, y3, x4, y4, px, py);
}


vector<Square> squares_to_render;

void render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    squares_to_render.clear();
    for (Cube cube: cubes)
        for (Square t: cube.squares) {
            squares_to_render.push_back(t);
        }

    sort(squares_to_render.begin(), squares_to_render.end(), view_sort_Z);
    for (Square t: squares_to_render) {
        Point2D p1 = get2DCoordinates(t.p1);
        Point2D p2 = get2DCoordinates(t.p2);
        Point2D p3 = get2DCoordinates(t.p3);
        Point2D p4 = get2DCoordinates(t.p4);
        drawFilledSquare(renderer, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, t.getColor());

    }
    SDL_RenderPresent(renderer);
}

int sp = 0;
Point3D rot_point;
vector<int> index_to_spin;

void get_rotation_point() {
//TODO : !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    float av_x = 0, av_y = 0, av_z = 0;
    int _cnt = 0;
    for (int k = 0; k < index_to_spin.size(); k++) {
        if (!index_to_spin[k])
            continue;
        Cube c = cubes[k];
        Square s = cubes[k].squares[0];
        av_x += s.p1.x + s.p2.x + s.p3.x + s.p4.x;
        av_y += s.p1.y + s.p2.y + s.p3.y + s.p4.y;
        av_z += s.p1.z + s.p2.z + s.p3.z + s.p4.z;
        _cnt += 4;
    }
    av_x = av_x / (float) _cnt;
    av_y = av_y / (float) _cnt;
    av_z = av_z / (float) _cnt;
    rot_point.x = av_x;
    rot_point.y = av_y;
    rot_point.z = av_z;
}

void logic() {
    if (sp > 0) {
        get_rotation_point();
        for (int k = 0; k < index_to_spin.size(); k++) {
            if (index_to_spin[k] == 1) {
                cubes[k].rotate_on_vector(spin_speed, rot_point.x, rot_point.y, rot_point.z);
            }
        }
        sp -= spin_speed;
    }
}


int px = 0, py = 0;

int getIndex(int xx, int yy) {
    int index = -1;
    for (int k = (int) squares_to_render.size() - 1; k >= 0; k--) {
        Square sq = squares_to_render[k];
        Point2D p1 = get2DCoordinates(sq.p1);
        Point2D p2 = get2DCoordinates(sq.p2);
        Point2D p3 = get2DCoordinates(sq.p3);
        Point2D p4 = get2DCoordinates(sq.p4);
        if (point_inside_square(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y,
                                xx, yy)) {
            index = sq.index;
            break;
        }
    }
    return index;
}

void update_rotation(int &x, int &y, int &z, int &dx, int &dy, int &dz) {
//TODO : !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (dx != 0) {
        if (dx == -1) {
            if (x != 0)
                dx = -1;
            else {
                dx = 0;
                if (z == 0)
                    dz = 1;
                else if (z == 2)
                    dz = -1;
                else if (y == 0)
                    dy = 1;
                else dy = -1;
            }
        } else {
            if (x != 2)
                dx = 1;
            else {
                dx = 0;
                if (z == 0)
                    dz = 1;
                else if (z == 2)
                    dz = -1;
                else if (y == 0)
                    dy = 1;
                else dy = -1;
            }
        }
    } else if (dy != 0) {
        if (dy == -1) {
            if (y != 0)
                dy = -1;
            else {
                dy = 0;
                if (x == 0)
                    dx = 1;
                else if (x == 2)
                    dx = -1;
                else if (z == 0)
                    dz = 1;
                else dz = -1;
            }
        } else {
            if (y != 2)
                dy = 1;
            else {
                dy = 0;
                if (x == 0)
                    dx = 1;
                else if (x == 2)
                    dx = -1;
                else if (z == 0)
                    dz = 1;
                else dz = -1;
            }
        }
    } else if (dz != 0) {
        if (dz == -1) {
            if (z != 0)
                dz = -1;
            else {
                dz = 0;
                if (x == 0)
                    dx = 1;
                else if (x == 2)
                    dx = -1;
                else if (y == 0)
                    dy = 1;
                else dy = -1;
            }
        } else {
            if (z != 2)
                dz = 1;
            else {
                dz = 0;
                if (x == 0)
                    dx = 1;
                else if (x == 2)
                    dx = -1;
                else if (y == 0)
                    dy = 1;
                else dy = -1;
            }
        }
    }
    x += dx;
    y += dy;
    z += dz;
}

void spin_matrix(int x, int y, int z, int dx, int dy, int dz) {
    int sx = x, sy = y, sz = z;
    int cnt = 10;
    vector<pair<int, pair<int, int>>> v;
    index_to_spin.clear();
    index_to_spin.resize(30, 0);
    while (true) {
        v.push_back({x, {y, z}});
        index_to_spin[cube_matrix[x][y][z]] = 1;
        update_rotation(x, y, z, dx, dy, dz);
        if (x == sx && y == sy && sz == z)
            break;
        if (!cnt) {
            break;
        }
        cnt--;
    }

    if (cnt == 0)
        v.clear();
    if (v.size() == 0) {
        cout << "ERROR" << endl;
        return;
    }
    int ai_x = 0, ai_y = 0, ai_z = 0;
    for (int k = 0; k < v.size(); k++) {
        ai_x += v[k].first;
        ai_y += v[k].second.first;
        ai_z += v[k].second.second;
    }
    ai_x /= v.size();
    ai_y /= v.size();
    ai_z /= v.size();
    index_to_spin[cube_matrix[ai_x][ai_y][ai_z]] = 1;
    sp = 90;
    int sv_val = cube_matrix[v[0].first][v[0].second.first][v[0].second.second];
    for (int k = 0; k < v.size() - 1; k++) {
        cube_matrix[v[k].first][v[k].second.first][v[k].second.second] = cube_matrix[v[k + 1].first][v[k +
                                                                                                       1].second.first][v[
                k + 1].second.second];
    }
    cube_matrix[v[v.size() - 1].first][v[v.size() - 1].second.first][v[v.size() - 1].second.second] = sv_val;
}

void spin(int index1, int index2) {
    int dx = 0, dy = 0, dz = 0;
    dx = cubes[index2].x - cubes[index1].x;
    dy = cubes[index2].y - cubes[index1].y;
    dz = cubes[index2].z - cubes[index1].z;
    spin_matrix(cubes[index1].x, cubes[index1].y, cubes[index1].z, dx, dy, dz);

}

int index1 = -1, index2 = -1;

void loop() {
    bool quit = false;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    bool down = false;
    bool onCube = false;
    for (int k = 0; k < cubes.size(); k++) {
        cubes[k].rotate_on_Y(30);
        cubes[k].rotate_on_X(30);
    }

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_RIGHT) {
                } else {
                    px = event.button.x;
                    py = event.button.y;
                    int index = getIndex(px, py);
                    if (index != -1) {
                        onCube = true;
                    }
                    index1 = index;
                    down = true;
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                down = false;
                onCube = false;
                index1 = -1;
                index2 = -1;
            }
            if (event.type == SDL_MOUSEMOTION) {
                if (down) {
                    if (onCube) {
                        int index = getIndex(event.button.x, event.button.y);
                        index2 = index;
                        if (index1 != index2) {
                            if (sp == 0)
                                spin(index1, index2);
                            onCube = false;
                            down = false;
                            index1 = -1;
                            index2 = -1;
                        }

                    } else {
                        float dxa = ((float) (event.button.x - px)) / 6.0f;
                        float dya = ((float) (event.button.y - py)) / 6.0f;
                        for (int k = 0; k < cubes.size(); k++) {
                            cubes[k].rotate_on_Y(dxa);
                            cubes[k].rotate_on_X(dya);
                        }
                        px = event.button.x;
                        py = event.button.y;
                    }

                }
            }

        }
        logic();
        render(renderer);
        SDL_Delay(DELAY);
    }
    SDL_DestroyRenderer(renderer);
}


void get_dimensions() {
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    window_width = current.w;
    window_height = current.h;
}

Cube make_cube(int x, int y, int z, int index) {
    Cube cube(x, y, z, index);

    // Define the 8 vertices of the cube
    Point3D p1(-0.0025f, -0.0025f, 0.0025f);
    Point3D p2(-0.0025f, 0.0025f, 0.0025f);
    Point3D p3(0.0025f, 0.0025f, 0.0025f);
    Point3D p4(0.0025f, -0.0025f, 0.0025f);
    Point3D p5(-0.0025f, -0.0025f, -0.0025f);
    Point3D p6(-0.0025f, 0.0025f, -0.0025f);
    Point3D p7(0.0025f, 0.0025f, -0.0025f);
    Point3D p8(0.0025f, -0.0025f, -0.0025f);

    // Create the 6 faces of the cube using squares
    // Left face
    cube.add_square(Square(p1, p5, p8, p4, Square::YELLOW, index));
    // Bottom face
    cube.add_square(Square(p5, p6, p7, p8, Square::ORANGE, index));
    // Top face
    cube.add_square(Square(p1, p4, p3, p2, Square::RED, index));
    // Front face
    cube.add_square(Square(p1, p2, p6, p5, Square::BLUE, index));
    // Right face
    cube.add_square(Square(p2, p3, p7, p6, Square::WHITE, index));
    // Back face
    cube.add_square(Square(p4, p8, p7, p3, Square::GREEN, index));


    return cube;
}

void make_objects() {
    int index = 0;
    for (float dx = -0.005, x = 0; dx <= 0.005; dx += 0.005, x++)
        for (float dy = -0.005, y = 0; dy <= 0.005; dy += 0.005, y++)
            for (float dz = -0.005, z = 0; dz <= 0.005; dz += 0.005, z++) {
                Cube cube = make_cube(x, y, z, index);
                cube_matrix[(int) x][(int) y][(int) z] = index;
                cube.move(dx, dy, dz);
                cubes.push_back(cube);
                index++;
            }
}

int main() {
    srand(time(0));
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Rubik's Cube", 0, 0, window_width, window_height, SDL_WINDOW_MAXIMIZED);
    get_dimensions();
    make_objects();
    loop();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
