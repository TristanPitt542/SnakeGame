#include "Snake.h"
#include <iostream>
#include "Collectable.h"
#include "Battery.h"
#include "Wall.h"
#include <SFML/Audio.hpp>

using namespace std;

Snake::Snake(sf::Vector2f pos, int ctrl, sf::Color col, float rad) : m_color(col), m_controlScheme(ctrl)
{
	m_tail.push_back(pos);
};

void Snake::Render(sf::RenderWindow& window)
{
	cout << m_batteryRemaining << endl;
	if (m_alive == false)
	{
		return;
	}

	if (IsBatteryActive())
	{
		CalculateBatteryEffectArea();

		sf::CircleShape batShape(GetRadius(), 4);
		batShape.setOrigin(sf::Vector2f(Snake::GetRadius(), Snake::GetRadius()));
		batShape.setFillColor(sf::Color(255,255,255,255));

		for (auto& it : m_batteryEffectArea)
		{
			batShape.setPosition(it);
			window.draw(batShape);
		}
	}
	size_t renderCount{ 0 };

	sf::CircleShape shape(Snake::GetRadius());

	shape.setFillColor(m_color);
	shape.setOrigin(sf::Vector2f(Snake::GetRadius(), Snake::GetRadius()));

	for (auto& it : m_tail)
	{
		shape.setPosition(it);
		window.draw(shape);
	}
}

void Snake::CheckCollision(vector<Collectable*>& collectables)
{
	for (Collectable* c : collectables)
	{
		if (c->GetVisible() && c->GetPosition() == m_tail.front())
		{
			AddScore(c->GetScore() * c->GetRandomSides());
			c->Die();
		}
	}
}

bool Snake::CheckSelfCollision() const
{
	std::list<sf::Vector2f>::const_iterator it{ m_tail.begin() };
	
	it++;

	while (it != m_tail.end())
	{
		if (*it == m_tail.front())
			return true;

		it++;
	}

	return false;
	return 0;
}

bool Snake::CheckCollision(Snake* other)
{
	for (auto it : m_tail)
	{
		if (it == other->m_tail.front())
			return true;
	}

	if (IsBatteryActive())
	{
		for (auto &pos : m_batteryEffectArea)
		{
			if (other->GetPosition() == pos)
			{
				other->Die();
				cout << "You got zapped" << endl;
				break;
			}
		}
	}
	return false;
}

bool Snake::CheckCollision(Battery& battery)
{
	if (GetPosition() == battery.GetPosition())
	{
		m_batteryRemaining = 20 + rand() % 21;
		return true;
	}

	return false;
}

void Snake::Update()
{
	sf::Vector2f newPosition{ m_tail.front() };
	//Direction
	switch (m_direction)
	{
	case Direction::North:
		newPosition.y -= Snake::GetRadius() * 2.0f;
		break;
	case Direction::East:
		newPosition.x += Snake::GetRadius() * 2.0f;
		break;
	case Direction::South:
		newPosition.y += Snake::GetRadius() * 2.0f;
		break;
	case Direction::West:
		newPosition.x -= Snake::GetRadius() * 2.0f;
		break;
	default:
		break;
	}

	m_tail.push_front(newPosition);

	if (m_growAmount == 0)
	{
		m_tail.pop_back();
	}
	else
	{
		m_growAmount --;
	}

	if (m_controlScheme == 0)
	{
		//Change Direction
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) and (m_direction != Direction::South))
		{
			m_direction = Direction::North;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) and (m_direction != Direction::West))
		{
			m_direction = Direction::East;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) and (m_direction != Direction::North))
		{
			m_direction = Direction::South;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) and (m_direction != Direction::East))
		{
			m_direction = Direction::West;
		}
	}
	else if (m_controlScheme == 1)
	{
		//Change Direction
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) and (m_direction != Direction::South))
		{
			m_direction = Direction::North;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) and (m_direction != Direction::West))
		{
			m_direction = Direction::East;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) and (m_direction != Direction::North))
		{
			m_direction = Direction::South;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) and (m_direction != Direction::East))
		{
			m_direction = Direction::West;
		}
	}

	if (m_batteryRemaining > 0)
	{
		m_batteryRemaining -= 1;
	}
};

void Snake::AddScore(int score)
{
	m_growAmount += score;
	m_score += score;
}

bool Snake::DoYouCoverPos(sf::Vector2f pos) const
{
	for (auto& it : m_tail)
	{
		if (it == pos)
			return true;
	}
	return false;
}

void Snake::CalculateBatteryEffectArea()
{
	m_batteryEffectArea.clear();

	for (auto& it : m_tail)
	{
		m_batteryEffectArea.push_back(it + sf::Vector2f(0, -Snake::GetRadius() * 2.0f));
		m_batteryEffectArea.push_back(it + sf::Vector2f(0,  Snake::GetRadius() * 2.0f));
		m_batteryEffectArea.push_back(it + sf::Vector2f(-Snake::GetRadius() * 2.0f, 0));
		m_batteryEffectArea.push_back(it + sf::Vector2f( Snake::GetRadius() * 2.0f, 0));

		m_batteryEffectArea.push_back(it + sf::Vector2f(0, -Snake::GetRadius() * 4.0f));
		m_batteryEffectArea.push_back(it + sf::Vector2f(0,  Snake::GetRadius() * 4.0f));
		m_batteryEffectArea.push_back(it + sf::Vector2f(-Snake::GetRadius() * 4.0f, 0));
		m_batteryEffectArea.push_back(it + sf::Vector2f( Snake::GetRadius() * 4.0f, 0));
	}
}

