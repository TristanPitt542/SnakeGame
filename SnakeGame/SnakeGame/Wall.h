#pragma once
#include <SFML/Graphics.hpp>
#include <list>

class Wall
{
private:
	sf::Vector2f m_position;

public:
	sf::Vector2f GetPosition() const { return m_position; }
	void Render(sf::RenderWindow& window);
};

