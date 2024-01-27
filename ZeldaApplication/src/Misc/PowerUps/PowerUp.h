#pragma once

#include<Engine.h>
#include "json.hpp"

using json = nlohmann::json;
using namespace Engine;

class PowerUp{
private:
    int value;
    AnimationFilm* m_singleFilm;

public:
    PowerUp() = default;
    ~PowerUp() = default;

    int getValue() const;
    AnimationFilm* getFilm() const;

    void setValue(int newCost);
    void setFilm(AnimationFilm* film);
};

