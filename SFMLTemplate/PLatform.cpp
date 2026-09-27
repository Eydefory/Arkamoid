#include "Platform.h"

#include "Constants.h"

namespace Arkanoid
{
    Platform::Platform()
        : speed(PLATFORM_SPEED),
        startX((SCREEN_WIDTH - PLATFORM_WIDTH) / 2.f),
        startY(SCREEN_HEIGHT - 50.f)
    {
        shape.setSize(sf::Vector2f(PLATFORM_WIDTH, PLATFORM_HEIGHT));
        shape.setPosition(startX, startY);
        shape.setFillColor(sf::Color(120, 100, 255));
    }

    void Platform::Reset()
    {
        shape.setPosition(startX, startY);
    }

    void Platform::Update(float deltaTime)
    {
        float movement = 0.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            movement -= speed * deltaTime;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            movement += speed * deltaTime;
        }

        shape.move(movement, 0.f);
        ClampToScreen();
    }

    void Platform::MoveToMouse(float mouseX)
    {
        shape.setPosition(
            mouseX - PLATFORM_WIDTH / 2.f,
            shape.getPosition().y
        );

        ClampToScreen();
    }

    void Platform::ClampToScreen()
    {
        sf::Vector2f position = shape.getPosition();

        if (position.x < 0.f)
            position.x = 0.f;

        if (position.x + PLATFORM_WIDTH > SCREEN_WIDTH)
            position.x = SCREEN_WIDTH - PLATFORM_WIDTH;

        shape.setPosition(position);
    }

    void Platform::Draw(sf::RenderWindow& window) const
    {
        window.draw(shape);
    }

    sf::FloatRect Platform::GetBounds() const
    {
        return shape.getGlobalBounds();
    }
}