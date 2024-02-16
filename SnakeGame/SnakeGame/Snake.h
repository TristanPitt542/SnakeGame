#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <set>

using namespace std;

class Collectable;
class Battery;
class Wall;

//dir
enum Direction
{
	North,
	East,
	West,
	South
};

class Snake
{
private:
	list<sf::Vector2f> m_tail;

	//snake var
	int m_controlScheme{ 0 };
	int m_growAmount{ 0 };
	int m_score{ 0 };

	sf::Color m_color;
	Direction m_direction{ Direction::East };

	bool m_alive{ true };

	int m_batteryRemaining{ 0 };

	vector<sf::Vector2f> m_batteryEffectArea;

	void CalculateBatteryEffectArea();

public:
	Snake(sf::Vector2f pos, int controlScheme, sf::Color col, float rad);
	void Render(sf::RenderWindow& window);
	void Update();
	void Die() { m_alive = false; };
	void AddScore(int score);
	bool DoYouCoverPos(sf::Vector2f pos) const;

	//Get functions
	bool GetAlive() { return m_alive; }
	int GetScore() { return m_score; }
	sf::Vector2f GetPosition() const { return m_tail.front(); }
	static float GetRadius() { return 10.0f; }

	//Collisions
	bool CheckCollision(Snake* other);
	bool CheckCollision(Battery& battery);
	void CheckCollision(vector<Collectable*>& collectables);
	bool CheckSelfCollision() const;

	//Battery
	bool IsBatteryActive() const { return (m_batteryRemaining > 0); }
};