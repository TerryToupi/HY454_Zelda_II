#pragma once 

#include <Engine.h>
#include <json.hpp>
#include "../Link/Link.h"

using json = nlohmann::json;
using namespace Engine;

class Layer1 : public Layer
{
public:
    using Teleports = std::vector<std::pair<Sprite,Sprite>>;

    Layer1();
    ~Layer1() = default;

    void onStart() override;

    void onDelete() override;

    void onUpdate(Time ts) override;

    void onEvent(Event& e) override;

    bool mover(Event& e);

    void move(Time ts);

    void InitializeTeleports();

    void TeleportCheck(); 

public:
    Ref<Scene> m_Scene;
    Link* link;
    Teleports m_teleports;

    int wdx = 100;
    int wdy = 100;

    Time curr = 0;
    Time step = 0;
};