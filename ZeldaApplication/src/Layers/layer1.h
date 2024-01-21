#pragma once 

#include <Engine.h>
#include <json.hpp>
#include "../Link/Link.h"

using json = nlohmann::json;
using namespace Engine;

typedef struct teleportsStruct {
    Sprite origin;
    Sprite dest;
    uint32_t stage;
}Teleports;

class Layer1 : public Layer
{
public:
    using Stages = std::vector<std::pair<int, int>>;

    Layer1();
    ~Layer1() = default;

    void onStart() override;

    void onDelete() override;

    void onUpdate(Time ts) override;

    void onEvent(Event& e) override;

    bool mover(Event& e);

    void move(Time ts);

    void InitializeTeleports();

    void InitializeStages();

    void TeleportCheck();

    void InitializeCamera();

    bool cameraInStageBounds(int cameraPosition, int cameraWidth, int stageMaxLeft, int stageMaxRight);

public:
    Ref<Scene> m_Scene;
    Link* link;
    std::vector<Teleports> m_teleports;
    Stages m_stages;
    uint32_t m_currStage;

    MovingAnimator* m_CameraLeft;
    MovingAnimator* m_CameraRight;
    MovingAnimation* m_CameraLeftAnim;
    MovingAnimation* m_CameraRightAnim;

    int wdx = 100;
    int wdy = 100;

    Time curr = 0;
    Time step = 0;
};