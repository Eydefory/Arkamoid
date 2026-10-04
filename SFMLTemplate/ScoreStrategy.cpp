#include "ScoreStrategy.h"

#include "Brick.h"
#include "DurableBrick.h"

namespace Arkanoid
{
    int StandardScoreStrategy::GetPoints(
        const Brick& brick
    ) const
    {
        if (dynamic_cast<const DurableBrick*>(&brick) != nullptr)
            return 30;

        return 10;
    }
}