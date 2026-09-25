#include "MapGenerator.h"
#include "GameManager.h"

namespace Ocean_Outlaws::scripts {
    int MapGenerator::nodeCounter = 0;

    //region dictionary definition
    const std::unordered_map<std::string, fs::path> MapGenerator::waterTileDictionary = {
            {"LLLLLLLL", "images/sprites/tiles/LLLLLLLL.png"},          // 1 - 8L
            {"LLLLLLWL", "images/sprites/tiles/LLLLLLWL.png"},          // 2 - 6L W L
            {"LLLLWLLL", "images/sprites/tiles/LLLLWLLL.png"},          // 3 - 4L W 3L
            {"LLLLWLWL", "images/sprites/tiles/LLLLWLWL.png"},          // 4 - 4L 2(WL)
            {"LLLLWWWL", "images/sprites/tiles/LLLLWWWL.png"},          // 5 - 4L 3W L
            {"LLWLLLLL", "images/sprites/tiles/LLWLLLLL.png"},          // 6 - 2L W 5L
            {"LLWLWLLL", "images/sprites/tiles/LLWLWLLL.png"},          // 7 - 2L 2(WL) 2L
            {"LLWLWLWL", "images/sprites/tiles/LLWLWLWL.png"},          // 8 - 2L 3(WL)
            {"LLWLWWWL", "images/sprites/tiles/LLWLWWWL.png"},          // 9 - 2L W L 3WL
            {"LLWWWLLL", "images/sprites/tiles/LLWWWLLL.png"},          // 10 - 2L 3W 3L
            {"LLWWWLWL", "images/sprites/tiles/LLWWWLWL.png"},          // 11 - 2L 3W L W L
            {"LLWWWWWL", "images/sprites/tiles/LLWWWWWL.png"},          // 12 - 2L 5W L
            {"WLLLLLLL", "images/sprites/tiles/WLLLLLLL.png"},          // 13 - W 7L
            {"WLLLLLWL", "images/sprites/tiles/WLLLLLWL.png"},          // 14 - W 5L W L
            {"WLLLLLWW", "images/sprites/tiles/WLLLLLWW.png"},          // 15 - W 5L 2W
            {"WLLLWLWL", "images/sprites/tiles/WLLLWLWL.png"},          // 16 - W 3L 2(WL)
            {"WLLLWLWW", "images/sprites/tiles/WLLLWLWW.png"},          // 17 - W 3L W L 2W
            {"WLLLWWWL", "images/sprites/tiles/WLLLWWWL.png"},          // 18 - W 3L 3W L
            {"WLLLWWWW", "images/sprites/tiles/WLLLWWWW.png"},          // 19 - W 3L 4W
            {"WLWLLLLL", "images/sprites/tiles/WLWLLLLL.png"},          // 20 - 2(WL) 4L
            {"WLWLLLWL", "images/sprites/tiles/WLWLLLWL.png"},          // 21 - 2(WL) 2L W L
            {"WLWLLLWW", "images/sprites/tiles/WLWLLLWW.png"},          // 22 - 2(WL) 2L 2W
            {"WLWLWLLL", "images/sprites/tiles/WLWLWLLL.png"},          // 23 - 3(WL) 2L
            {"WLWLWLWL", "images/sprites/tiles/WLWLWLWL.png"},          // 24 - 4(WL)
            {"WLWLWLWW", "images/sprites/tiles/WLWLWLWW.png"},          // 25 - 3(WL) 2W
            {"WLWLWWWL", "images/sprites/tiles/WLWLWWWL.png"},          // 26 - 2(WL) 3W L
            {"WLWLWWWW", "images/sprites/tiles/WLWLWWWW.png"},          // 27 - 2(WL) 4 W
            {"WLWWWLLL", "images/sprites/tiles/WLWWWLLL.png"},          // 28 - W L 3W 3L
            {"WLWWWLWL", "images/sprites/tiles/WLWWWLWL.png"},          // 29 - W L 3W L W L
            {"WLWWWLWW", "images/sprites/tiles/WLWWWLWW.png"},          // 30 - W L 3W L 2W
            {"WLWWWWWL", "images/sprites/tiles/WLWWWWWL.png"},          // 31 - W L 5W L
            {"WLWWWWWW", "images/sprites/tiles/WLWWWWWW.png"},          // 32 - W L 6W
            {"WWWLWWWW", "images/sprites/tiles/WWWLWWWW.png"},          // 33 - 3W L 4W
            {"WWWLLLLL", "images/sprites/tiles/WWWLLLLL.png"},          // 34 - 3W 5L
            {"WWWLLLWL", "images/sprites/tiles/WWWLLLWL.png"},          // 35 - 3W 3L W L
            {"WWWLLLWW", "images/sprites/tiles/WWWLLLWW.png"},          // 36 - 3W 3L 2W
            {"WWWLWLLL", "images/sprites/tiles/WWWLWLLL.png"},          // 37 - 3W L W 3L
            {"WWWLWLWL", "images/sprites/tiles/WWWLWLWL.png"},          // 38 - 3W 2(LW) L
            {"WWWLWLWW", "images/sprites/tiles/WWWLWLWW.png"},          // 39 - 3W 2(LW) W
            {"WWWLWWWL", "images/sprites/tiles/WWWLWWWL.png"},          // 40 - 2(3W L)
            {"WWWWWLLL", "images/sprites/tiles/WWWWWLLL.png"},          // 41 - 5W 3L
            {"WWWWWLWL", "images/sprites/tiles/WWWWWLWL.png"},          // 42 - 5W L W L
            {"WWWWWLWW", "images/sprites/tiles/WWWWWLWW.png"},          // 43 - 5W L 2W
            {"WWWWWWWL", "images/sprites/tiles/WWWWWWWL.png"},          // 44 - 7W L
            {"WWWWWWWW", "images/sprites/tiles/WWWWWWWW.png"},          // 45 - 8W
            {"WLLLWLLL", "images/sprites/tiles/WLLLWLLL.png"},          // 46 - 2(W 3L)
            {"LLWLLLWL", "images/sprites/tiles/LLWLLLWL.png"}           // 47 - 2L W 3L W L
    };
    //endregion

