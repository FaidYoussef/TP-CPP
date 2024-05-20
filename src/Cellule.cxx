#include "Cellule.hxx"
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

// Constructor
Cellule::Cellule(int id_1, int id_2, Vector3D centre, std::vector<Particule3D> particules) {
    this->id[0] = id_1;
    this->id[1] = id_2;
    this->centre = centre;
    this->particules = particules;
    this->nbParticules = static_cast<int>(particules.size());
}

// Constructor
Cellule::Cellule(int id_1, int id_2, Vector3D centre) {
    this->id[0] = id_1;
    this->id[1] = id_2;
    this->centre = centre;
    this->particules = std::vector<Particule3D>();
    this->nbParticules = 0;
}

// Default constructor
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

// Get the cell ID
int* Cellule::getId() {
    return id;
}

// Get the number of particles
int Cellule::getNbParticules() const {
    return nbParticules;
}

// Get the particles
std::vector<Particule3D>& Cellule::getParticules() {
    return particules;
}

// Get the center
Vector3D Cellule::getCentre() const {
    return centre;
}

// Set the particles
void Cellule::setParticules(std::vector<Particule3D> particules) {
    this->particules = particules;
    this->nbParticules = static_cast<int>(particules.size());
}

// Add a particle
void Cellule::addParticule(Particule3D particule) {
    particules.push_back(particule);
    nbParticules++;
}

// Remove a particle by object
void Cellule::removeParticule(const Particule3D& p) {
    try {
        auto new_end = std::remove(particules.begin(), particules.end(), p);
        if (new_end != particules.end()) {
            particules.erase(new_end, particules.end());
            nbParticules = static_cast<int>(particules.size());
        } else {
            throw std::runtime_error("Particle to be removed not found.");
        }
    } catch (const std::exception &e) {
        std::cerr << "Error in removeParticule: " << e.what() << std::endl;
        throw; // Re-throw the exception after logging it
    }

}

// Remove a particle by index
void Cellule::removeParticule(int index) {
    try {
        if (index < 0 || index >= (int)particules.size()) {
            throw std::out_of_range("Index out of range.");
        }
        particules.erase(particules.begin() + index);
        nbParticules = static_cast<int>(particules.size());
    } catch (const std::exception &e) {
        std::cerr << "Error in removeParticule: " << e.what() << std::endl;
        throw; // Re-throw the exception after logging it
    }

}

// Clear all particles
void Cellule::clearParticules() {
    try {
        particules.clear();
        nbParticules = 0;
    } catch (const std::exception &e) {
        std::cerr << "Error in clearParticules: " << e.what() << std::endl;
        throw; // Re-throw the exception after logging it
    }

    std::cout << "All particles cleared.\n";
}
