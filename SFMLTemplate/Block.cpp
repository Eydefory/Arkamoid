#include "Block.h"

#include "Constants.h"

namespace Arkanoid
{
    Block::Block(float x, float y)
        : destroyed(false)
    {
        shape.setSize(sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color(220, 80, 100));
    }

    void Block::Update(float deltaTime)
    {
    }

    void Block::Draw(sf::RenderWindow& window) const
    {
        if (!destroyed)
            window.draw(shape);
    }

    sf::FloatRect Block::GetBounds() const
    {
        return shape.getGlobalBounds();
    }

    bool Block::IsDestroyed() const
    {
        return destroyed;
    }

    bool Block::CheckCollision(const sf::FloatRect& ballBounds)
    {
        if (destroyed)
            return false;

        if (!ballBounds.intersects(shape.getGlobalBounds()))
            return false;

        destroyed = true;
        return true;
    }
}