
#include "Vector3D.hxx"
#include <cmath>

Vector3D::Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

Vector3D::Vector3D() : x(0), y(0), z(0) {}

// Destructor
Vector3D::~Vector3D() {
    // No dynamic memory to free, nothing to do here
}

// Copy constructor
Vector3D::Vector3D(const Vector3D &other) : x(other.x), y(other.y), z(other.z) {}

// Copy assignment operator
Vector3D& Vector3D::operator=(const Vector3D &other) {
    if (this == &other) {
        return *this; // Handle self-assignment
    }
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

void Vector3D::setX(double x) {
    this->x = x;
}

void Vector3D::setY(double y) {
    this->y = y;
}

void Vector3D::setZ(double z) {
    this->z = z;
}

double Vector3D::getX() const {
    return x;
}

double Vector3D::getY() const {
    return y;
}

double Vector3D::getZ() const {
    return z;
}

Vector3D Vector3D::operator-(const Vector3D& other) const {
    return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D Vector3D::operator+=(const Vector3D& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;   
}
Vector3D Vector3D::operator+(const Vector3D& other) const {
    return Vector3D(x + other.x, y + other.y, z + other.z);
}


Vector3D Vector3D::operator*(const double& scalar) const {
    return Vector3D(x * scalar, y * scalar, z * scalar);
}

double Vector3D::operator*(const Vector3D& other) const {
    return x * other.x + y * other.y + z * other.z;
}

double Vector3D::norm() const {
    return sqrt(x * x + y * y + z * z);
}

bool Vector3D::operator==(const Vector3D& other) const {
    return x == other.x && y == other.y && z == other.z;
}

bool Vector3D::operator!=(const Vector3D& other) const {
    return !(*this == other);
}