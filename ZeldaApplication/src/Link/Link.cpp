#include "Link.h"

Link::Link(AnimationSheet* _sheet, Ref<Scene> _scene)
{
    std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
    json configVars = json::parse(file);

    m_damageCoolDown = 0;
    m_lookingAt = "right";
    m_state = "moving";
    m_scene = _scene;
    m_keys = 0;

    setHealth(configVars["Link"]["HP"]);
    setDamage(configVars["Link"]["Damage"]);
    setMagicPoints(configVars["Link"]["MagicPoints"]);
    setLives(configVars["Link"]["InitialLives"]);
    setSpeed(100 - configVars["Link"]["Speed"]);
    setJumpingForce(configVars["Link"]["JumpingForce"]);
    setPoints(0);

    m_sheet = _sheet;
    EmplaceFilm("moving_right", new AnimationFilm(m_sheet, "Assets/Config/Animations/Link/moving_right.json"));
    EmplaceFilm("moving_left", new AnimationFilm(m_sheet, "Assets/Config/Animations/Link/moving_left.json"));
    EmplaceFilm("crouch_left", new AnimationFilm(m_sheet, "Assets/Config/Animations/Link/crouch_left.json"));
    EmplaceFilm("crouch_right", new AnimationFilm(m_sheet, "Assets/Config/Animations/Link/crouch_right.json"));
    EmplaceFilm("attacking_left", new AnimationFilm(m_sheet, "Assets/Config/Animations/Link/attacking_left.json"));
    EmplaceFilm("attacking_right", new AnimationFilm(m_sheet, "Assets/Config/Animations/Link/attacking_right.json"));
    EmplaceFilm("crouch_attack_left", new AnimationFilm(m_sheet, "Assets/Config/Animations/Link/crouch_attack_left.json"));
    EmplaceFilm("crouch_attack_right", new AnimationFilm(m_sheet, "Assets/Config/Animations/Link/crouch_attack_right.json"));
    EmplaceFilm("damage_from_left", new AnimationFilm(m_sheet, "Assets/Config/Animations/Link/damage_from_left.json"));
    EmplaceFilm("damage_from_right", new AnimationFilm(m_sheet, "Assets/Config/Animations/Link/damage_from_right.json"));

    EmplaceAnimation(new FrameRangeAnimation("frame_moving_right", 0, m_films["moving_right"]->GetTotalFrames(), 0, 300, 12 * 16, 50));
    EmplaceAnimation(new FrameRangeAnimation("frame_moving_left", 0, m_films["moving_left"]->GetTotalFrames(), 0, 300, 12 * 16, 50));
    EmplaceAnimation(new FrameRangeAnimation("frame_crouch_left", 0, m_films["crouch_left"]->GetTotalFrames(), 1, 0, 0, 50));
    EmplaceAnimation(new FrameRangeAnimation("frame_crouch_right", 0, m_films["crouch_right"]->GetTotalFrames(), 1, 0, 0, 50));
    EmplaceAnimation(new FrameRangeAnimation("frame_attacking_left", 0, m_films["attacking_left"]->GetTotalFrames(), 1, 0, 0, 100));
    EmplaceAnimation(new FrameRangeAnimation("frame_attacking_right", 0, m_films["attacking_right"]->GetTotalFrames(), 1, 0, 0, 100));
    EmplaceAnimation(new FrameRangeAnimation("frame_crouch_attack_left", 0, m_films["crouch_attack_left"]->GetTotalFrames(), 1, 0, 0, 100));
    EmplaceAnimation(new FrameRangeAnimation("frame_crouch_attack_right", 0, m_films["crouch_attack_right"]->GetTotalFrames(), 1, 0, 0, 100));

    jumpAnimation = new MovingAnimation("mov_jumping", getJumpingForce(), 0, 0, 20);
    EmplaceAnimation(jumpAnimation);
    EmplaceAnimation(new MovingAnimation("mov_moving", 0, 0, 0, getSpeed()));
    EmplaceAnimation(new MovingAnimation("mov_damage", 5, 0, 0, getSpeed()));
    EmplaceAnimation(new FrameRangeAnimation("frame_damage_from_left", 0, m_films["damage_from_left"]->GetTotalFrames(), 2, 0, 0, 10));
    EmplaceAnimation(new FrameRangeAnimation("frame_damage_from_right", 0, m_films["damage_from_right"]->GetTotalFrames(), 2, 0, 0, 10));
  
    EmplaceAnimation(new MovingAnimation("mov_gravity", 0, 0, 0, 4));
    EmplaceAnimation(new MovingAnimation("mov_damage", 5, 0, 0, 10));

    EmplaceAnimator("frame_animator", new FrameRangeAnimator());
    EmplaceAnimator("mov_jumping", new MovingAnimator());
    EmplaceAnimator("mov_moving", new MovingAnimator());
    EmplaceAnimator("mov_gravity", new MovingAnimator());
    EmplaceAnimator("mov_damage", new MovingAnimator());

    InitializeAnimators();  //initializes the onAction and OnFinish of all inserted animations in the entity
}

int Link::getHealth() const 
{
    return m_health;
}

int Link::getDamage() const 
{
    return m_damage;
}

int Link::getMagicPoints() const
{
    return m_magicPoints;
}

int Link::getLives() const 
{
    return m_lives;
}

int Link::getSpeed() const
{
    return m_speed;
}

int Link::getJumpingForce() const
{
    return m_jumpingForce;
}

int Link::getDamageCoolDown() const
{
    return m_damageCoolDown;
}

int Link::getAttackingStateCoolDown() const
{
    return m_attackingStateCoolDown;
}

int Link::getPoints() const
{
    return m_points;
}

int Link::getKeys() const
{
    return m_keys;
}


bool Link::HasKey() const
{
    return m_keys > 0;
}

void Link::setHealth(int newHealth)
{
    m_health = newHealth;
}

void Link::setDamage(int newDamage) 
{
    m_damage = newDamage;
}

void Link::setMagicPoints(int newMagicPoints) 
{
    m_magicPoints = newMagicPoints;
}

void Link::setLives(int newLives)
{
    m_lives = newLives;
}

void Link::setSpeed(int newSpeed)
{
    m_speed = newSpeed;
}

void Link::setJumpingForce(int newJumpingForce)
{
    m_jumpingForce = newJumpingForce;
}

void Link::setDamageCoolDown(int _cooldown)
{
    m_damageCoolDown = _cooldown;
}

void Link::setAttackingStateCoolDown(int _cooldown)
{
    m_attackingStateCoolDown = _cooldown;
}

void Link::setPoints(int _points)
{
    m_points = _points;
}

void Link::takeDamage(int amount) 
{
    if (shieldspell.isActive())
        amount = amount / 2;

    m_health -= amount;

    if (m_health <= 0)
        loseLife();
}

void Link::heal(int amount) 
{
    m_health += amount;

    if (m_health > 100)
        m_health = 100;
}

void Link::castSpell(int spellCost)
{
    if(m_magicPoints >= spellCost)
        m_magicPoints -= spellCost;
}

void Link::loseLife() 
{
    m_lives -= 1;
}

void Link::AddKey()
{
    m_keys++;
}

void Link::RemoveKey()
{
    m_keys--;
}
