#include "Enemy.h"

int Enemy::getHealth() const {
    return health;
}

int Enemy::getDamage() const {
    return damage;
}

int Enemy::getPoints() const {
    return points;
}


void Enemy::setHealth(int newHealth) {
    health = newHealth;
}

void Enemy::setDamage(int newDamage) {
    damage = newDamage;
}

void Enemy::setPoints(int newPoints) {
    points = newPoints;
}

void Enemy::attack() {

}

void Enemy::takeDamage(int amount) {
    health -= amount;

    if (health <= 0) {
        disapear();
    }
}

void Enemy::disapear() {

}

void Enemy::move() {

}