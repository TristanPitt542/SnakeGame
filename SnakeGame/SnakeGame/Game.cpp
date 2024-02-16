#include "Game.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "Wall.h"
#include "Collectable.h"
#include <stdlib.h>
#include "Snake.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

constexpr int kScreenWidth {  800 };
constexpr int kScreenHeight { 600 };

float headSize = Snake::GetRadius() * 2.0f;

bool Game::IsPosFree(sf::Vector2f pos) const
{
    for (Snake* s : snakeVector)
    {
        if (s->DoYouCoverPos(pos))
            return false;
    }

    for (Collectable* c : m_collectable)
    {
        if (c->GetVisible() && c->GetPosition() == pos)
            return false;
    }

    return true;
}

sf::Vector2f Game::GetRandomPos() const
{
    int headSize = Snake::GetRadius() * 2;
    int rangeOnX = kScreenWidth - (headSize * 2);
    int rangeOnY = kScreenHeight - (headSize * 2);
    sf::Vector2f pos;
    do
    {
        float x = ((rand() % rangeOnX) / headSize) * headSize + headSize + headSize / 2;
        float y = ((rand() % rangeOnY) / headSize) * headSize + headSize + headSize / 2;

        pos.x = (float)x;
        pos.y = (float)y;
    } while (!IsPosFree(pos));

    return pos;
}

bool Game::HandleCollision()
{
    //collectables collision
    for (Snake* s : snakeVector)
    {
        s->CheckCollision(m_collectable);
    }

    //other snake collision
    for (size_t i = 0; i < snakeVector.size(); i++)
    {
        for (size_t j = i + 1; j < snakeVector.size(); j++)
        {
            bool collided{ snakeVector[i]->CheckCollision(snakeVector[j]) };
            if (collided)
            {
                cout << "2 Snakes Collisided" << endl;
                return true;
            }

        }
        return false;
    }
}

//Main menu
void Game::FrontEnd()
{
    sf::Font font;

    if (!font.loadFromFile("Data/Fonts/menu_font.ttf"))
    {
        cout << "Error loading font" << endl;
        return;
    }
    //title
    sf::Text title("Main Menu", font, 50);
    title.setPosition(kScreenWidth/2, 50);
    title.setOrigin(140, 25);

    //playbutton
    sf::RectangleShape playButton(sf::Vector2f(200, 50));
    playButton.setFillColor(sf::Color::Green);
    playButton.setPosition(300, 200);

    //play text
    sf::Text playButtonText("Play", font, 30);
    playButtonText.setPosition(355, 210);
    playButtonText.setColor(sf::Color::White);

    //Highscore button
    sf::RectangleShape HSButton(sf::Vector2f(200, 50));
    HSButton.setFillColor(sf::Color::Red);
    HSButton.setPosition(300, 300);

    //Highscore text 
    sf::Text HSButtonText("Highscore", font, 30);
    HSButtonText.setPosition(310, 308);
    HSButtonText.setColor(sf::Color::White);

    //quit button
    sf::RectangleShape quitButton(sf::Vector2f(200, 50));
    quitButton.setFillColor(sf::Color::Red);
    quitButton.setPosition(300, 400);

    //quit text 
    sf::Text quitButtonText("Quit", font, 30);
    quitButtonText.setPosition(360, 410);
    quitButtonText.setColor(sf::Color::White);

    while(m_window->isOpen())
    {
        sf::Event event;
        while (m_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window->close();
            }
        }

        //button pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);
            sf::FloatRect playbounds = playButton.getGlobalBounds();
            sf::FloatRect quitbounds = quitButton.getGlobalBounds();
            sf::FloatRect HSbounds = HSButton.getGlobalBounds();
            if (playbounds.contains(mousePos.x, mousePos.y))
            {
                m_window->clear();
                m_state = GameState::Settings;
                return;
            }
            else if (quitbounds.contains(mousePos.x, mousePos.y))
            {
                m_window->clear();
                m_state = GameState::Exit;
                return;
            }
            else if (HSbounds.contains(mousePos.x, mousePos.y))
            {
                m_window->clear();
                m_state = GameState::HighScore;
                return;
            }
        }

        m_window->clear();

        m_window->draw(title);
        m_window->draw(playButton);
        m_window->draw(playButtonText);
        m_window->draw(quitButton);
        m_window->draw(quitButtonText);
        m_window->draw(HSButton);
        m_window->draw(HSButtonText);

        m_window->display();
    }
}

