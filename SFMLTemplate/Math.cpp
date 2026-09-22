#include "Math.h"

#include <cmath>

namespace Arkanoid
{
    float Length(const sf::Vector2f& vector)
    {
        return std::sqrt(vector.x * vector.x + vector.y * vector.y);
    }

    sf::Vector2f Normalize(const sf::Vector2f& vector)
    {
        float length = Length(vector);

        if (length == 0.f)
            return sf::Vector2f(0.f, 0.f);

        return sf::Vector2f(vector.x / length, vector.y / length);
    }
}