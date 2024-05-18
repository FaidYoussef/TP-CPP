#include "Cellule.hxx"
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

Cellule::Cellule(int id_1, int id_2, Vector3D centre, std::vector<Particule3D> particules) {
    this->id[0] = id_1;
    this->id[1] = id_2;
    this->centre = centre;
    this->particules = particules;
    this->nbParticules = (int) particules.size() ;
}
Cellule ::Cellule(int id_1, int id_2, Vector3D centre) {
    this->id[0] = id_1;
    this->id[1] = id_2;
    this->centre = centre;
    this->particules = std::vector<Particule3D>();
    this->nbParticules = 0;
}

Cellule::Cellule() : nbParticules(0), particules(std::vector<Particule3D>()) {}

// Destructor
Cellule::~Cellule() {
    // No dynamic memory to free, nothing to do here
}

// Copy constructor
Cellule::Cellule(const Cellule &other) {
    id[0] = other.id[0];
    id[1] = other.id[1];
    centre = other.centre;
    particules = other.particules;
    nbParticules = other.nbParticules;
}

// Copy assignment operator
Cellule& Cellule::operator=(const Cellule &other) {
    if (this == &other) {
        return *this; // Handle self-assignment
    }
    id[0] = other.id[0];
    id[1] = other.id[1];
    centre = other.centre;
    particules = other.particules;
    nbParticules = other.nbParticules;
    return *this;
}

int* Cellule::getId() {
    return id;
}

int Cellule::getNbParticules() const {
    return nbParticules;
}

std::vector<Particule3D>& Cellule::getParticules() {
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