//Ingame
void Game::InGameScreen() 
{
    //Load Font
    sf::Font font;

    if (!font.loadFromFile("Data/Fonts/menu_font.ttf"))
    {
        cout << "Error loading font" << endl;
        return;
    }

    //Spawn snake
    Snake* snake1 = new Snake(GetRandomPos(), 0, sf::Color::Green, 10.0f);
    snakeVector.push_back(snake1);

    //if 2 player spawn 2 snakes
    if (m_players == 2)
    {
        Snake* snake2 = new Snake(GetRandomPos(), 1, sf::Color::Red, 10.0f);
        snakeVector.push_back(snake2);
    }

    //battery
    m_battery.ChangePosition(GetRandomPos());
    //spawn collectable
    for (int i = 0; i < 5; i++)
    {
        m_collectable.emplace_back(new Collectable());
    }

    //Clock
    sf::Clock clock;

    sf::Clock countdown;

    float timeOfLastLoop(clock.getElapsedTime().asSeconds());
    int TimesAroundLooop{ 0 };
    int fps{ 0 };

    sf::Text scoreText;

    // Main loop
    while (m_window->isOpen())
    {
        //FPS
        float fps = 1.0f / clock.getElapsedTime().asSeconds();
        sf::Text fpsText("FPS: " + to_string((int)fps), font, 20);
        fpsText.setPosition(sf::Vector2f(680, 25));
       
        // Handle any pending SFML events
        sf::Event event;
        while (m_window->pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                m_window->close();
                break;
            default:
                break;
            }
        }
        
        // We must clear the window each time around the loop
        m_window->clear(sf::Color(50,50,50,255));
        
        //draw fps on screen
        m_window->draw(fpsText);

        //Self Collision Check
        for (Snake* s : snakeVector)
        {
            s->CheckCollision(m_collectable);
            
            if (s->CheckCollision(m_battery))
                m_battery.ChangePosition(GetRandomPos());
            
            if (s->CheckSelfCollision())
            {
                s->Die();
                cout << "You walked into yourself...haha" << endl;
            }

            if ((s->GetPosition().x <= headSize) or (s->GetPosition().x >= kScreenWidth - headSize) or (s->GetPosition().y <= headSize) or (s->GetPosition().y >= kScreenHeight - headSize))
            {
                cout << "You got a good look at the wall" << endl;
                s->Die();
            }
                
        }

        //render snakes
        for (Snake* snake : snakeVector)
        {
            snake->Render(*m_window);
        }
        
        //Alive snake counter
        int numberOfAliveSnakes{ 0 };
        for (Snake* s : snakeVector)
        {
            if (s->GetAlive())
                numberOfAliveSnakes++;
        }
        if (numberOfAliveSnakes < 2)
        {
            break;
        }

        //spawn collectable on random pos
        if (rand() % 20 == 0)
        {
            for (Collectable* c : m_collectable)
            {
                if (c->GetVisible() == false)
                {
                    c->Spawn(GetRandomPos());
                    break;
                }
            }
        }

        for (Collectable* c : m_collectable)
        {
            c->Render(*m_window);
        }

        m_wall.Render(*m_window);

        m_battery.Render(*m_window);

        sf::Vector2f pos = sf::Vector2f(Snake::GetRadius() * 2 + 5, Snake::GetRadius() * 2);
        
        int count{ 0 };
        for (Snake* s : snakeVector)
        {
            scoreText.setPosition(pos);
            scoreText.setFont(font);
            scoreText.setCharacterSize(20);
            scoreText.setString("Snake" + to_string(count + 1) + ":" + to_string(s->GetScore()));

            m_window->draw(scoreText);

            pos.y += 30;

            count++;
        }

        //Timer
        scoreText.setCharacterSize(30);
        scoreText.setPosition(kScreenWidth / 2.5, Snake::GetRadius() * 2);
        scoreText.setString("TIME:" + to_string((int)(90 - countdown.getElapsedTime().asSeconds())));

        m_window->draw(scoreText);
        //Set Simulation Time
        if (clock.getElapsedTime().asMilliseconds() >= kTimeBetweenTicks)
        {
            //Update snakes
            for (Snake* s : snakeVector)
            {
                s->Update();
                if (HandleCollision() == true)
                {
                    s->Die();
                }
            }

            clock.restart();
        }

        // Graphics
        m_window->display();
    }

    for (size_t i = 0; i < snakeVector.size(); i++)
    {
        if (snakeVector[i]->GetAlive())
        {
            winner = i;
            highScore = snakeVector[i]->GetScore();
        }
        else if (((int)countdown.getElapsedTime().asSeconds() <= 0) and (snakeVector[i]->GetScore() > snakeVector[highScore]->GetScore()))
        {
            highScore = snakeVector[i]->GetScore();
        }
    }

    //cleanup
    for (Snake* s : snakeVector)
        delete s;
    snakeVector.clear();

    for (Collectable* c : m_collectable)
        delete c;
    m_collectable.clear();

    m_state = GameState::GameOver;
}

