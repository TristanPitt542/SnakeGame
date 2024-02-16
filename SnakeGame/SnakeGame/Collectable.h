#pragma once
#include "iostream"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace std;
class Collectable
{
private:
	int m_points = 1;
	int m_sides = GetRandomSides();
	sf::Vector2f m_position;
	bool m_visible{ false };
	float m_rotation;

public:
	bool GetVisible() const { return m_visible; };
	int GetScore() const { return m_points; };
	sf::Vector2f GetPosition() const { return m_position; }
	int GetRandomSides() { return rand() % 4 + 4; }

	void Spawn(sf::Vector2f pos);
	void Render(sf::RenderWindow& window);
	void Die() { m_visible = false; };
};