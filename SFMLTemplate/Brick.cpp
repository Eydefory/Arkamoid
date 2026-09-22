#include "Brick.h"

#include "Constants.h"

namespace Arkanoid
{
    Brick::Brick(float x, float y)
        : destroyed(false)
    {
        shape.setSize(sf::Vector2f(BRICK_WIDTH, BRICK_HEIGHT));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color(220, 80, 100));
    }

    bool Brick::IsDestroyed() const
    {
        return destroyed;
    }

    void Brick::Destroy()
    {
        destroyed = true;
    }

    bool Brick::CheckCollision(const sf::FloatRect& ballBounds)
    {
        if (destroyed)
            return false;

        if (!ballBounds.intersects(shape.getGlobalBounds()))
            return false;

        destroyed = true;
        return true;
    }

    void Brick::Draw(sf::RenderWindow& window) const
    {
        if (!destroyed)
            window.draw(shape);
    }

    sf::FloatRect Brick::GetBounds() const
    {
        return shape.getGlobalBounds();
    }
}