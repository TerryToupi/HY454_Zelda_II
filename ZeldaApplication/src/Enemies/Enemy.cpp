#include "Enemy.h"

int Enemy::GetHealth() const {
    return health;
}

int Enemy::GetDamage() const {
    return damage;
}

int Enemy::GetPoints() const {
    return points;
}

int Enemy::GetSpeed() const
{
    return speed;
}

int Enemy::GetStage() const
{
    return m_stage;
}

int Enemy::getDamageCoolDown() const
{
    return m_damageCoolDown;
}

uint32_t Enemy::GetMaxX() const
{
    return m_maxX;
}

uint32_t Enemy::GetMinX() const
{
    return m_minX;
}

void Enemy::SetHealth(int newHealth) {
    health = newHealth;
}

void Enemy::SetDamage(int newDamage) {
    damage = newDamage;
}

void Enemy::SetPoints(int newPoints) {
    points = newPoints;
}

void Enemy::SetSpeed(int newSpeed) {
    speed = newSpeed;
}

void Enemy::setDamageCoolDown(int _cooldown)
{
    m_damageCoolDown = _cooldown;
}


void Enemy::SetMinX(uint32_t _minX)
{
    m_minX = _minX;
}

void Enemy::SetMaxX(uint32_t _maxX)
{
    m_maxX = _maxX;
}

void Enemy::Attack() {

}

void Enemy::TakeDamage(int amount) {
    health -= amount;
    if (health < 0)
        health = 0;
}

void Enemy::Disapear() {

}

void Enemy::Move() {

}