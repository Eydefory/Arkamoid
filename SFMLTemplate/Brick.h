#pragma once

#include <SFML/Graphics.hpp>

namespace Arkanoid
{
    class Brick
    {
    public:
        Brick(float x, float y);

        bool IsDestroyed() const;
        void Destroy();

        bool CheckCollision(const sf::FloatRect& ballBounds);

        void Draw(sf::RenderWindow& window) const;

        sf::FloatRect GetBounds() const;

    private:
        sf::RectangleShape shape;
        bool destroyed;
    };
}