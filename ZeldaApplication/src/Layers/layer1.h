#pragma once 

#include <Engine.h>
#include <json.hpp>
#include "../Link/Link.h"
#include "../Enemies/Enemies.h"
#include "../Misc/Door/Door.h"
#include "../Misc/Collectible/Collectinble.h"
#include "../Misc/Elevator/Elevator.h"
#include "../Misc/Block/Block.h"
#include "../Misc/Arrows/Arrow.h"
#include "../Flash/Flash.h"

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
    using Blocks = std::vector<Block*>;
    using Arrows = std::unordered_map<ID, Arrow*>;

    Layer1();

    /*-----INITIALIZERS-----*/
    void InitializeTeleports();
    void InitializeStages();
    void InitializeEnemies(GridLayer *grid);
    void InitializeDoors();
    void InitialiazeCollectibles();
    void InitializeAudio();
    void InitializeElevators(GridLayer* grid);
    void InitializeBridge();
    void LoadSheets();
    
    /*----LAYER FUNCTIONS----*/
    void onStart() override;
    void onDelete() override;
    void onUpdate(Time ts) override;
    void onEvent(Event& e) override;
    bool LinkStartAnimations(KeyTapEvent& e);
    bool LinkStopAnimations(KeyReleaseEvent& e);
    void EnemyMovement();
    void ResetElevators();
    bool ElevatorStart(KeyTapEvent& e);
    bool ElevatorStop(KeyReleaseEvent& e);
    void SpawnArrow(std::string direction, uint32_t x, uint32_t y);
 
 
    /*----HANDLERS---*/
    void TeleportHandler();
    void EnemyHandler();
    void CheckTimers(Time ts);
    void UpdateSpell(Spell& spell, Time ts);
    void UpdateCollectibleCooldowns(Time ts);
    void DoorHandler();
    void CollectibleHandler();
    void ElevatorHandler();
    void BridgeHandler();
    void CreateCollectible(std::string jsonPath, std::string type, enum c_type cType);
    void DropCollectible(Enemy* enemy);
    void SpellFollowLink();
    void ArrowHandler();
    void WaypointHandler();

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
    Blocks m_blocks;
    Arrows m_arrows;

    Sprite m_lava;
    Sprite m_end;

    Flash flash;
    int waitTeleportCounter = 0;
};