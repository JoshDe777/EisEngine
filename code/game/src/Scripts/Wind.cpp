#include "Wind.h"
#include "GameManager.h"

namespace Ocean_Outlaws::scripts {
    //region static const values
    const std::array<std::pair<CardinalDirection, float>, 16> Wind::directions = {
            std::make_pair(EAST, 0.0f),
            std::make_pair(EAST_NORTH_EAST, 22.5f),
            std::make_pair(NORTH_EAST, 45.0f),
            std::make_pair(NORTH_NORTH_EAST, 67.5f),
            std::make_pair(NORTH, 90.0f),
            std::make_pair(NORTH_NORTH_WEST, 112.5f),
            std::make_pair(NORTH_WEST, 135.0f),
            std::make_pair(WEST_NORTH_WEST, 157.5f),
            std::make_pair(WEST, 180.0f),
            std::make_pair(WEST_SOUTH_WEST, 202.5f),
            std::make_pair(SOUTH_WEST, 225.0f),
            std::make_pair(SOUTH_SOUTH_WEST, 247.5f),
            std::make_pair(SOUTH, 270.0f),
            std::make_pair(SOUTH_SOUTH_EAST, 292.5f),
            std::make_pair(SOUTH_EAST, 315.0f),
            std::make_pair(EAST_SOUTH_EAST, 337.5f)
    };
    //endregion

    Wind::Wind(EisEngine::Game &engine, EisEngine::ecs::guid_t owner) :
    Script(engine, owner),
    indicator(engine) {
        if(perlinSeed == 0)
            perlinSeed = RandomInt(0, std::numeric_limits<int>::max());
        noise = FastNoiseLite(perlinSeed);
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    }

    Vector2 Wind::GetWind() { return windForce * GameManager::cardinalVectors.at(windDirection);}

    CardinalDirection Wind::GetWindDirection() { return windDirection;}

    void Wind::Update() {
        // noise returns within range [-1, 1], so clamp to [0, 1] -> half value + 0.5 -> -1/2 + 0.5 = 0.
        float windPerlinValue = (noise.GetNoise(perlinX, perlinY) / 2.0f) + 0.5f;
        perlinX += perlinStep;

        auto newWindDirection = AngleToWindDirection(windPerlinValue * 360);
        if(newWindDirection == windDirection)
             return;

        windDirection = newWindDirection;
        UpdateDisplays();
    }

    CardinalDirection Wind::AngleToWindDirection(const float &angle) {
        CardinalDirection closestDirection = EAST;
        float minDifference = 360.0f;

        for (const auto& direction : directions) {
            float difference = Math::Dist(angle, direction.second);
            if (difference > 180.0f)
                difference = 360.0f - difference;

            if (difference < minDifference) {
                minDifference = difference;
                closestDirection = direction.first;
            }
        }

        return closestDirection;
    }

    void Wind::UpdateDisplays() {
        indicator.UpdateRotation(windDirection);
    }
}
