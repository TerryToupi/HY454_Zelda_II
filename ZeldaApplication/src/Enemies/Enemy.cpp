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

uint32_t Enemy::GetID() const
{
    return m_id;
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