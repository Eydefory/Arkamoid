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
        sf::RenderWindow window;
        GameState gameState;
        sf::Font font;

        void HandleEvents();
        void Update(float deltaTime);
        void Draw();

        void DrawInterface();
        void DrawGameOver();
        void DrawWin();

        void Restart();
    };
}