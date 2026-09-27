#include "DurableBrick.h"

#include "Constants.h"

namespace Arkanoid
{
    DurableBrick::DurableBrick(float x, float y)
        : Brick(x, y),
        hitPoints(3),
        damageColors{
            sf::Color(120, 180, 255),
            sf::Color(255, 190, 80),
            sf::Color(220, 80, 100)
        }
    {
        brickShape.setFillColor(damageColors[0]);
    }

    bool DurableBrick::CheckCollision(const sf::FloatRect& ballBounds)
    {
        if (destroyed)
            return false;

        if (!ballBounds.intersects(brickShape.getGlobalBounds()))
            return false;

        --hitPoints;

        if (hitPoints <= 0)
        {
            destroyed = true;
        }
        else
        {
            UpdateColor();
        }

        return true;
    }

    void DurableBrick::UpdateColor()
    {
        int colorIndex = 3 - hitPoints;

        if (colorIndex >= 0 && colorIndex < static_cast<int>(damageColors.size()))
            brickShape.setFillColor(damageColors[colorIndex]);
    }

    void DurableBrick::Draw(sf::RenderWindow& window) const
    {
        if (!destroyed)
            window.draw(brickShape);
    }
}