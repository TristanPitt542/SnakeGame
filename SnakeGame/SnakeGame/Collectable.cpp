#include "Collectable.h"
#include "Snake.h"
#include "Game.h"
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio.hpp>

void Collectable::Spawn(sf::Vector2f pos)
{
	m_position = pos;
	m_visible = true;
}

void Collectable::Render(sf::RenderWindow& window)
{
	if (!m_visible)
		return;

	sf::CircleShape shape(Snake::GetRadius(), m_sides);

	shape.setPosition(m_position);
	shape.setFillColor(sf::Color::Yellow);
	shape.setOrigin(sf::Vector2f(Snake::GetRadius(), Snake::GetRadius()));
	shape.setRotation(m_rotation);
	window.draw(shape);

}
