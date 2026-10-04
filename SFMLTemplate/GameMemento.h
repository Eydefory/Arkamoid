#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

namespace Arkanoid
{
    class GameMemento
    {
    public:
        GameMemento(
            int score,
            int lives,
            const sf::Vector2f& ballPosition,
            float platformX,
            float platformWidth,
            const std::vector<bool>& destroyedBricks
        );

        int GetScore() const;
        int GetLives() const;

        sf::Vector2f GetBallPosition() const;

        float GetPlatformX() const;
        float GetPlatformWidth() const;

        const std::vector<bool>& GetDestroyedBricks() const;

    private:
        int score;
        int lives;

        sf::Vector2f ballPosition;

        float platformX;
        float platformWidth;

        std::vector<bool> destroyedBricks;
    };
}