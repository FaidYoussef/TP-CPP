#include "Particule3D.hxx"
#include <iostream>


Particule3D::Particule3D(int id, float masse, int catégorie, Vector3D force, Vector3D position, Vector3D vitesse) {
    this->id = id;
    this->masse = masse;
    this->catégorie = catégorie;
    this->force = force;
    this->position = position;
    this->vitesse = vitesse;
}

Particule3D::Particule3D() : id(0), masse(0), catégorie(0), force(Vector3D()), position(Vector3D()), vitesse(Vector3D()) {}

// Destructor
Particule3D::~Particule3D() {
    // No dynamic memory to free, nothing to do here
}

// Copy constructor
Particule3D::Particule3D(const Particule3D &other) {
    id = other.id;
    masse = other.masse;
    catégorie = other.catégorie;
    force = other.force;
    position = other.position;
    vitesse = other.vitesse;
}

// Copy assignment operator
Particule3D& Particule3D::operator=(const Particule3D &other) {
    if (this == &other) {
        return *this; // Handle self-assignment
    }
    id = other.id;
    masse = other.masse;
    catégorie = other.catégorie;
    force = other.force;
    position = other.position;
    vitesse = other.vitesse;
    return *this;
}

bool Particule3D::operator<(const Particule3D& other) const {
    return id < other.id;
}

bool Particule3D::operator==(const Particule3D &other) const{
    return id == other.id;
}

void Particule3D::setPos(Vector3D pos) {
    position = pos;
}

void Particule3D::setVit(float x, float y, float z) {
    vitesse.setX(x);
    vitesse.setY(y);
    vitesse.setZ(z);
}

void Particule3D::setForce(Vector3D f) {
    force = f;
}

void Particule3D::setMasse(float m) {
    masse = m;
}

void Particule3D::setCategorie(int c) {
    catégorie = c;
}

void Particule3D::setId(int i) {
    id = i;
}

Vector3D Particule3D::getPos() const {
    return position;
}

Vector3D Particule3D::getVit() {
    return vitesse;
}

Vector3D Particule3D::getForce() {
    return force;
}

float Particule3D::getMasse() const {
    return masse;
}

int Particule3D::getCategorie() const {
    return catégorie;
}

int Particule3D::getId() const {
    return id;
}
