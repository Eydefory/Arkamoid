#include "GameState.h"

#include "Constants.h"

#include <cmath>

namespace Arkanoid
{
    GameState::GameState()
        : score(0),
        lives(START_LIVES),
        gameOver(false),
        win(false)
    {
        CreateBricks();
    }

    void GameState::Reset()
    {
        score = 0;
        lives = START_LIVES;
        gameOver = false;
        win = false;

        platform.Reset();
        ball.Reset();

        CreateBricks();
    }

    void GameState::CreateBricks()
    {
        bricks.clear();

        for (int row = 0; row < BRICK_ROWS; ++row)
        {
            for (int column = 0; column < BRICK_COLUMNS; ++column)
            {
                float x = BRICKS_START_X + column * (BRICK_WIDTH + BRICK_GAP);
                float y = BRICKS_START_Y + row * (BRICK_HEIGHT + BRICK_GAP);

                bricks.push_back(
                    std::make_unique<Brick>(x, y)
                );
            }
        }
    }

    void GameState::Update(float deltaTime)
    {
        if (gameOver || win)
            return;

        platform.Update(deltaTime);
        ball.Update(deltaTime);

        ball.CheckPlatformCollision(platform.GetBounds());

        CheckBrickCollisions();

        if (ball.IsOutOfScreen())
        {
            LoseLife();

            if (lives > 0)
            {
                ball.Reset();
                platform.Reset();
            }
        }

        if (AllBricksDestroyed())
            win = true;
    }

    void GameState::HandleMouse(float mouseX)
    {
        if (gameOver || win)
            return;

        platform.MoveToMouse(mouseX);
    }

    void GameState::CheckBrickCollisions()
    {
        for (const auto& brick : bricks)
        {
            if (brick->IsDestroyed())
                continue;

            if (!brick->CheckCollision(ball.GetBounds()))
                continue;

            sf::FloatRect brickBounds = brick->GetBounds();
            sf::FloatRect ballBounds = ball.GetBounds();

            float ballCenterX = ballBounds.left + ballBounds.width / 2.f;
            float ballCenterY = ballBounds.top + ballBounds.height / 2.f;

            float brickCenterX = brickBounds.left + brickBounds.width / 2.f;
            float brickCenterY = brickBounds.top + brickBounds.height / 2.f;

            float differenceX = ballCenterX - brickCenterX;
            float differenceY = ballCenterY - brickCenterY;

            if (std::abs(differenceX) > std::abs(differenceY))
                ball.BounceHorizontal();
            else
                ball.BounceVertical();

            score += 10;

            break;
        }
    }

    bool GameState::AllBricksDestroyed() const
    {
        for (const auto& brick : bricks)
        {
            if (!brick->IsDestroyed())
                return false;
        }

        return true;
    }

    void GameState::LoseLife()
    {
        --lives;

        if (lives <= 0)
        {
            lives = 0;
            gameOver = true;
        }
    }

    void GameState::Draw(sf::RenderWindow& window) const
    {
        for (const auto& brick : bricks)
            brick->Draw(window);

        platform.Draw(window);
        ball.Draw(window);
    }

    bool GameState::IsGameOver() const
    {
        return gameOver;
    }

    bool GameState::IsWin() const
    {
        return win;
    }

    int GameState::GetScore() const
    {
        return score;
    }

    int GameState::GetLives() const
    {
        return lives;
    }
}