//Highscore Table
void Game::HighSchore()
{
    sf::Font font;

    if (!font.loadFromFile("Data/Fonts/menu_font.ttf"))
    {
        cout << "Error loading font" << endl;
        return;
    }
    //title
    sf::Text title("Highest Score", font, 50);
    title.setPosition(kScreenWidth / 2, 50);
    title.setOrigin(200, 25);
    string scores;

    //score
    ifstream savescore;
    savescore.open("Data/Save/save.txt", ios::in);
    if (savescore.is_open())
    {
        savescore >> scores;
        savescore.close();
    }
    sf::Text hiScoreText;
    hiScoreText.setFont(font);
    hiScoreText.setCharacterSize(20);
    hiScoreText.setColor(sf::Color::White);
    hiScoreText.setPosition(300, 300);
  
    stringstream s;

    s << "Highest Score: " << scores;
    hiScoreText.setString(s.str());

    //Exit button
    sf::RectangleShape backButton(sf::Vector2f(250, 50));
    backButton.setFillColor(sf::Color::Red);
    backButton.setPosition(160, 500);
    backButton.setOrigin(125, 25);

    //Exit text
    sf::Text backButtonText("Back", font, 30);
    backButtonText.setPosition(160, 510);
    backButtonText.setColor(sf::Color::White);
    backButtonText.setOrigin(50, 25);


    while (m_window->isOpen())
    {
        sf::Event event;
        while (m_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window->close();
            }
        }

        //button pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);
            sf::FloatRect backbounds = backButton.getGlobalBounds();

            if (backbounds.contains(mousePos.x, mousePos.y))
            {
                m_window->clear();
                m_state = GameState::Waiting;
                return;
            }
        }

        m_window->clear();

        //title
        m_window->draw(title);

        m_window->draw(hiScoreText);
        //back button
        m_window->draw(backButton);
        m_window->draw(backButtonText);

        //display
        m_window->display();
    }
}

