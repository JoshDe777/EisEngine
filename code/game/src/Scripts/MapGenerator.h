#pragma once

#include <EisEngine.h>
#include <FastNoiseLite.h>

#include "../Enums.h"

#include "../MapGeneration/SeaNodeGraph.h"
#include "../GameObjects/MapTile.h"

namespace Ocean_Outlaws::scripts {
    using namespace map;
    class MapGenerator : public Script {
    public:
        MapGenerator(Game &engine, guid_t owner);
        SeaNodeGraph seaNodes;
        SeaNode* FindClosestNode(const Vector2& coordinates);

        void GenerateNewMap(const int& seed = 0);
        void ClearMap();

        int mapSize = 1;
    protected:
        void Invalidate() override;
    private:
        static const std::unordered_map<std::string, fs::path> waterTileDictionary;
        //region lists
        std::vector<std::vector<TileTypes>> tileMap;
        std::vector<std::vector<MapTile>> tileGrid = {};
        std::vector<Vector2> waterNodeCoordinates = {};
        std::vector<DebugLine> boundaries = {};
        //endregion

        //region generation variables:
        float xOffset = 0.0f;
        float yOffset = 0.0f;
        float mapScale = 0.0009f;
        //endregion

        //region perlin noise
        float perlinModifier = 0;
        /// \n The scale at which the noise should be scaled.
        float perlinLandThreshold = 0.67f;
        /// \n The seed for the noise object. Leave at 0 for a random seed generated before every game.
        int perlinSeed = 0;
        /// \n The perlin noise object.
        FastNoiseLite noise;
        GameObject* tileParent = nullptr;
        //endregion

        Game& engine;

        static int nodeCounter;

        //region functions
        // sub-region init functions
        void Init();
        void GenerateMap();
        void DrawBoundaryLines();
        // sub-region terrain gen
        void GenerateTerrain();
        TileTypes DetermineTerrain(const int& x, const int& y);
        // sub-region tile creation
        void PlaceTiles();
        MapTile CreateTile(const int& x, const int& y, const fs::path& path);
        std::string EvaluateTileNeighbors(const int& x, const int& y);
        // sub-region sea node graph
        void CreateGraph();
        //endregion
    };
}

using MapGenerator = Ocean_Outlaws::scripts::MapGenerator;
