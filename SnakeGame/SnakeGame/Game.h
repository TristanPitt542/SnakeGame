#pragma once
using namespace std;
#include <iostream>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <queue>
#include "Collectable.h"
#include "Wall.h"
#include "Battery.h"

using namespace std;

class Snake;
class Wall;
class Battery;

enum GameState
{
	Waiting,
	Settings,
	Playing,
	HighScore,
	GameOver,
	Exit
};

class Game
{
private:
	GameState m_state{ GameState::Waiting };

	//delay
	int kTimeBetweenTicks{ 300 };

	int Collectable_count = 0;

	int m_players = 2;
	int m_AI = 0;
	
	//highscore 
	int winner{ 0 };
	int highScore{ 0 };

	//vectors
	sf::RenderWindow* m_window{ nullptr };
	vector<Snake*> snakeVector;
	vector<Collectable*> m_collectable;
	priority_queue<int> scoreVector;
	Battery m_battery;
	Wall m_wall;

	bool IsPosFree(sf::Vector2f pos) const;
	sf::Vector2f GetRandomPos() const;
	bool HandleCollision();

	void FrontEnd();
	void SettingsScreen();
	void InGameScreen();
	void HighSchore();
	void EndScreen();
	void Exit();
public:

	void Run();
};
