#pragma once

#include <SFML/System/Vector2.hpp>

namespace Arkanoid
{
    float Length(const sf::Vector2f& vector);
    sf::Vector2f Normalize(const sf::Vector2f& vector);
}