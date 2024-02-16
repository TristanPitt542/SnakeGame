#include "Battery.h"
#include "Snake.h"

void Battery::Render(sf::RenderWindow& window)
{
	sf::CircleShape shape(Snake::GetRadius());

	shape.setPosition(m_position);
	shape.setFillColor(sf::Color::Cyan);
	shape.setOrigin(sf::Vector2f(Snake::GetRadius(), Snake::GetRadius()));
	window.draw(shape);
}

void Battery::Update()
{
}
