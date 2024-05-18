
#ifndef Particule3D_HXX
#define Particule3D_HXX

#include "Vector3D.hxx" // Inclure le fichier d'en-tête Vector2D

class Particule3D {
    private:
        int id;
        float masse;
        int catégorie;
        Vector3D force;
        Vector3D position;
        Vector3D vitesse;

    public:
        // Constructeurs
        Particule3D(int id, float masse, int catégorie, Vector3D force, Vector3D position, Vector3D vitesse);
        Particule3D();
        ~Particule3D();  // Destructor
        Particule3D(const Particule3D &other);  // Copy constructor
        Particule3D& operator=(const Particule3D &other);  // Copy assignment operator


        // Méthodes de modification des attributs
        void setPos(Vector3D pos);
        void setVit(float x, float y, float z);
        void setForce(Vector3D f);
        void setMasse(float m);
        void setCategorie(int c);
        void setId(int i);

        // Méthodes de récupération des attributs
        Vector3D getPos() const;
        Vector3D getVit();
        Vector3D getForce();
        float getMasse() const;
        int getCategorie() const;
        int getId() const;

        // Opérateur de comparaison <
        bool operator<(const Particule3D& other) const;
        bool operator==(const Particule3D &other) const;  // Operator ==
};

#endif 
