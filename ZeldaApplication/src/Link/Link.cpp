#include "Link.h"

Link::Link()
{
    m_sheet = new AnimationSheet("link_sheet", "Assets/AnimationFilms/link-sprites.bmp");
    EmplaceFilm("moving_right", new AnimationFilm(m_sheet, "Assets/Config/Animations/Link/moving_right.json"));
    EmplaceFilm("moving_left", new AnimationFilm(m_sheet, "Assets/Config/Animations/Link/moving_left.json"));
    EmplaceFilm("crouch_left", new AnimationFilm(m_sheet, "Assets/Config/Animations/Link/crouch_left.json"));
    EmplaceFilm("crouch_right", new AnimationFilm(m_sheet, "Assets/Config/Animations/Link/crouch_right.json"));

    EmplaceAnimation(new FrameRangeAnimation("moving_right", 0, m_films["moving_right"]->GetTotalFrames(), 0, 300, 12 * 16, 50));
    EmplaceAnimation(new FrameRangeAnimation("moving_left", 0, m_films["moving_left"]->GetTotalFrames(), 0, 300, 12 * 16, 50));
    EmplaceAnimation(new FrameRangeAnimation("crouch_left", 0, m_films["crouch_left"]->GetTotalFrames(), 1, 0, 0, 50));
    EmplaceAnimation(new FrameRangeAnimation("crouch_right", 0, m_films["crouch_right"]->GetTotalFrames(), 1, 0, 0, 50));

    EmplaceAnimator("moving_right", new FrameRangeAnimator());
    EmplaceAnimator("moving_left", new FrameRangeAnimator());
    EmplaceAnimator("crouch_left", new FrameRangeAnimator());
    EmplaceAnimator("crouch_right", new FrameRangeAnimator());

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

std::string Link::GetState()
{
    return m_state;
}

void Link::SetState(std::string _state)
{
    m_state = _state;
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

void Link::takeDamage(int amount) 
{
    health -= amount;

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
