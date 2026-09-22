#include "Game.h"

#include "Constants.h"

#include <string>

namespace Arkanoid
{
    Game::Game()
        : window(
            sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT),
            "Arkanoid"
        )
    {
        window.setFramerateLimit(60);

        font.loadFromFile(
            "Resources/Fonts/Roboto-Light.ttf"
        );
    }

    void Game::Run()
    {
        sf::Clock clock;

        while (window.isOpen())
        {
            float deltaTime = clock.restart().asSeconds();

            HandleEvents();
            Update(deltaTime);
            Draw();
        }
    }

    void Game::HandleEvents()
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();

                if ((gameState.IsGameOver() || gameState.IsWin()) &&
                    event.key.code == sf::Keyboard::Enter)
                {
                    Restart();
                }
            }

            if (event.type == sf::Event::MouseMoved)
            {
                gameState.HandleMouse(
                    static_cast<float>(event.mouseMove.x)
                );
            }
        }
    }

    void Game::Update(float deltaTime)
    {
        gameState.Update(deltaTime);
    }

    void Game::Draw()
    {
        window.clear(sf::Color(20, 20, 35));

        gameState.Draw(window);
        DrawInterface();

        if (gameState.IsGameOver())
            DrawGameOver();

        if (gameState.IsWin())
            DrawWin();

        window.display();
    }

    void Game::DrawInterface()
    {
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(22);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString(
            "Score: " + std::to_string(gameState.GetScore())
        );
        scoreText.setPosition(20.f, 15.f);

        window.draw(scoreText);

        sf::Text livesText;
        livesText.setFont(font);
        livesText.setCharacterSize(22);
        livesText.setFillColor(sf::Color::White);
        livesText.setString(
            "Lives: " + std::to_string(gameState.GetLives())
        );
        livesText.setPosition(680.f, 15.f);

        window.draw(livesText);
    }

    void Game::DrawGameOver()
    {
        sf::RectangleShape overlay;
        overlay.setSize(
            sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT)
        );
        overlay.setFillColor(sf::Color(0, 0, 0, 170));

        window.draw(overlay);

        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::White);
        title.setString("GAME OVER");

        sf::FloatRect titleBounds = title.getLocalBounds();

        title.setPosition(
            (SCREEN_WIDTH - titleBounds.width) / 2.f,
            220.f
        );

        window.draw(title);

        sf::Text restart;
        restart.setFont(font);
        restart.setCharacterSize(24);
        restart.setFillColor(sf::Color::White);
        restart.setString("ENTER - Restart");

        sf::FloatRect restartBounds = restart.getLocalBounds();

        restart.setPosition(
            (SCREEN_WIDTH - restartBounds.width) / 2.f,
            310.f
        );

        window.draw(restart);
    }

    void Game::DrawWin()
    {
        sf::RectangleShape overlay;
        overlay.setSize(
            sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT)
        );
        overlay.setFillColor(sf::Color(0, 0, 0, 150));

        window.draw(overlay);

        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::Green);
        title.setString("YOU WIN!");

        sf::FloatRect titleBounds = title.getLocalBounds();

        title.setPosition(
            (SCREEN_WIDTH - titleBounds.width) / 2.f,
            220.f
        );

        window.draw(title);

        sf::Text restart;
        restart.setFont(font);
        restart.setCharacterSize(24);
        restart.setFillColor(sf::Color::White);
        restart.setString("ENTER - Restart");

        sf::FloatRect restartBounds = restart.getLocalBounds();

        restart.setPosition(
            (SCREEN_WIDTH - restartBounds.width) / 2.f,
            310.f
        );

        window.draw(restart);
    }

    void Game::Restart()
    {
        gameState.Reset();
    }
}