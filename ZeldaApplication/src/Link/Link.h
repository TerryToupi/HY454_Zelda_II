#ifndef LINK_HPP
#define LINK_HPP

#include <Engine.h>
#include "../Entity/Entity.h"
#include "../Spells/LifeSpell.h"
#include "../Spells/ShieldSpell.h"
#include "../Spells/ThunderSpell.h"
#include "../Spells/JumpSpell.h"

using namespace Engine;

class Link : public Entity{
private:
    int health;
    int damage;
    int magicPoints;
    int lives;
    int speed;
    int jumpingForce;
    int m_damageCoolDown;
    bool m_key;


public:
    Link(AnimationSheet* _sheet);
    ~Link() = default;
    Animation* jumpAnimation;

    JumpSpell jumpspell;
    LifeSpell lifespell;
    ShieldSpell shieldspell;


    int getHealth() const;
    int getDamage() const;
    int getMagicPoints() const;
    int getLives() const;
    int getSpeed() const;
    int getJumpingForce() const;
    int getDamageCoolDown() const;
    bool HasKey() const;
    
    void setHealth(int newHealth);
    void setDamage(int newDamage);
    void setMagicPoints(int newMagicPoints);
    void setLives(int newLives);
    void setSpeed(int newLives);
    void setJumpingForce(int jumpingForce);
    void setDamageCoolDown(int _cooldown);


    void takeDamage(int amount);
    void heal(int amount);
    void castSpell(int spellCost);
    void loseLife();
    void SetKey(bool _key);

};

#endif 
