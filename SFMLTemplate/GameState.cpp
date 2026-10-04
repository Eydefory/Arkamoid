#include "GameState.h"

#include "Bonus.h"
#include "BonusCommand.h"
#include "Constants.h"
#include "DurableBrick.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>

namespace Arkanoid
{
    GameState::~GameState() = default;

    GameState::GameState()
        : scoreStrategy(std::make_unique<StandardScoreStrategy>()),
        score(0),
        lives(START_LIVES),
        gameOver(false),
        win(false),
        fireballTimer(0.f),
        shrinkTimer(0.f),
        speedTimer(0.f)
    {
        CreateBricks();
    }

    void GameState::Reset()
    {
        score = 0;
        lives = START_LIVES;

        gameOver = false;
        win = false;

        fireballTimer = 0.f;
        shrinkTimer = 0.f;
        speedTimer = 0.f;

        platform.Reset();
        ball.Reset();

        bonuses.clear();

        CreateBricks();
    }

    void GameState::CreateBricks()
    {
        bricks.clear();

        for (int row = 0; row < BLOCK_ROWS; ++row)
        {
            for (int column = 0; column < BLOCK_COLUMNS; ++column)
            {
                float x = BLOCK_START_X +
                    column * (BLOCK_WIDTH + BLOCK_GAP);

                float y = BLOCK_START_Y +
                    row * (BLOCK_HEIGHT + BLOCK_GAP);

                if ((row + column) % 3 == 0)
                {
                    bricks.push_back(
                        std::make_unique<DurableBrick>(x, y)
                    );
                }
                else
                {
                    bricks.push_back(
                        std::make_unique<Brick>(x, y)
                    );
                }
            }
        }
    }

