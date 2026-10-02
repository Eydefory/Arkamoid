#pragma once

#include <SFML/Graphics.hpp>

#include "GameObject.h"

namespace Arkanoid
{
    class Brick : public GameObject
    {
    public:
        Brick(float x, float y);

        void Update(float deltaTime) override;
        void Draw(sf::RenderWindow& window) const override;
        sf::FloatRect GetBounds() const override;

        bool CheckCollision(const sf::FloatRect& ballBounds);

        bool IsDestroyed() const;

    protected:
        virtual void OnHit();

        sf::RectangleShape brickShape;
        bool destroyed;
    };
}