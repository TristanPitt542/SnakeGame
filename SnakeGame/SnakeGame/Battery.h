#pragma once

#include <SFML/Graphics.hpp>

class Battery
{
private:
	sf::Vector2f m_position;
	float m_ringRatio{ 0.1f };
	bool m_grow{ true };

public:
	void Render(sf::RenderWindow& window);
	void Update();
	sf::Vector2f GetPosition() const { return m_position; }

	void ChangePosition(sf::Vector2f position) { m_position = position; }
};