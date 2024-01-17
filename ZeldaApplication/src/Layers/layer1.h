#pragma once 

#include <Engine.h>
#include "../Link/Link.h"

using namespace Engine;

class layer1 : public Layer
{
public:
    layer1();
    ~layer1() = default;

    void onStart() override;

    void onDelete() override;

    void onUpdate(Time ts) override;

    void onEvent(Event& e) override;

    bool mover(Event& e);

    void FrameRangeActionLeft();
    void FrameRangeActionRight();
    void FrameRangeActionCrouchLeft();
    void FrameRangeActionCrouchRight();
    void FrameRangeActionAttackLeft();
    void FrameRangeActionAttackRight();
    void FrameRangerFinish(Animator* animator, const Animation& anim);

    void move(Time ts);

public:
    Ref<Scene> m_Scene;
    Ref<AnimationSheet> m_linkSheet;
    Link* link;


    Ref<MovingAnimator> m_movingLink;
    Ref<MovingAnimator> m_CamLeft;
    Ref<MovingAnimator> m_CamRight;

    int wdx = 100;
    int wdy = 100;

    Time curr = 0;
    Time step = 0;
};