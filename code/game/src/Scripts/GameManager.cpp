#include "GameManager.h"

#include "../GameObjects/Barrel.h"
#include "../GameObjects/EndScreen.h"
#include "../GameObjects/MainMenu.h"

namespace Ocean_Outlaws::scripts {
    //region cardinal direction vectors dictionary def
    const std::map<CardinalDirection, Vector2> GameManager::cardinalVectors = {
            {CardinalDirection::EAST,             Vector2(1, 0)},
            {CardinalDirection::EAST_NORTH_EAST,
             Vector2(Math::Cos(22.5f, DEGREES), Math::Sin(22.5f, DEGREES))},
            {CardinalDirection::NORTH_EAST,
             Vector2(Math::Cos(45, DEGREES), Math::Sin(45, DEGREES))},
            {CardinalDirection::NORTH_NORTH_EAST,
             Vector2(Math::Cos(67.5f, DEGREES), Math::Sin(67.5f, DEGREES))},
            {CardinalDirection::NORTH,            Vector2(0, 1)},
            {CardinalDirection::NORTH_NORTH_WEST,
             Vector2(Math::Cos(112.5f, DEGREES), Math::Sin(112.5f, DEGREES))},
            {CardinalDirection::NORTH_WEST,
             Vector2(Math::Cos(135, DEGREES), Math::Sin(135, DEGREES))},
            {CardinalDirection::WEST_NORTH_WEST,
             Vector2(Math::Cos(157.5f, DEGREES), Math::Sin(157.5f, DEGREES))},
            {CardinalDirection::WEST,             Vector2(-1, 0)},
            {CardinalDirection::WEST_SOUTH_WEST,
             Vector2(Math::Cos(202.5f, DEGREES), Math::Sin(202.5f, DEGREES))},
            {CardinalDirection::SOUTH_WEST,
             Vector2(Math::Cos(225, DEGREES), Math::Sin(225, DEGREES))},
            {CardinalDirection::SOUTH_SOUTH_WEST,
             Vector2(Math::Cos(247.5f, DEGREES), Math::Sin(247.5f, DEGREES))},
            {CardinalDirection::SOUTH,            Vector2(0, -1)},
            {CardinalDirection::SOUTH_SOUTH_EAST,
             Vector2(Math::Cos(292.5f, DEGREES), Math::Sin(292.5f, DEGREES))},
            {CardinalDirection::SOUTH_EAST,
             Vector2(Math::Cos(315, DEGREES), Math::Sin(315, DEGREES))},
            {CardinalDirection::EAST_SOUTH_EAST,
             Vector2(Math::Cos(337.5f, DEGREES), Math::Sin(337.5f, DEGREES))}
    };
    //endregion

    GameManager::GameManager(Game &engine, guid_t owner) :
            Script(engine, owner) {
        mainMenu = new MainMenu(engine);
    }

    int GameManager::GetMapWidth() const { return mapWidth;}

    void GameManager::StartGame(){
        if(!mapGenerator)
            mapGenerator = entity()->GetComponent<MapGenerator>();
        mapGenerator->GenerateNewMap();

        EstablishSpawnableNodes();
        PlacePlayer();

        InGame = true;

        if(!mainMenu)
            return;

        GameObject::Destroy(mainMenu);
        mainMenu = nullptr;
    }

    void GameManager::GameOver() {
        onGameOver.invoke(*this);

        GameObject::Destroy(player);
        player = nullptr;

        auto enemies = GameObject::FindAll("Enemy");
        for(auto enemy : enemies)
            GameObject::Destroy(enemy);
        enemiesOnMap = 0;

        auto barrels = GameObject::FindAllWithTag("Barrel");
        for(auto barrel : barrels)
            GameObject::Destroy(barrel);

        if(!mapGenerator)
            mapGenerator = entity()->GetComponent<MapGenerator>();
        mapGenerator->ClearMap();

        spawnableNodes.clear();

        InGame = false;
        InEndScreen = true;

        endScreen = new EndScreen(engine);
    }

    void GameManager::Update() {
        if(InGame){
            if(spawnableNodes.empty())
                DEBUG_RUNTIME_ERROR("No applicable nodes for enemies to spawn!")
            if(CanSpawnEnemy())
                SpawnShip();
            if(CanSpawnBarrel())
                PlaceBarrel();
        }
        else {
            if(menuTimer > 0)
                menuTimer -= Time::deltaTime;
        }
    }

    void GameManager::NavigateMenu() {
        if(InEndScreen)
            OpenMainMenu();
        else if (menuTimer <= 0)
            StartGame();
    }

    void GameManager::EstablishSpawnableNodes() {
        for(auto& node : entity()->GetComponent<MapGenerator>()->seaNodes.nodes){
            if(node.edges.size() != 8)
                continue;

            bool is3PlusTilesFromLand = true;

            for(auto edge : node.edges){
                auto neighbor = *edge->A == node ? edge->B : edge->A;
                if(neighbor->edges.size() != 8){
                    is3PlusTilesFromLand = false;
                    break;
                }
            }

            if(is3PlusTilesFromLand)
                spawnableNodes.emplace_back(&node);
        }
    }

    bool GameManager::CanSpawnEnemy() {
        if(enemiesOnMap >= maxEnemiesOnMap)
            return false;

        applicablePositions.clear();
        FindAdequatePositions();

        if(applicablePositions.empty())
            return false;
        return true;
    }

    bool GameManager::CanSpawnBarrel() {
        auto barrelCount = (int) GameObject::FindAllWithTag("Barrel").size();
        if(spawnableNodes.empty() || barrelCount >= maxBarrelsOnMap)
            return false;
        return true;
    }

    void GameManager::FindAdequatePositions() {
        auto player = GameObject::Find("Player");
        for(auto node : spawnableNodes){
            if(Vector2::Distance((Vector2) player->transform->GetGlobalPosition(),
                                 2*(node->coordinates)) >= minSpawnDistFromPlayer)
                applicablePositions.emplace_back(2*node->coordinates.x, 2*node->coordinates.y, 0);
        }
    }

    void GameManager::SpawnShip() {
        auto newShip = new EnemyShip(engine, 4,applicablePositions[RandomInt(0,
                                                                (int) applicablePositions.size()-1)]);
        enemiesOnMap++;
        newShip->OnShipwreck.addListener([&] (Ship &ship){
            enemiesOnMap--;
        });
    }

    void GameManager::PlacePlayer() {
        // find closest viable position to center of map (100, 100)
        auto closestPos = std::numeric_limits<float>::max();
        Vector3 playerSpawnPos = Vector3::zero;
        for(auto node : spawnableNodes){
            auto dist = Vector2::Distance(Vector2(100, 100),2*(node->coordinates));
            if(dist < closestPos){
                closestPos = dist;
                playerSpawnPos = Vector3(2*node->coordinates.x, 2*node->coordinates.y, 0);
            }
        }
        // place player
        player = new PlayerShip(engine);
        player->transform->SetGlobalPosition(playerSpawnPos);
    }

    void GameManager::PlaceBarrel() {
        auto pos = Vector3(spawnableNodes[RandomInt(0, (int) spawnableNodes.size() - 1)]->coordinates * 2);
        new Barrel(engine, pos);
    }

    void GameManager::OpenMainMenu() {
        if(endScreen){
            GameObject::Destroy(endScreen);
            endScreen = nullptr;
        }
        mainMenu = new MainMenu(engine);
        InEndScreen = false;

        menuTimer = 1.0f;
    }
}