//Settings
void Game::SettingsScreen()
{
    //load font
    sf::Font font;
    sf::Font plus_fnt;

    if (!font.loadFromFile("Data/Fonts/menu_font.ttf"))
    {
        cout << "Error loading font" << endl;
        return;
    }
    if (!plus_fnt.loadFromFile("Data/Fonts/+_font.ttf"))
    {
        cout << "Error loading font" << endl;
        return;
    }

    //title
    sf::Text title("Settings", font, 50);
    title.setPosition(kScreenWidth/2, 50);
    title.setOrigin(125, 25);

    //Player text
    sf::Text playerButtonText("Players: " + to_string(m_players), font, 30);
    playerButtonText.setPosition(kScreenWidth / 2.25f, 210);
    playerButtonText.setOrigin(50, 25);
    playerButtonText.setColor(sf::Color::White);

    //minusbutton
    sf::RectangleShape minusbutton(sf::Vector2f(50, 50));
    minusbutton.setFillColor(sf::Color::Red);
    minusbutton.setPosition(160, 200);
    minusbutton.setOrigin(25, 25);

    //minus text
    sf::Text minusButtonText("-", plus_fnt, 30);
    minusButtonText.setPosition(170, 205);
    minusButtonText.setOrigin(25, 25);
    minusButtonText.setColor(sf::Color::White);

    //plusbutton
    sf::RectangleShape plusbutton(sf::Vector2f(50, 50));
    plusbutton.setFillColor(sf::Color::Green);
    plusbutton.setPosition(640, 200);
    plusbutton.setOrigin(25, 25);

    //plus text 
    sf::Text plusButtonText("+", plus_fnt, 30);
    plusButtonText.setPosition(650, 205);
    plusButtonText.setOrigin(25, 25);
    plusButtonText.setColor(sf::Color::White);


    //AI text
    sf::Text AIButtonText("AI Snake: " + to_string(m_AI), font, 30);
    AIButtonText.setPosition(kScreenWidth / 2, 300);
    AIButtonText.setOrigin(110, 25);
    AIButtonText.setColor(sf::Color::White);

    //minusbutton
    sf::RectangleShape AIminusbutton(sf::Vector2f(50, 50));
    AIminusbutton.setFillColor(sf::Color::Red);
    AIminusbutton.setPosition(160, 300);
    AIminusbutton.setOrigin(25, 25);

    //minus text
    sf::Text AIminusButtonText("-", plus_fnt, 30);
    AIminusButtonText.setPosition(170, 305);
    AIminusButtonText.setOrigin(25, 25);
    AIminusButtonText.setColor(sf::Color::White);

    //plusbutton
    sf::RectangleShape AIplusbutton(sf::Vector2f(50, 50));
    AIplusbutton.setFillColor(sf::Color::Green);
    AIplusbutton.setPosition(640, 300);
    AIplusbutton.setOrigin(25, 25);

    //plus text 
    sf::Text AIplusButtonText("+", plus_fnt, 30);
    AIplusButtonText.setPosition(650, 305);
    AIplusButtonText.setOrigin(25, 25);
    AIplusButtonText.setColor(sf::Color::White);

    //start button
    sf::RectangleShape startButton(sf::Vector2f(200, 50));
    startButton.setFillColor(sf::Color::Green);
    startButton.setPosition(300, 400);

    //start text
    sf::Text startButtonText("Start", font, 30);
    startButtonText.setPosition(350, 410);
    startButtonText.setColor(sf::Color::White);

    //main loop
    while (m_window->isOpen())
    {
        sf::Event event;
        while (m_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window->close();
            }
        }

        //Take player count
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            int t = 0;
            t + 1;
        }

        //check if button was pressed
        if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);
            
            //Player
            sf::FloatRect playerplusbounds = plusbutton.getGlobalBounds();
            sf::FloatRect playerminusbounds = minusbutton.getGlobalBounds();

            //AI
            sf::FloatRect AIplusbounds = AIplusbutton.getGlobalBounds();
            sf::FloatRect AIminusbounds = AIminusbutton.getGlobalBounds();

            //play button
            sf::FloatRect playbounds = startButton.getGlobalBounds();

            if (playerminusbounds.contains(mousePos.x, mousePos.y) and (m_players != 1))
            {
                m_window->clear();
                m_players -= 1;
                m_AI = 1;
                return;
            }
            else if (playerplusbounds.contains(mousePos.x, mousePos.y) and (m_players != 2))
            {
                m_window->clear();
                m_players += 1;
                return;
            }
            else if (AIplusbounds.contains(mousePos.x, mousePos.y) and (m_AI != 4))
            {
                m_window->clear();
                m_AI += 1;
                return;
            }
            else if (AIminusbounds.contains(mousePos.x, mousePos.y) and (m_AI != 0))
            {
                m_window->clear();
                m_AI -= 1;
                return;
            }
            else if (playbounds.contains(mousePos.x, mousePos.y))
            {
                m_window->clear();
                m_state = GameState::Playing;
                return;
            }
        }

        m_window->clear();
        
        //draw title
        m_window->draw(title);

        //draw player buttons
        m_window->draw(playerButtonText);
        m_window->draw(minusbutton);
        m_window->draw(minusButtonText);
        m_window->draw(plusbutton);
        m_window->draw(plusButtonText);

        //draw AI buttons
        m_window->draw(AIButtonText);
        m_window->draw(AIminusbutton);
        m_window->draw(AIminusButtonText);
        m_window->draw(AIplusbutton);
        m_window->draw(AIplusButtonText);


        //draw start button
        m_window->draw(startButton);
        m_window->draw(startButtonText);

        //Graphics
        m_window->display();
    }
}

