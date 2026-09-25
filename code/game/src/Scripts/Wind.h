#pragma once

#include <FastNoiseLite.h>

#include "EisEngine.h"
#include "../Enums.h"
#include "../ui-elements/WindIndicator.h"

namespace Ocean_Outlaws::scripts {
    using namespace UI;
    /// \n The soft breeze in the world, giving a force into a given direction.
    class Wind : public Script {
    public:
        /// \n The soft breeze in the world, giving a force into a given direction.
        Wind(Game &engine, guid_t owner);
        /// \n Returns the 2D vector of the general wind direction.
        Vector2 GetWind();
        /// \n Returns the general wind direction.
        CardinalDirection GetWindDirection();
    protected:
        void Update() override;
    private:
        /// \n The force of the wind. As of now is constant, could later be made to vary somewhat.
        float windForce = 0.1f;
        /// \n The direction in which the wind will blow.
        CardinalDirection windDirection = EAST;
        //region noise
        /// \n The x position of the current perlin iteration.
        float perlinX = 0;
        /// \n The y position of the current perlin iteration.
        float perlinY = 0;
        /// \n The step size of the perlin iterations.
        float perlinStep = 0.01f;
        /// \n The seed for the noise object. Leave at 0 for a random seed generated before every game.
        int perlinSeed = 0;
        /// \n The perlin noise object.
        FastNoiseLite noise;
        //endregion

        /// \n Updates the displays related to wind.
        void UpdateDisplays();
        WindIndicator indicator;

        static CardinalDirection AngleToWindDirection(const float& angle);
        static const std::array<std::pair<CardinalDirection, float>, 16> directions;
    };
}

using Wind = Ocean_Outlaws::scripts::Wind;
