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
        CreateBlocks();
    }

    void GameState::Reset()
    {
        score = 0;
        lives = START_LIVES;
        gameOver = false;
        win = false;

        platform.Reset();
        ball.Reset();

        CreateBlocks();
    }

    void GameState::CreateBlocks()
    {
        blocks.clear();

        for (int row = 0; row < BLOCK_ROWS; ++row)
        {
            for (int column = 0; column < BLOCK_COLUMNS; ++column)
            {
                float x = BLOCK_START_X + column * (BLOCK_WIDTH + BLOCK_GAP);
                float y = BLOCK_START_Y + row * (BLOCK_HEIGHT + BLOCK_GAP);

                blocks.push_back(std::make_unique<Block>(x, y));
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
        CheckBlockCollisions();

        if (ball.IsOutOfScreen())
        {
            LoseLife();

            if (lives > 0)
            {
                ball.Reset();
                platform.Reset();
            }
        }

        if (AllBlocksDestroyed())
            win = true;
    }

    void GameState::HandleMouse(float mouseX)
    {
        if (gameOver || win)
            return;

        platform.MoveToMouse(mouseX);
    }

    void GameState::CheckBlockCollisions()
    {
        for (const auto& block : blocks)
        {
            if (block->IsDestroyed())
                continue;

            if (!block->CheckCollision(ball.GetBounds()))
                continue;

            sf::FloatRect blockBounds = block->GetBounds();
            sf::FloatRect ballBounds = ball.GetBounds();

            float ballCenterX = ballBounds.left + ballBounds.width / 2.f;
            float ballCenterY = ballBounds.top + ballBounds.height / 2.f;

            float blockCenterX = blockBounds.left + blockBounds.width / 2.f;
            float blockCenterY = blockBounds.top + blockBounds.height / 2.f;

            float differenceX = ballCenterX - blockCenterX;
            float differenceY = ballCenterY - blockCenterY;

            if (std::abs(differenceX) > std::abs(differenceY))
                ball.BounceHorizontal();
            else
                ball.BounceVertical();

            score += 10;
            break;
        }
    }

    bool GameState::AllBlocksDestroyed() const
    {
        for (const auto& block : blocks)
        {
            if (!block->IsDestroyed())
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
        for (const auto& block : blocks)
            block->Draw(window);

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