    MapGenerator::MapGenerator(Game &engine, guid_t owner) :
    Script(engine, owner), engine(engine), seaNodes(*this) {
        Init();
    }

    void MapGenerator::Invalidate() {
        ClearMap();
        Script::Invalidate();
    }

    void MapGenerator::Init() {
        auto* gm = entity()->GetComponent<GameManager>();
        mapSize = gm->GetMapWidth();
        noise = FastNoiseLite();
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    }

    void MapGenerator::GenerateNewMap(const int& seed) {
        if(seed == 0)
            perlinSeed = RandomInt(0, std::numeric_limits<int>::max());
        else
            perlinSeed = seed;
        noise.SetSeed(perlinSeed);
        GenerateMap();
        CreateGraph();
        DrawBoundaryLines();
    }

    void MapGenerator::ClearMap() {
        for(auto& b : boundaries)
            b.Invalidate();
        boundaries.clear();

        // reset sea node graph
        seaNodes.Reset();

        // clear tiles
        tileMap.clear();
        tileGrid.clear();
        GameObject::Destroy(tileParent);
        tileParent = nullptr;
    }

    void MapGenerator::DrawBoundaryLines() {
        boundaries.reserve(4);
        auto pos = entity()->transform->GetGlobalPosition();
        auto minX = pos.x - 0.1f;
        auto minY = pos.y - 0.1f;
        auto maxX = pos.x + (float) mapSize * 2 - 1.9f;
        auto maxY = pos.y + (float) mapSize * 2 - 1.9f;

        auto bottomLeftCorner = Vector2(minX, minY);
        auto bottomRightCorner = Vector2(maxX, minY);
        auto topLeftCorner = Vector2(minX, maxY);
        auto topRightCorner = Vector2(maxX, maxY);

        boundaries.emplace_back(engine, bottomLeftCorner, bottomRightCorner, Color::red);
        boundaries.emplace_back(engine, bottomRightCorner, topRightCorner, Color::red);
        boundaries.emplace_back(engine, topRightCorner, topLeftCorner, Color::red);
        boundaries.emplace_back(engine, topLeftCorner, bottomLeftCorner, Color::red);
    }

    void MapGenerator::GenerateMap() {
        GenerateTerrain();
        PlaceTiles();
    }

    void MapGenerator::GenerateTerrain() {
        tileMap = std::vector<std::vector<TileTypes>>(mapSize, std::vector<TileTypes>(mapSize));
        for(int i = 0; i < mapSize; i++)
            for(int j = 0; j < mapSize; j++)
                tileMap[i][j] = DetermineTerrain(i, j);
    }

    TileTypes MapGenerator::DetermineTerrain(const int &x, const int &y) {
        if(perlinModifier == 0)
            perlinModifier = (float) mapSize * mapScale;

        if(perlinModifier == 0)
            DEBUG_RUNTIME_ERROR("Perlin Modifier set to 0!")
        float xCoord = ((float) x - xOffset) / perlinModifier;
        float yCoord = ((float) y - yOffset) / perlinModifier;
        float perlin = (noise.GetNoise(xCoord, yCoord) + 1) / 2;

        if(perlin >= perlinLandThreshold)
            return LAND;
        return WATER;
    }

    void MapGenerator::PlaceTiles() {
        tileParent = new GameObject(engine);
        tileGrid.resize(mapSize);
        for(int i = 0; i < mapSize; i++){
            tileGrid.reserve(mapSize);
            for(int j = 0; j < mapSize; j++){
                fs::path path = "";
                auto tileType = tileMap[i][j];
                std::string waterTileType;

                switch (tileType) {
                    case WATER:
                        waterTileType = EvaluateTileNeighbors(i, j);
                        path = waterTileDictionary.at(waterTileType);
                        break;
                    case LAND:
                        path = MapTile::landTileSpritePath;
                        break;
                }

                if(waterTileType != "WWWWWWWW")
                    tileGrid[i].push_back(CreateTile(i, j, path));
            }
        }
    }

