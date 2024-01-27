#include "PowerUp.h"

int PowerUp::getValue() const {
    return value;
}

void PowerUp::setValue(int newValue) {
    value = newValue;
}

AnimationFilm* PowerUp::getFilm() const {
    return m_singleFilm;
}







void PowerUp::setFilm(AnimationFilm* newFilm) {
    m_singleFilm = newFilm;
}