    void GameState::Update(float deltaTime)
    {
        if (gameOver || win)
            return;

        UpdateBonusEffects(deltaTime);

        platform.Update(deltaTime);
        ball.Update(deltaTime);

        ball.CheckPlatformCollision(
            platform.GetBounds()
        );

        CheckBrickCollisions();
        CheckBonuses();

        for (auto& bonus : bonuses)
            bonus->Update(deltaTime);

        bonuses.erase(
            std::remove_if(
                bonuses.begin(),
                bonuses.end(),
                [](const std::unique_ptr<Bonus>& bonus)
                {
                    return bonus->IsCollected() ||
                        bonus->IsOutOfScreen();
                }
            ),
            bonuses.end()
        );

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

            sf::FloatRect ballBounds = ball.GetBounds();
            sf::FloatRect brickBounds = brick->GetBounds();

            if (!brick->CheckCollision(ballBounds))
                continue;

            if (fireballTimer <= 0.f)
            {
                float ballLeft = ballBounds.left;
                float ballRight =
                    ballBounds.left + ballBounds.width;

                float ballTop = ballBounds.top;
                float ballBottom =
                    ballBounds.top + ballBounds.height;

                float brickLeft = brickBounds.left;
                float brickRight =
                    brickBounds.left + brickBounds.width;

                float brickTop = brickBounds.top;
                float brickBottom =
                    brickBounds.top + brickBounds.height;

                float overlapX =
                    std::min(ballRight, brickRight) -
                    std::max(ballLeft, brickLeft);

                float overlapY =
                    std::min(ballBottom, brickBottom) -
                    std::max(ballTop, brickTop);

                if (overlapX < overlapY)
                    ball.BounceHorizontal();
                else
                    ball.BounceVertical();
            }
            else
            {
                brick->Destroy();
            }

            if (brick->IsDestroyed())
            {
                score += scoreStrategy->GetPoints(*brick);

                float chance =
                    static_cast<float>(std::rand()) /
                    static_cast<float>(RAND_MAX);

                if (chance < BONUS_DROP_CHANCE)
                {
                    SpawnBonus(
                        brickBounds.left +
                        brickBounds.width / 2.f -
                        BONUS_SIZE / 2.f,
                        brickBounds.top
                    );
                }
            }

            break;
        }
    }

    void GameState::CheckBonuses()
    {
        sf::FloatRect platformBounds =
            platform.GetBounds();

        for (auto& bonus : bonuses)
        {
            if (!bonus->IsCollected() &&
                bonus->GetBounds().intersects(platformBounds))
            {
                bonus->Collect(*this);
            }
        }
    }

    void GameState::SpawnBonus(float x, float y)
    {
        int type = std::rand() % 3;

        if (type == 0)
        {
            bonuses.push_back(
                std::make_unique<Bonus>(
                    x,
                    y,
                    BonusType::Fireball,
                    std::make_unique<FireballCommand>()
                )
            );
        }
        else if (type == 1)
        {
            bonuses.push_back(
                std::make_unique<Bonus>(
                    x,
                    y,
                    BonusType::Shrink,
                    std::make_unique<ShrinkCommand>()
                )
            );
        }
        else
        {
            bonuses.push_back(
                std::make_unique<Bonus>(
                    x,
                    y,
                    BonusType::Speed,
                    std::make_unique<SpeedCommand>()
                )
            );
        }
    }

    void GameState::ApplyFireballBonus()
    {
        fireballTimer = BONUS_DURATION;

        if (speedTimer > 0.f)
        {
            ball.SetSpeedMultiplier(
                SPEED_BONUS_MULTIPLIER
            );
        }
        else
        {
            ball.SetSpeedMultiplier(
                FIREBALL_SPEED_MULTIPLIER
            );
        }
    }

    void GameState::ApplyShrinkBonus()
    {
        shrinkTimer = BONUS_DURATION;

        platform.SetTemporaryWidth(
            SHRINK_PLATFORM_WIDTH
        );
    }

    void GameState::ApplySpeedBonus()
    {
        speedTimer = BONUS_DURATION;

        ball.SetSpeedMultiplier(
            SPEED_BONUS_MULTIPLIER
        );
    }

    void GameState::UpdateBonusEffects(float deltaTime)
    {
        if (fireballTimer > 0.f)
        {
            fireballTimer -= deltaTime;

            if (fireballTimer <= 0.f)
                fireballTimer = 0.f;
        }

        if (shrinkTimer > 0.f)
        {
            shrinkTimer -= deltaTime;

            if (shrinkTimer <= 0.f)
            {
                shrinkTimer = 0.f;
                platform.ResetWidth();
            }
        }

        if (speedTimer > 0.f)
        {
            speedTimer -= deltaTime;

            if (speedTimer <= 0.f)
                speedTimer = 0.f;
        }

        if (speedTimer > 0.f)
        {
            ball.SetSpeedMultiplier(
                SPEED_BONUS_MULTIPLIER
            );
        }
        else if (fireballTimer > 0.f)
        {
            ball.SetSpeedMultiplier(
                FIREBALL_SPEED_MULTIPLIER
            );
        }
        else
        {
            ball.ResetSpeed();
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

    GameMemento GameState::Save() const
    {
        std::vector<bool> destroyedBricks;

        for (const auto& brick : bricks)
        {
            destroyedBricks.push_back(
                brick->IsDestroyed()
            );
        }

        return GameMemento(
            score,
            lives,
            ball.GetPosition(),
            platform.GetPositionX(),
            platform.GetWidth(),
            destroyedBricks
        );
    }

    void GameState::Load(const GameMemento& memento)
    {
        score = memento.GetScore();
        lives = memento.GetLives();

        gameOver = false;
        win = false;

        fireballTimer = 0.f;
        shrinkTimer = 0.f;
        speedTimer = 0.f;

        
        CreateBricks();

        ball.SetPosition(
            memento.GetBallPosition()
        );

        platform.Reset();

        platform.SetTemporaryWidth(
            memento.GetPlatformWidth()
        );

        platform.SetPositionX(
            memento.GetPlatformX()
        );

        ball.ResetSpeed();

        const std::vector<bool>& destroyedBricks =
            memento.GetDestroyedBricks();

        for (std::size_t i = 0;
            i < bricks.size();
            ++i)
        {
            if (i < destroyedBricks.size() &&
                destroyedBricks[i])
            {
                bricks[i]->Destroy();
            }
        }

        bonuses.clear();

        gameOver = lives <= 0;
        win = AllBricksDestroyed();
    }

    bool GameState::SaveToFile(
        const std::string& fileName
    ) const
    {
        GameMemento memento = Save();

        std::ofstream file(fileName);

        if (!file)
            return false;

        sf::Vector2f ballPosition =
            memento.GetBallPosition();

        file << memento.GetScore() << '\n';
        file << memento.GetLives() << '\n';

        file << ballPosition.x << ' '
            << ballPosition.y << '\n';

        file << memento.GetPlatformX() << ' '
            << memento.GetPlatformWidth() << '\n';

        const std::vector<bool>& destroyedBricks =
            memento.GetDestroyedBricks();

        file << destroyedBricks.size() << '\n';

        for (bool destroyed : destroyedBricks)
            file << (destroyed ? 1 : 0) << ' ';

        file << '\n';

        return true;
    }

    bool GameState::LoadFromFile(
        const std::string& fileName
    )
    {
        std::ifstream file(fileName);

        if (!file)
            return false;

        int savedScore;
        int savedLives;

        sf::Vector2f savedBallPosition;

        float savedPlatformX;
        float savedPlatformWidth;

        std::size_t brickCount;

        if (!(file >> savedScore >> savedLives))
            return false;

        if (!(file >>
            savedBallPosition.x >>
            savedBallPosition.y))
        {
            return false;
        }

        if (!(file >>
            savedPlatformX >>
            savedPlatformWidth))
        {
            return false;
        }

        if (!(file >> brickCount))
            return false;

        std::vector<bool> destroyedBricks(
            brickCount,
            false
        );

        for (std::size_t i = 0;
            i < brickCount;
            ++i)
        {
            int value;

            if (!(file >> value))
                return false;

            destroyedBricks[i] = value != 0;
        }

        GameMemento memento(
            savedScore,
            savedLives,
            savedBallPosition,
            savedPlatformX,
            savedPlatformWidth,
            destroyedBricks
        );

        Load(memento);

        return true;
    }

    void GameState::Draw(
        sf::RenderWindow& window
    ) const
    {
        for (const auto& brick : bricks)
            brick->Draw(window);

        for (const auto& bonus : bonuses)
            bonus->Draw(window);

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