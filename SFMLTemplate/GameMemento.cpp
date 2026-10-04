#include "GameMemento.h"

namespace Arkanoid
{
    GameMemento::GameMemento(
        int scoreValue,
        int livesValue,
        const sf::Vector2f& ballPositionValue,
        float platformXValue,
        float platformWidthValue,
        const std::vector<bool>& destroyedBricksValue
    )
        : score(scoreValue),
        lives(livesValue),
        ballPosition(ballPositionValue),
        platformX(platformXValue),
        platformWidth(platformWidthValue),
        destroyedBricks(destroyedBricksValue)
    {
    }

    int GameMemento::GetScore() const
    {
        return score;
    }

    int GameMemento::GetLives() const
    {
        return lives;
    }

    sf::Vector2f GameMemento::GetBallPosition() const
    {
        return ballPosition;
    }

    float GameMemento::GetPlatformX() const
    {
        return platformX;
    }

    float GameMemento::GetPlatformWidth() const
    {
        return platformWidth;
    }

    const std::vector<bool>& GameMemento::GetDestroyedBricks() const
    {
        return destroyedBricks;
    }
}