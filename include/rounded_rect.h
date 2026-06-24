#pragma once
#ifndef ROUNDED_RECT_H
#define ROUNDED_RECT_H

#include <SFML/Graphics.hpp>
#include <cmath>

// Tao hinh chu nhat bo goc dang sf::ConvexShape.
// Cong thuc getPoint() theo SFML Wiki (Draw Rounded Rectangle).
inline sf::ConvexShape createRoundedRect(sf::Vector2f size,
                                         float radius = 0.f,
                                         unsigned int cornerPointCount = 8) {
    unsigned int totalPoints = cornerPointCount * 4;
    sf::ConvexShape shape(totalPoints);

    if (cornerPointCount == 0) return shape;

    // Gioi han radius khong vuot qua nua canh ngan nhat
    float r = radius;
    float maxR = (size.x < size.y ? size.x : size.y) / 2.f;
    if (r > maxR) r = maxR;

    static const float PI = 3.141592654f;
    float deltaAngle = 90.f / (float)(cornerPointCount - 1);

    for (unsigned int index = 0; index < totalPoints; index++) {
        unsigned int centerIndex = (unsigned int)(index / cornerPointCount);

        sf::Vector2f center;
        switch (centerIndex) {
        case 0: center = { size.x - r, r }; break;            // goc tren-phai
        case 1: center = { r, r }; break;                     // goc tren-trai
        case 2: center = { r, size.y - r }; break;            // goc duoi-trai
        default: center = { size.x - r, size.y - r }; break;  // goc duoi-phai
        }

        float angle = deltaAngle * (float)(index - centerIndex) * PI / 180.f;
        shape.setPoint(index, sf::Vector2f(r * std::cos(angle) + center.x,
            -r * std::sin(angle) + center.y));
    }

    return shape;
}

#endif // ROUNDED_RECT_H
