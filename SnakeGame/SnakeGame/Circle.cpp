#include "Circle.h"
#include <SFML/Graphics.hpp>

Circle::Circle(sf::Vector2f pos, sf::Color color, float rotation, float rad) : Shape(pos, color, rotation), m_radius(rad)
{
}

void Circle::Render(sf::RenderWindow& window)
{
    sf::CircleShape shape(m_radius);

    shape.setPosition(m_position);
    shape.setFillColor(m_color);
    shape.setOrigin(sf::Vector2f(m_radius, m_radius));
    shape.setRotation(m_rotation);
    window.draw(shape);
    Update();
};

void Circle::Grow()
{
    m_radius += .1f;
};