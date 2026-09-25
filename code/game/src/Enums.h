#pragma once

namespace Ocean_Outlaws{
    enum ShipSide{
        STARBOARD,
        PORT
    };

    enum CardinalDirection{
        EAST = 0,
        EAST_NORTH_EAST = 23,
        NORTH_EAST = 45,
        NORTH_NORTH_EAST = 68,
        NORTH = 90,
        NORTH_NORTH_WEST = 113,
        NORTH_WEST = 135,
        WEST_NORTH_WEST = 158,
        WEST = 180,
        WEST_SOUTH_WEST = 203,
        SOUTH_WEST = 225,
        SOUTH_SOUTH_WEST = 248,
        SOUTH = 270,
        SOUTH_SOUTH_EAST = 293,
        SOUTH_EAST = 315,
        EAST_SOUTH_EAST = 338
    };

    enum TileTypes{
        WATER = 0,
        LAND = 1
    };

    inline std::string to_string(const TileTypes& type){
        switch (type) {
            case WATER:
                return "Water";
            case LAND:
                return "Land";
            default:
                return "undefined";
        }
    }

    inline std::string to_string(const CardinalDirection& dir){
        switch(dir){
            case EAST:
                return "East";
            case EAST_NORTH_EAST:
                return "East-North-East";
            case NORTH_EAST:
                return "North-East";
            case NORTH_NORTH_EAST:
                return "North-North-East";
            case NORTH:
                return "North";
            case NORTH_NORTH_WEST:
                return "North-North-West";
            case NORTH_WEST:
                return "North-West";
            case WEST_NORTH_WEST:
                return "West-North-West";
            case WEST:
                return "West";
            case WEST_SOUTH_WEST:
                return "West-South-West";
            case SOUTH_WEST:
                return "South-West";
            case SOUTH_SOUTH_WEST:
                return "South-South-West";
            case SOUTH:
                return "South";
            case SOUTH_SOUTH_EAST:
                return "South-South-East";
            case SOUTH_EAST:
                return "South-East";
            case EAST_SOUTH_EAST:
                return "East-South-East";
        }
    }

    enum ReferenceSizes {
        WIDTH = 3840,
        HEIGHT = 2108
    };

    inline float getRelativeWidth(const float& val, const float& screenWidth)
    { return (val * screenWidth) / (float) WIDTH;}

    inline float getRelativeHeight(const float& val, const float& screenHeight)
    { return (val * screenHeight) / (float) HEIGHT;}
}