//EndGame
void Game::EndScreen()
{
    m_window->clear();


    sf::Font font;
    if (!font.loadFromFile("Data/Fonts/menu_font.ttf"))
    {
        cout << "Error loading font" << endl;
        return;
    }

    fstream savescore;
    savescore.open("Data/Save/save.txt", ios::out);


    if (savescore.is_open())
    {
        savescore << highScore << "\n";
        
        savescore.close();
    }
    else cout << "Problem with opening file";


    //title
    sf::Text title("Game Over", font, 50);
    title.setOrigin(130,25);
    title.setPosition(kScreenWidth / 2, 50);
    
    //winner
    sf::Text winnerText("Winner is Snake " + to_string(winner+1), font, 30);
    winnerText.setPosition(kScreenWidth / 2, 200);
    winnerText.setOrigin(150, 25);

    //Score
    sf::Text scoreText("Score: " + to_string(highScore), font, 30);
    scoreText.setPosition(winnerText.getPosition().x, winnerText.getPosition().y+100);
    scoreText.setOrigin(150, 25);
   
    //play again button
    sf::RectangleShape playAgainButton(sf::Vector2f(250, 50));
    playAgainButton.setFillColor(sf::Color::Green);
    playAgainButton.setPosition(640, 500);
    playAgainButton.setOrigin(125, 25);

    //play again text
    sf::Text playAgainButtonText("Play Again", font, 30);
    playAgainButtonText.setPosition(playAgainButton.getPosition().x, playAgainButton.getPosition().y);
    playAgainButtonText.setColor(sf::Color::White);
    playAgainButtonText.setOrigin(100, 15);

    //Exit button
    sf::RectangleShape exitButton(sf::Vector2f(250, 50));
    exitButton.setFillColor(sf::Color::Red);
    exitButton.setPosition(160, 500);
    exitButton.setOrigin(125, 25);

    //Exit text
    sf::Text exitButtonText("Exit", font, 30);
    exitButtonText.setPosition(160, 510);
    exitButtonText.setColor(sf::Color::White);
    exitButtonText.setOrigin(50, 25);

    //button pressed
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);
        sf::FloatRect playAgainbounds = playAgainButton.getGlobalBounds();
        sf::FloatRect exitbounds = exitButton.getGlobalBounds();

        if (playAgainbounds.contains(mousePos.x, mousePos.y))
        {
            m_window->clear();
            m_state = GameState::Settings;
            return;
        }
        else if (exitbounds.contains(mousePos.x, mousePos.y))
        {
            m_window->clear();
            m_state = GameState::Exit;
            return;
        }
    }
    
    m_window->draw(title);
    m_window->draw(winnerText);
    m_window->draw(playAgainButton);
    m_window->draw(playAgainButtonText);
    m_window->draw(exitButton);
    m_window->draw(exitButtonText);
    m_window->draw(scoreText);
    m_window->display();
}

//Exit function
void Game::Exit()
{
    exit(0);
}   

void Game::Run()
{
    std::cout << "SnakeGame: Starting" << std::endl;

    //randomize
    srand((unsigned)time(0));
    
    sf::Music music;

    if (!music.openFromFile("Data/Music/retro_music.ogg"))
    {
        cout << "Error loading music" << endl;
        return;
    }
    music.play();
    music.setLoop(true);
    music.setVolume(1.5f);

    m_window = new sf::RenderWindow(sf::VideoMode(kScreenWidth, kScreenHeight), "C++ Snake ICA : U2460536");

    bool ShouldExit{ false };
    while (!ShouldExit)
    {
        switch (m_state)
        {
        case GameState::Waiting:
            FrontEnd();
            break;
        case GameState::Settings:
            SettingsScreen();
            break;
        case GameState::Playing:
            InGameScreen();
            break;
        case GameState::GameOver:
        {
            EndScreen();
            break;
        }
        case GameState::HighScore:
            HighSchore();
            break;
        case GameState::Exit:
            Exit();
        default:
            FrontEnd();
        }
    }

    std::cout << "SnakeGame: Finished" << std::endl;
};