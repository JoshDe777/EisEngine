#pragma once

#include <EisEngine.h>

#include "MapGenerator.h"
#include "../Enums.h"
#include "../GameObjects/PlayerShip.h"
#include "../GameObjects/EnemyShip.h"
#include "../MapGeneration/SeaNode.h"

namespace Ocean_Outlaws::scripts {
    using namespace Ocean_Outlaws::map;
    /// \n The central game manager for Ocean Outlaws!.
    class GameManager : public Script {
        using event = EisEngine::events::Event<GameManager, GameManager&>;
    public:
        /// \n A dictionary of Vector2's mapped to a cardinal direction.
        static const std::map<CardinalDirection, Vector2> cardinalVectors;
        /// \n The central game manager for Ocean Outlaws!
        explicit GameManager(Game &engine, guid_t owner);
        /// \n Gets the map's width.
        [[nodiscard]] int GetMapWidth() const;
        /// \n Called when the game is supposed to end.
        void GameOver();

        void StartGame();
        bool IsInGame() const { return InGame;}
        void NavigateMenu();

        /// \n An event invoked whenever the game is meant to end.
        event onGameOver;
    protected:
        void Update() override;
    private:
        /// \n The width of the game map in tiles.
        int mapWidth = 100;

        //region enemy spawning
        void EstablishSpawnableNodes();
        bool CanSpawnEnemy();
        void FindAdequatePositions();
        void SpawnShip();
        void OpenMainMenu();
        void PlacePlayer();
        bool CanSpawnBarrel();
        void PlaceBarrel();

        int maxEnemiesOnMap = 5;
        int maxBarrelsOnMap = 30;
        int enemiesOnMap = 0;
        float minSpawnDistFromPlayer = 40;

        bool InGame = false;
        bool InEndScreen = false;

        MapGenerator* mapGenerator = nullptr;

        PlayerShip* player = nullptr;
        GameObject* mainMenu = nullptr;
        GameObject* endScreen = nullptr;
        float menuTimer = 0;

        std::vector<SeaNode*> spawnableNodes;
        std::vector<Vector3> applicablePositions;
        //endregion
    };
}

using GameManager = Ocean_Outlaws::scripts::GameManager;
