#pragma once 

#include <Engine.h>
#include "../Link/Link.h"

using namespace Engine;

class Layer1 : public Layer
{
public:
    Layer1();
    ~Layer1() = default;

    void onStart() override;

    void onDelete() override;

    void onUpdate(Time ts) override;

    void onEvent(Event& e) override;

    bool mover(Event& e);

    void move(Time ts);

public:
    Ref<Scene> m_Scene;
    Ref<AnimationSheet> m_linkSheet;
    Link* link;

    std::unordered_map < std::string, Ref<AnimationFilm> > animationFilmsMap;
    std::unordered_map < std::string, Ref<FrameRangeAnimation> > frameRangeAnimationsMap;
    std::unordered_map < std::string, Ref<FrameRangeAnimator> > frameRangeAnimatorsMap;

    Ref<MovingAnimator> m_movingLink;
    Ref<MovingAnimator> m_CamLeft;
    Ref<MovingAnimator> m_CamRight;

    int wdx = 100;
    int wdy = 100;

    Time curr = 0;
    Time step = 0;
};