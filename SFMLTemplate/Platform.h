#pragma once

#include <SFML/Graphics.hpp>

namespace Arkanoid
{
    class Platform
    {
    public:
        Platform();

        void Reset();
        void Update(float deltaTime);
        void MoveToMouse(float mouseX);
        void Draw(sf::RenderWindow& window) const;

        sf::FloatRect GetBounds() const;

    private:
        sf::RectangleShape shape;
        float speed;
        float startX;
        float startY;

        void ClampToScreen();
    };
}