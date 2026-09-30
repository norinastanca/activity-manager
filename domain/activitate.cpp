#include "activitate.h"


Activitate& Activitate::operator=(const Activitate& act) {

    this->titlu = act.titlu;
    this->descriere = act.descriere;
    this->tip = act.tip;
    this->durata = act.durata;

    return *this;
}

int Activitate::getDurata() const {
    return this->durata;
}

const string& Activitate::getTitlu() const {
    return this->titlu;
}

const string& Activitate::getDescriere() const {
    return this->descriere;
}

const string& Activitate::getTip() const {
    return this->tip;
}