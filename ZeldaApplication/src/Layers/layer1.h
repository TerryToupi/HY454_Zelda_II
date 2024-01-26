#pragma once 

#include <Engine.h>
#include <json.hpp>
#include "../Link/Link.h"
#include "../Enemies/Enemies.h"
#include "../Door/Door.h"

using json = nlohmann::json;
using namespace Engine;

/*
----Teleport Location Structure----
origin  : waypoint of origin
dest    : waypoint of destination
stage   : stage that the location belongs to
-----------------------------------
*/



typedef struct teleportsStruct {
    Sprite origin;
    Sprite dest;
    uint32_t stage;
}Teleports;

class Layer1 : public Layer
{
public:
    using Stages = std::vector<std::pair<int, int>>;
    using Enemies = std::unordered_map<ID, Enemy*>;
    using Sounds = std::unordered_map<std::string, AudioID>;
    using Sheets = std::unordered_map<std::string, AnimationSheet*>;
    using Collectibles = std::unordered_map<std::string, Entity*>;
    using Doors = std::unordered_map<ID, Door*>;
    using Elevators = std::unordered_map<ID, Entity*>;

    Layer1();

    /*-----INITIALIZERS-----*/
    void InitializeTeleports();
    void InitializeStages();
    void InitializeEnemies(GridLayer *grid);
    void InitializeDoors();
    void InitialiazeCollectibles();
    void InitializeAudio();
    void LoadSheets();
    
    /*----LAYER FUNCTIONS----*/
    void onStart() override;
    void onDelete() override;
    void onUpdate(Time ts) override;
    void onEvent(Event& e) override;
    bool mover(Event& e);
    void EnemyMovement();
    
    /*----HANDLERS---*/
    void TeleportHandler();
    void EnemyHandler();
    void CheckTimers(Time ts);
    void UpdateSpell(Spell& spell, Time ts);
    void DoorHandler();
    void CollectibleHandler();

public:
    Link* link;                                         // main character instance
    std::vector<Teleports> m_teleports;                 // teleport locations
    Stages m_stages;                                    // map stages
    uint32_t m_currStage;                               // stage tracker
    Sounds m_sounds;  // sound files
    Enemies m_enemies;
    Sheets m_sheets;
    Collectibles m_collectibles;
    Doors m_doors;


    Wosu* wosu;
};