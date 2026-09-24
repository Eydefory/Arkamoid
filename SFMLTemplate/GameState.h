#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

#include "Platform.h"
#include "Ball.h"
#include "Block.h"

namespace Arkanoid
{
    class GameState
    {
    public:
        GameState();

        void Reset();
        void Update(float deltaTime);
        void HandleMouse(float mouseX);

        void Draw(sf::RenderWindow& window) const;

        bool IsGameOver() const;
        bool IsWin() const;

        int GetScore() const;
        int GetLives() const;

    private:
        Platform platform;
        Ball ball;
        std::vector<std::unique_ptr<Block>> blocks;

        int score;
        int lives;

        bool gameOver;
        bool win;

        void CreateBlocks();
        void CheckBlockCollisions();
        bool AllBlocksDestroyed() const;
        void LoseLife();
    };
}