    MapTile MapGenerator::CreateTile(const int &x, const int &y, const fs::path& path) {
        auto tileType = tileMap[x][y];
        const std::string name = "tile-x" + std::to_string(x) + "-y" + std::to_string(y);

        // manage tile -> naming, tagging and transform.
        auto tile = MapTile(engine, path, tileParent->transform,
                            Vector3((float) x * 2, (float) y * 2, -1.1));
        tile.Rename(name);
        tile.SetTag(Ocean_Outlaws::to_string(tileType));

        return tile;
    }

    std::string MapGenerator::EvaluateTileNeighbors(const int &x, const int &y) {
        /** check all 8 neighboring tiles and return a char for each. returns L if adjacent to a land tile,
        and W for a water tile. Land tiles on the N, S, W, and E neighbors automatically assign land tiles
        to adjacent corners since irrelevant for sprite choice **/
        char neighbors[8] = {};

        //region init + safety checks
        for(auto& neighbor : neighbors)
            neighbor = 'v';
        if (x == 0) {
            neighbors[1] = 'W';
            neighbors[2] = 'W';
            neighbors[3] = 'W';
        }
        if (y == 0) {
            neighbors[3] = 'W';
            neighbors[4] = 'W';
            neighbors[5] = 'W';
        }
        if (x == mapSize - 1) {
            neighbors[5] = 'W';
            neighbors[6] = 'W';
            neighbors[7] = 'W';
        }
        if ( y == mapSize -1) {
            neighbors[0] = 'W';
            neighbors[1] = 'W';
            neighbors[7] = 'W';
        }
        //endregion

        //region north (x, y+1) -> n[0]
        if (neighbors[0] == 'v') {
            switch (tileMap[x][y + 1])
            {
                case WATER:
                    neighbors[0] = 'W';
                    break;
                default:
                    neighbors[0] = 'L';
                    neighbors[1] = 'L';
                    neighbors[7] = 'L';
                    break;
            }
        }
        //endregion

        //region north-west (x-1, y+1) -> n[1]
        if (neighbors[1] == 'v') {
            switch (tileMap[x - 1][y + 1])
            {
                case WATER:
                    neighbors[1] = 'W';
                    break;
                default:
                    neighbors[1] = 'L';
                    break;
            }
        }
        //endregion

        //region west (x-1, y) -> n[2]
        if (neighbors[2] == 'v') {
            switch (tileMap[x-1][y])
            {
                case WATER:
                    neighbors[2] = 'W';
                    break;
                default:
                    neighbors[2] = 'L';
                    neighbors[1] = 'L';
                    neighbors[3] = 'L';
                    break;
            }
        }
        //endregion

        //region south-west (x-1, y-1) -> n[3]
        if (neighbors[3] == 'v') {
            switch (tileMap[x-1][y-1])
            {
                case WATER:
                    neighbors[3] = 'W';
                    break;
                default:
                    neighbors[3] = 'L';
                    break;
            }
        }
        //endregion

        //region south (x, y-1) -> n[4]
        if (neighbors[4] == 'v') {
            switch (tileMap[x][y-1])
            {
                case WATER:
                    neighbors[4] = 'W';
                    break;
                default:
                    neighbors[4] = 'L';
                    neighbors[3] = 'L';
                    neighbors[5] = 'L';
                    break;
            }
        }
        //endregion

        //region south-east (x+1, y-1) -> n[5]
        if (neighbors[5] == 'v') {
            switch (tileMap[x+1][y-1])
            {
                case WATER:
                    neighbors[5] = 'W';
                    break;
                default:
                    neighbors[5] = 'L';
                    break;
            }
        }
        //endregion

        //region east (x+1, y) -> n[6]
        if (neighbors[6] == 'v') {
            switch (tileMap[x+1][y])
            {
                case WATER:
                    neighbors[6] = 'W';
                    break;
                default:
                    neighbors[6] = 'L';
                    neighbors[5] = 'L';
                    neighbors[7] = 'L';
                    break;
            }
        }
        //endregion

        //region north-east (x+1, y+1) -> n[7]
        if (neighbors[7] == 'v') {
            switch (tileMap[x+1][y+1]) {
                case WATER:
                    neighbors[7] = 'W';
                    break;
                default:
                    neighbors[7] = 'L';
                    break;
            }
        }
        //endregion

        std::string result(neighbors, 8);

        if (result == "WWWWWWWW")
            waterNodeCoordinates.emplace_back((float) x, (float) y);

        return result;
    }

    void MapGenerator::CreateGraph() {
        for(auto& coord : waterNodeCoordinates){
            auto node = &seaNodes.nodes.emplace_back(nodeCounter++);
            node->coordinates = coord;
        }
        waterNodeCoordinates.clear();
        seaNodes.AssignNeighbors();
    }

    SeaNode *MapGenerator::FindClosestNode(const Vector2 &coordinates) {
        return seaNodes.FindClosestNode(coordinates);
    }
}
