#include "Link.h"

Link::Link()
{
    std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
    json configVars = json::parse(file);

    m_damageCoolDown = 0;
    m_lookingAt = "right";
    m_state = "moving";
    m_type = "Link";

    setHealth(configVars["Link"]["HP"]);
    setDamage(configVars["Link"]["Damage"]);
    setMagicPoints(configVars["Link"]["MagicPoints"]);
    setLives(configVars["Link"]["InitialLives"]);
    setSpeed(100 - configVars["Link"]["Speed"]);
    setJumpingForce(configVars["Link"]["JumpingForce"]);


    m_sheet = new AnimationSheet("link_sheet", "Assets/AnimationFilms/link-sprites.bmp");
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
    return health;
}

int Link::getDamage() const 
{
    return damage;
}

int Link::getMagicPoints() const
{
    return magicPoints;
}

int Link::getLives() const 
{
    return lives;
}

int Link::getSpeed() const
{
    return speed;
}

int Link::getJumpingForce() const
{
    return jumpingForce;
}

int Link::getDamageCoolDown() const
{
    return m_damageCoolDown;
}

void Link::setHealth(int newHealth)
{
    health = newHealth;
}

void Link::setDamage(int newDamage) 
{
    damage = newDamage;
}

void Link::setMagicPoints(int newMagicPoints) 
{
    magicPoints = newMagicPoints;
}

void Link::setLives(int newLives)
{
    lives = newLives;
}

void Link::setSpeed(int newSpeed)
{
    speed = newSpeed;
}

void Link::setJumpingForce(int newJumpingForce)
{
    jumpingForce = newJumpingForce;
}

void Link::setDamageCoolDown(int _cooldown)
{
    m_damageCoolDown = _cooldown;
}

void Link::takeDamage(int amount) 
{
    if (shieldspell.isActive())
        amount = amount / 2;

    health -= amount;

    ENGINE_TRACE(amount);
    ENGINE_TRACE(health);

    if (health <= 0)
        loseLife();
}

void Link::heal(int amount) 
{
    health += amount;

    if (health > 100)
        health = 100;
}

void Link::castSpell(int spellCost)
{
    if(magicPoints >= spellCost)
        magicPoints -= spellCost;
}

void Link::loseLife() 
{
    lives -= 1;
}

