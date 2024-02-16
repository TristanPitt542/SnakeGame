#include "Wall.h"
#include "Snake.h"
#include "Game.h"

void Wall::Render(sf::RenderWindow& window)
{
    float headSize = Snake::GetRadius() * 2.0f;

    //Horisontal
    sf::RectangleShape rect_hor(sf::Vector2f(window.getSize().x, headSize));
    rect_hor.setFillColor(sf::Color(128, 128, 128, 255));

    //draw wall on top
    rect_hor.setPosition({ 0, 0 });
    window.draw(rect_hor);
    //draw wall on bottom
    rect_hor.setPosition({ 0,window.getSize().y - headSize });
    window.draw(rect_hor);

    //Vertical
    sf::RectangleShape rect_ver(sf::Vector2f(headSize, window.getSize().y));
    rect_ver.setFillColor(sf::Color(128, 128, 128, 255));
    //draw wall on left
    rect_ver.setPosition({ 0, 0 });
    window.draw(rect_ver);

    //draw wall on right
    rect_ver.setPosition({window.getSize().x - headSize, 0});
    window.draw(rect_ver);
};