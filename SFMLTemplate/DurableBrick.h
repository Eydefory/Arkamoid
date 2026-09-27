#pragma once

#include "Brick.h"

#include <vector>

namespace Arkanoid
{
    class DurableBrick : public Brick
    {
    public:
        DurableBrick(float x, float y);

        bool CheckCollision(const sf::FloatRect& ballBounds) override;
        void Draw(sf::RenderWindow& window) const override;

    private:
        int hitPoints;
        std::vector<sf::Color> damageColors;

        void UpdateColor();
    };
}