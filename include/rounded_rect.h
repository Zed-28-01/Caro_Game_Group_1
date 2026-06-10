#pragma once
#ifndef ROUNDED_RECT_H
#define ROUNDED_RECT_H

#include <SFML/Graphics.hpp>
#include <cmath>

// ============================================================
// RoundedRectangleShape (V2 #30 / #31)
// ============================================================
// sf::Shape subclass ve hinh chu nhat BO GOC. Header-only (inline) de
// khong phai sua CaroGame.vcxproj. Dung cho UI polish: hang list, panel,
// thanh scrollbar, button frame... (se tai dung trong Sprint 4 #31).
//
// Cach dung:
//   RoundedRectangleShape r(sf::Vector2f(w, h), 8.f);
//   r.setOrigin(w / 2.f, h / 2.f);
//   r.setPosition(cx, cy);
//   r.setFillColor(...);
//   window.draw(r);
//
// Cong thuc getPoint() theo SFML Wiki (Source: Draw Rounded Rectangle).
class RoundedRectangleShape : public sf::Shape {
public:
    explicit RoundedRectangleShape(sf::Vector2f size = sf::Vector2f(0.f, 0.f),
        float radius = 0.f, unsigned int cornerPointCount = 8) {
        m_size = size;
        m_radius = radius;
        m_cornerPointCount = cornerPointCount;
        update();
    }

    void setSize(sf::Vector2f size) { m_size = size; update(); }
    sf::Vector2f getSize() const { return m_size; }

    void setCornerRadius(float radius) { m_radius = radius; update(); }
    float getCornerRadius() const { return m_radius; }

    void setCornerPointCount(unsigned int count) {
        m_cornerPointCount = count;
        update();
    }

    virtual std::size_t getPointCount() const override {
        return m_cornerPointCount * 4;
    }

    virtual sf::Vector2f getPoint(std::size_t index) const override {
        if (m_cornerPointCount == 0) return sf::Vector2f(0.f, 0.f);

        // Gioi han radius khong vuot qua nua canh ngan nhat
        float r = m_radius;
        float maxR = (m_size.x < m_size.y ? m_size.x : m_size.y) / 2.f;
        if (r > maxR) r = maxR;

        static const float PI = 3.141592654f;
        float deltaAngle = 90.f / (float)(m_cornerPointCount - 1);
        unsigned int centerIndex = (unsigned int)(index / m_cornerPointCount);

        sf::Vector2f center;
        switch (centerIndex) {
        case 0: center = { m_size.x - r, r }; break;            // goc tren-phai
        case 1: center = { r, r }; break;                       // goc tren-trai
        case 2: center = { r, m_size.y - r }; break;            // goc duoi-trai
        default: center = { m_size.x - r, m_size.y - r }; break; // goc duoi-phai
        }

        float angle = deltaAngle * (float)(index - centerIndex) * PI / 180.f;
        return sf::Vector2f(r * std::cos(angle) + center.x,
            -r * std::sin(angle) + center.y);
    }

private:
    sf::Vector2f m_size;
    float m_radius;
    unsigned int m_cornerPointCount;
};

#endif // ROUNDED_RECT_H
