#include "Game.h"

#include "Constants.h"

#include <string>

namespace Arkanoid
{
    Game::Game()
        : window(
            sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT),
            "Arkanoid"
        ),
        currentScreen(GameScreen::Menu)
    {
        window.setFramerateLimit(60);

        font.loadFromFile("Resources/Fonts/Roboto-Light.ttf");
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
            {
                window.close();
            }

            if (event.type != sf::Event::KeyPressed)
            {
                if (event.type == sf::Event::MouseMoved &&
                    currentScreen == GameScreen::Playing)
                {
                    gameState.HandleMouse(
                        static_cast<float>(event.mouseMove.x)
                    );
                }

                continue;
            }

            if (event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }

            if (currentScreen == GameScreen::Menu)
            {
                if (event.key.code == sf::Keyboard::Enter)
                    StartGame();
            }
            else if (currentScreen == GameScreen::Win)
            {
                if (event.key.code == sf::Keyboard::Y)
                    RestartGame();

                if (event.key.code == sf::Keyboard::N)
                    currentScreen = GameScreen::Menu;
            }
            else if (currentScreen == GameScreen::GameOver)
            {
                if (event.key.code == sf::Keyboard::Enter)
                    RestartGame();

                if (event.key.code == sf::Keyboard::Escape)
                    currentScreen = GameScreen::Menu;
            }
        }
    }

    void Game::Update(float deltaTime)
    {
        if (currentScreen != GameScreen::Playing)
            return;

        gameState.Update(deltaTime);

        if (gameState.IsWin())
            currentScreen = GameScreen::Win;
        else if (gameState.IsGameOver())
            currentScreen = GameScreen::GameOver;
    }

    void Game::Draw()
    {
        window.clear(sf::Color(20, 20, 35));

        if (currentScreen == GameScreen::Menu)
            DrawMenu();
        else
            DrawGame();

        window.display();
    }

    void Game::DrawMenu()
    {
        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::White);
        title.setString("ARKANOID");

        sf::FloatRect titleBounds = title.getLocalBounds();

        title.setPosition(
            (SCREEN_WIDTH - titleBounds.width) / 2.f,
            180.f
        );

        window.draw(title);

        sf::Text start;
        start.setFont(font);
        start.setCharacterSize(26);
        start.setFillColor(sf::Color::White);
        start.setString("ENTER - Start");

        sf::FloatRect startBounds = start.getLocalBounds();

        start.setPosition(
            (SCREEN_WIDTH - startBounds.width) / 2.f,
            300.f
        );

        window.draw(start);

        sf::Text exit;
        exit.setFont(font);
        exit.setCharacterSize(22);
        exit.setFillColor(sf::Color::White);
        exit.setString("ESC - Exit");

        sf::FloatRect exitBounds = exit.getLocalBounds();

        exit.setPosition(
            (SCREEN_WIDTH - exitBounds.width) / 2.f,
            350.f
        );

        window.draw(exit);
    }

    void Game::DrawGame()
    {
        gameState.Draw(window);
        DrawInterface();

        if (currentScreen == GameScreen::Win)
            DrawWin();

        if (currentScreen == GameScreen::GameOver)
            DrawGameOver();
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

    void Game::DrawWin()
    {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));

        window.draw(overlay);

        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(45);
        title.setFillColor(sf::Color::Green);
        title.setString("YOU WIN!");

        sf::FloatRect titleBounds = title.getLocalBounds();

        title.setPosition(
            (SCREEN_WIDTH - titleBounds.width) / 2.f,
            190.f
        );

        window.draw(title);

        sf::Text question;
        question.setFont(font);
        question.setCharacterSize(25);
        question.setFillColor(sf::Color::White);
        question.setString("Play again?");

        sf::FloatRect questionBounds = question.getLocalBounds();

        question.setPosition(
            (SCREEN_WIDTH - questionBounds.width) / 2.f,
            280.f
        );

        window.draw(question);

        sf::Text options;
        options.setFont(font);
        options.setCharacterSize(22);
        options.setFillColor(sf::Color::White);
        options.setString("Y - Yes     N - No");

        sf::FloatRect optionsBounds = options.getLocalBounds();

        options.setPosition(
            (SCREEN_WIDTH - optionsBounds.width) / 2.f,
            330.f
        );

        window.draw(options);
    }

    void Game::DrawGameOver()
    {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));

        window.draw(overlay);

        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(45);
        title.setFillColor(sf::Color::Red);
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

    void Game::StartGame()
    {
        gameState.Reset();
        currentScreen = GameScreen::Playing;
    }

    void Game::RestartGame()
    {
        gameState.Reset();
        currentScreen = GameScreen::Playing;
    }
}