#include "Cellule.hxx"
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

Cellule::Cellule( Vector3D centre, std::vector<Particule3D> particules) {
    this->centre = centre;
    this->particules = particules;
    this->nbParticules = particules.size();
}

Cellule::Cellule() : nbParticules(0), particules(std::vector<Particule3D>()) {}



int Cellule::getNbParticules() const {
    return nbParticules;
}

std::vector<Particule3D> Cellule::getParticules() {
    return particules;
}

Vector3D Cellule::getCentre() const {
    return centre;
}

void Cellule::setParticules(std::vector<Particule3D> particules) {
    this->particules = particules;
}

void Cellule::addParticule(Particule3D particule) {
    particules.push_back(particule);
    nbParticules++;
}

void Cellule::removeParticule(Particule3D particule) {
    for (int i = 0; i < nbParticules; i++) {
        if (particules[i].getId() == particule.getId()) {
            particules.erase(particules.begin() + i);
            nbParticules--;
            break;
        }
    }
}