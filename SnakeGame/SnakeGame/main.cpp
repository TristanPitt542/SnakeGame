#include "Game.h"
#include <SFML/Audio.hpp>

void Pickup(sf::SoundBuffer& buffer, sf::Sound sound)
{
    sound.setBuffer(buffer);
    sound.play();
}

int main()
{
    sf::SoundBuffer buffer;
    sf::Sound sound;

    Pickup(buffer, sound);
    //run game
    Game newGame;
    newGame.Run();

    return 0;
}