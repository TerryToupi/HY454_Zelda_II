#ifndef LINK_HPP
#define LINK_HPP

#include <Engine.h>
#include "../Entity/Entity.h"
#include "../Misc/Spells/LifeSpell.h"
#include "../Misc/Spells/ShieldSpell.h"
#include "../Misc/Spells/ThunderSpell.h"
#include "../Misc/Spells/JumpSpell.h"
#include "../Misc/PowerUps/Key.h"
#include "../Misc/PowerUps/RedPotion.h"
#include "../Misc/PowerUps/BluePotion.h"
#include "../Misc/PowerUps/BasicPointBag.h"
#include "../Misc/PowerUps/StrongerPointBag.h"
#include "../Misc/PowerUps/ExtraLife.h"

using namespace Engine;

class Link : public Entity{
private:
    int m_health;
    int m_maxHealth;
    int m_damage;
    int m_magicPoints;
    int m_maxMagicPoints;
    int m_lives;
    int m_speed;
    int m_jumpingForce;
    int m_damageCoolDown;
    int m_attackingStateCoolDown;
    int m_keys;
    int m_points;
 
public:
    Link(AnimationSheet* _sheet, Ref<Scene> _scene);
    ~Link() = default;
    Animation* jumpAnimation;

    JumpSpell jumpspell;
    LifeSpell lifespell;
    ShieldSpell shieldspell;
    ThunderSpell thunderspell;

    Key key;
    BluePotion bluepotion;
    RedPotion redpotion;
    BasicPointBag basicpointbag;
    StrongerPointBag strongerpointbag;
    ExtraLife extralife;

    void SetKritikos(bool _kritikos);
    bool IsKritikos();

    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getMagicPoints() const;
    int getMaxMagicPoints() const;
    int getLives() const;
    int getSpeed() const;
    int getJumpingForce() const;
    int getDamageCoolDown() const;
    int getAttackingStateCoolDown() const;
    int getPoints() const;
    int getKeys() const;
    
    void setHealth(int newHealth);
    void setMaxHealth(int newHealth);
    void setDamage(int newDamage);
    void setMagicPoints(int newMagicPoints);
    void setMaxMagicPoints(int newMagicPoints);
    void setLives(int newLives);
    void setSpeed(int newLives);
    void setJumpingForce(int jumpingForce);
    void setDamageCoolDown(int _cooldown);
    void setAttackingStateCoolDown(int _cooldown);
    void setPoints(int _points);


    void takeDamage(int amount);
    void heal(int amount);
    void castSpell(int spellCost);
    void loseLife();
    void RemoveKey();
    void AddKey();
    bool HasKey() const;

};

#endif 
