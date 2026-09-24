#pragma once

#include <SFML/Graphics.hpp>

#include "GameState.h"

namespace Arkanoid
{
    class Game
    {
    public:
        Game();

        void Run();

    private:
        enum class GameScreen
        {
            Menu,
            Playing,
            Win,
            GameOver
        };

        sf::RenderWindow window;
        GameState gameState;
        sf::Font font;

        GameScreen currentScreen;

        void HandleEvents();
        void Update(float deltaTime);
        void Draw();

        void DrawMenu();
        void DrawGame();
        void DrawInterface();
        void DrawWin();
        void DrawGameOver();

        void StartGame();
        void RestartGame();
    };
}