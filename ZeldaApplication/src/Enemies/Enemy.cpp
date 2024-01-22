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

int Enemy::GetStage() const
{
    return m_stage;
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

void Enemy::Attack() {

}

void Enemy::TakeDamage(int amount) {
    health -= amount;

    if (health <= 0) {
        Disapear();
    }
}

void Enemy::Disapear() {

}

void Enemy::Move() {

}