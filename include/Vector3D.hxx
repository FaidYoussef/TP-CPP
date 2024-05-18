// Vector3D.hxx

#ifndef VECTOR3D_HXX
#define VECTOR3D_HXX

class Vector3D {
private:
    double x;
    double y;
    double z;

public:
    // Constructeurs
    Vector3D(double x, double y, double z);
    Vector3D();
    // Destructor
    ~Vector3D();

    // Copy constructor
    Vector3D(const Vector3D &other) ;

    // Copy assignment operator

    // Accesseurs
    double getX() const;
    double getY() const;
    double getZ() const;

    // Mutateurs
    void setX(double x);
    void setY(double y);
    void setZ(double z);

    // Opérateurs surchargés
    Vector3D& operator=(const Vector3D &other) ;
    Vector3D operator-(const Vector3D& other) const;
    Vector3D operator+(const Vector3D& other) const;
    Vector3D operator+=(const Vector3D& other);
    Vector3D operator*(const double& scalar) const;
    double operator*(const Vector3D& other) const;
    bool operator==(const Vector3D& other) const;
    bool operator!=(const Vector3D& other) const;

    // Méthodes
    double norm() const;
};

#endif // VECTOR3D_HXX
