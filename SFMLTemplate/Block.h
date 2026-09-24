#pragma once

#include <SFML/Graphics.hpp>

#include "GameObject.h"

namespace Arkanoid
{
    class Block : public GameObject
    {
    public:
        Block(float x, float y);

        void Update(float deltaTime) override;
        void Draw(sf::RenderWindow& window) const override;

        sf::FloatRect GetBounds() const override;

        bool IsDestroyed() const;
        bool CheckCollision(const sf::FloatRect& ballBounds);

    private:
        sf::RectangleShape shape;
        bool destroyed;
    };
}