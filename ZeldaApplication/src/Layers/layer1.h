#pragma once 

#include <Engine.h>
#include <json.hpp>
#include "../Link/Link.h"
#include "../Enemies/Wosu.h"

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
    using Enemies = std::unordered_map<uint32_t, Enemy*>;
    using Sounds = std::unordered_map<std::string, AudioID>;

    Layer1();
    ~Layer1() = default;

    /*-----INITIALIZERS-----*/
    void InitializeTeleports();
    void InitializeStages();
    void InitializeAudio();
    
    /*----LAYER FUNCTIONS----*/
    void onStart() override;
    void onDelete() override;
    void onUpdate(Time ts) override;
    void onEvent(Event& e) override;
    bool mover(Event& e);
    
    /*----CHECKERS----*/
    void TeleportCheck();

public:
    Ref<Scene> m_Scene;
    Link* link;                                         // main character instance
    std::vector<Teleports> m_teleports;                 // teleport locations
    Stages m_stages;                                    // map stages
    uint32_t m_currStage;                               // stage tracker
    Sounds m_sounds;  // sound files
    Enemies m_enemies;
};