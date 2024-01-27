#include "PowerUp.h"

int PowerUp::getValue() const {
    return value;
}

AnimationFilm* PowerUp::getFilm() const {
    return m_singleFilm;
}




void PowerUp::setValue(int newValue) {
    value = newValue;
}

void PowerUp::setFilm(AnimationFilm* newFilm) {
    m_singleFilm = newFilm;
}
