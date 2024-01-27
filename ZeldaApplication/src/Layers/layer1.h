#pragma once 

#include <Engine.h>
#include <json.hpp>
#include "../Link/Link.h"
#include "../Enemies/Enemies.h"
#include "../Misc/Door/Door.h"
#include "../Misc/Collectible/Collectinble.h"
#include "../Misc/Elevator/Elevator.h"

using json = nlohmann::json;
using namespace Engine;

/*
----Teleport Location Structure----
origin  : waypoint of origin
dest    : waypoint of destination
stage   : stage that the location belongs to
-----------------------------------
*/



typedef struct teleportsStruct 
{
    Sprite origin;
    Sprite dest;
    uint32_t stage;
}Teleport;

class Layer1 : public Layer
{
public:
    using Stages = std::vector<std::pair<int, int>>;
    using Enemies = std::unordered_map<ID, Enemy*>;
    using Sounds = std::unordered_map<std::string, AudioID>;
    using Sheets = std::unordered_map<std::string, AnimationSheet*>;
    using Collectibles = std::unordered_map<c_type, std::vector<Collectible*>>;
    using Doors = std::unordered_map<ID, Door*>;
    using Elevators = std::unordered_map<ID, Elevator*>;
    using Teleports = std::vector<Teleport>;

    Layer1();

    /*-----INITIALIZERS-----*/
    void InitializeTeleports();
    void InitializeStages();
    void InitializeEnemies(GridLayer *grid);
    void InitializeDoors();
    void InitialiazeCollectibles();
    void InitializeAudio();
    void InitializeElevators(GridLayer* grid);
    void LoadSheets();
    
    /*----LAYER FUNCTIONS----*/
    void onStart() override;
    void onDelete() override;
    void onUpdate(Time ts) override;
    void onEvent(Event& e) override;
    bool mover(Event& e);
    void EnemyMovement();
    bool ElevatorMovement(Event& e);
    
    /*----HANDLERS---*/
    void TeleportHandler();
    void EnemyHandler();
    void CheckTimers(Time ts);
    void UpdateSpell(Spell& spell, Time ts);
    void UpdateCollectibleCooldowns(Time ts);
    void DoorHandler();
    void CollectibleHandler();
    void ElevatorHandler();
    void CreateCollectible(std::string json, std::string type, enum c_type);
    void DropCollectible(Enemy* enemy);
    void SpellFollowLink();

public:
    Link* link;                             // main character instance
    Teleports m_teleports;                  // teleport locations
    Teleports m_elevator_teleports;
    Stages m_stages;                        // map stages
    uint32_t m_currStage;                   // stage tracker
    Sounds m_sounds;                        // sound files
    Enemies m_enemies;
    Sheets m_sheets;
    Collectibles m_collectibles;
    Doors m_doors;
    Elevators m_elevators;



    Wosu* wosu;
};