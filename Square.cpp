//
// Created by beso on 04/06/23.
//

#include "Square.h"

Color Square::getColor() {
    if (color == WHITE)
        return Color::WHITE();
    if (color == YELLOW)
        return Color::YELLOW();
    if (color == RED)
        return Color::RED();
    if (color == ORANGE)
        return Color::ORANGE();
    if (color == BLUE)
        return Color::BLUE();
    if (color == GREEN)
        return Color::GREEN();
    if (color == PURPLE)
        return Color::PURPLE();
    return Color::WHITE();
}